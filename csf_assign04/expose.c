//Lucy Zhang, Ajay Ananthakrishnan
//wzhang99@jhu.edu, ajayananth@jhu.edu
//
// expose plugin: changes all red/green/blue color component 
// values by a specified factor
//

#include <stdlib.h>
#include "image_plugin.h"

struct Arguments {
        //accepts single argument, exposure factor
        float factor;
};

const char *get_plugin_name(void) {
        return "expose";
}

const char *get_plugin_desc(void) {
        return "adjust the intensity of all pixels";
}

void *parse_arguments(int num_args, char *args[]) {
        if (num_args != 1) { //tile takes in 1 argument
            return NULL;
        }
		float factor = (atof)(args[0]);
		if (factor < 0) { //factor can't be less than 0
			return NULL;
		}
        struct Arguments *arg = malloc(sizeof(struct Arguments));
        arg->factor = factor;
        return arg;
}

// Helper function to change red/green/blue color component values by 
// specified factor
static uint32_t expose(uint32_t pix, float factor) {
        uint8_t r, g, b, a;
        img_unpack_pixel(pix, &r, &g, &b, &a);

        //check to make sure pixel rgb values are not 'overexposed'
        if (r * factor > 255) { //red
            r = 255; 
        } else {
            r = r * factor;
        }

        if (g * factor > 255) { //green
            g = 255; 
        } else {
            g = g * factor;
        }

        if (b * factor > 255) { //blue
            b = 255; 
        } else {
            b = b * factor;
        }

        return img_pack_pixel(r, g, b, a);
}

struct Image *transform_image(struct Image *source, void *arg_data) {
        struct Arguments *args = arg_data;

        // Allocate a result Image
        struct Image *out = img_create(source->width, source->height);
        if (!out) {
            free(args);
            return NULL;
        }

        unsigned num_pixels = source->width * source->height;
		//loop through all pixels and expose each one
        for (unsigned i = 0; i < num_pixels; i++) {
            out->data[i] = expose(source->data[i], args->factor);
        }

        free(args);
        return out;
}



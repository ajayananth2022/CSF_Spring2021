//Lucy Zhang, Ajay Ananthakrishnan
//wzhang99@jhu.edu, ajayananth@jhu.edu
//
// Example plugin: it just swaps the blue and green color component
// values for each pixel in the source image.
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
        (void) args; // this is just to avoid a warning about an unused parameter

        if (num_args != 1) {
                return NULL;
        }
        struct Arguments *arg = malloc(sizeof(struct Arguments));
        arg->factor = (atof)(args[0]);

        if (arg->factor < 0) {
                return NULL;
        }

        return arg;
}

// Helper function to change red/green/blue color component values by specified factor
static uint32_t expose(uint32_t pix, float factor) {
        uint8_t r, g, b, a;
        img_unpack_pixel(pix, &r, &g, &b, &a);

        //check to make sure pixel rgb values are not 'overexposed'
        if (r * factor >255) {
                r = 255; 
        } else {
                r= r * factor;
        }

        if (g * factor >255) {
                g = 255; 
        } else {
                g= g * factor;
        }

        if (b * factor >255) {
                b = 255; 
        } else {
                b= b * factor;
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
        for (unsigned i = 0; i < num_pixels; i++) {
                out->data[i] = expose(source->data[i], args->factor);
        }

        free(args);

        return out;
}



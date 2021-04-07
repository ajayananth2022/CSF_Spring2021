//Lucy Zhang, Ajay Ananthakrishnan
//wzhang99@jhu.edu, ajayananth@jhu.edu
//
// mirrorh plugin: generates a mirror image of the input image,
// with all pixels being reflected horizontally
//

#include <stdlib.h>
#include "image_plugin.h"

struct Arguments {
	// just define a single dummy field.
	int dummy;
};

const char *get_plugin_name(void) {
	return "mirrorh";
}

const char *get_plugin_desc(void) {
	return "mirror image horizontally";
}

void *parse_arguments(int num_args, char *args[]) {
	(void) args; // this is just to avoid a warning about an unused parameter

	if (num_args != 0) {
		return NULL;
	}
	return calloc(1, sizeof(struct Arguments));
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
	int row = 0; //row index
	int col = 0; //column index
	int width = source->width; 

	//loop through all pixels
	for (unsigned i = 0; i < num_pixels; i++) {
		if (col > width - 1) { //change to a new row
			col = 0; 
			row++; 
		}
		//reverse pixels horizontally
		out->data[col + row * width] = source->data[width - 1 - col + row * width]; 
		col++; 
	}

	free(args);
	return out;
}

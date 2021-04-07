//Lucy Zhang, Ajay Ananthakrishnan
//wzhang99@jhu.edu, ajayananth@jhu.edu
//
// mirrorv plugin: generates a mirror image of the input image,
// with all pixels being reflected vertically
//

#include <stdlib.h>
#include "image_plugin.h"

struct Arguments {
	// This plugin doesn't accept any command line arguments;
	int dummy;
};

const char *get_plugin_name(void) {
	return "mirrorv";
}

const char *get_plugin_desc(void) {
	return "mirror image vertically";
}

void *parse_arguments(int num_args, char *args[]) {
	(void) args; 

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
	int height = source->height; 
	int width = source->width; 

	//loop through all pixels
	for (unsigned i = 0; i < num_pixels; i++) {
		if (col > width - 1) { //change to a new row
			col = 0; 
			row++; 
		}
		//reverse pixels vertically
		out->data[col + row * width] = source->data[(height - 1 - row) * width + col]; 
		col++; 
	}

	free(args);
	return out;
}

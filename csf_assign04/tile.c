//Lucy Zhang, Ajay Ananthakrishnan
//wzhang99@jhu.edu, ajayananth@jhu.edu
//
// Example plugin: it just swaps the blue and green color component
// values for each pixel in the source image.
//

#include <stdlib.h>
#include "image_plugin.h"

struct Arguments {
	// This plugin doesn't accept any command line arguments;
	// just define a single dummy field.
	int num_tiles;
};

const char *get_plugin_name(void) {
	return "tile";
}

const char *get_plugin_desc(void) {
	return "tile source image in an NxN arrangement";
}

void *parse_arguments(int num_args, char *args[]) {
	if (num_args != 1) {
		return NULL;
	}
	int tiles = atoi(args[0]);
	if (tiles <= 0) {
		return NULL;
	}
	struct Arguments *arguments = malloc(sizeof(struct Arguments));
	arguments->num_tiles = tiles;
	return (void *)arguments;
}

unsigned calculate_tile_len(unsigned source_len, unsigned total_tiles, unsigned index) {
	unsigned tile_len = source_len / total_tiles;
	unsigned excess = source_len % total_tiles; //number of excess pixels
	if (index < excess) return tile_len + 1;
	return tile_len;
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
	unsigned tiles = args->num_tiles;


	unsigned row_num = 0; 
	unsigned column_num = 0; 

	for (unsigned i = 0; i < num_pixels; i++) {
		//out->data[i] = swap_bg(source->data[i]);
	}

	free(args);

	return out;
}

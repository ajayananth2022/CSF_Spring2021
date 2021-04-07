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

unsigned tile_idx(unsigned excess, unsigned bound, unsigned tile_len, unsigned idx) {
	if (excess != 0) {
		if (idx < bound) return idx % (tile_len + 1);
		return (idx - bound) % tile_len;
	} 
	return idx % tile_len;
}

struct Image *transform_image(struct Image *source, void *arg_data) {
	struct Arguments *args = arg_data;
	// Allocate a result Image
	struct Image *out = img_create(source->width, source->height);
	if (!out) {
		free(args);
		return NULL;
	}
	unsigned tiles = args->num_tiles;
	unsigned source_width = source->width;
	unsigned source_height = source->height;
	unsigned tile_width = source_width / tiles;
	unsigned tile_height = source_height / tiles;
	unsigned width_excess = source_width % tiles;
	unsigned height_excess = source_height % tiles;
	unsigned width_bound = (tile_width + 1) * width_excess;
	unsigned height_bound = (tile_height + 1) * height_excess;

	for (unsigned r = 0; r < source_height; r++) {
		unsigned tile_r = tiles * tile_idx(height_excess, height_bound, tile_height, r);
		for (unsigned c = 0; c < source_width; c++) {
			unsigned tile_w = tiles * tile_idx(width_excess, width_bound, tile_width, c);
			out->data[c + r * source_width] = source->data[tile_c + tile_r * source_width];
		}
	}
	free(args);
	return out;
}

//Lucy Zhang, Ajay Ananthakrishnan
//wzhang99@jhu.edu, ajayananth@jhu.edu
//
// tile plugin: generates an image containing an N x N arrangement 
// of tiles, each tile being a smaller version of the original image, and 
// the overall result image having the same dimensions as the original image
//

#include <stdlib.h>
#include "image_plugin.h"

struct Arguments {
	//accepts single argument, tile factor
	int num_tiles;
};

const char *get_plugin_name(void) {
	return "tile";
}

const char *get_plugin_desc(void) {
	return "tile source image in an NxN arrangement";
}

void *parse_arguments(int num_args, char *args[]) {
	if (num_args != 1) { //tile takes in 1 argument
		return NULL;
	}
	int tiles = atoi(args[0]);
	if (tiles <= 0) { //number of tile has to be a positive integer
		return NULL;
	}
	struct Arguments *arguments = malloc(sizeof(struct Arguments));
	arguments->num_tiles = tiles;
	return (void *)arguments;
}

//helper function to find the corresponding tile index in source from
//a given index in the output image
unsigned tile_idx(unsigned excess, unsigned bound, unsigned tile_len, unsigned idx) {
	if (excess != 0) { // if not divisible
		//pixels within the bound that have excess
		if (idx < bound) return idx % (tile_len + 1); 
		//pixels outside the bound (no excess)
		return (idx - bound) % tile_len;
	} 
	return idx % tile_len; // if divisible (no excess pixels)
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
	unsigned width = source->width;
	unsigned height = source->height;
	unsigned tile_w = width / tiles; //width of each tile
	unsigned tile_h = height / tiles; //height of each tile
	
	//excess pixels in the width and height
	unsigned width_excess = width % tiles; 
	unsigned height_excess = height % tiles;
	
	//the bound between tiles that have excess pixels
	unsigned width_bound = (tile_w + 1) * width_excess;
	unsigned height_bound = (tile_h + 1) * height_excess;
	
	//loop through out's rows and cols and find corresponding rows and cols in source
	for (unsigned r = 0; r < height; r++) {
		unsigned tile_r = tiles * tile_idx(height_excess, height_bound, tile_h, r);
		for (unsigned c = 0; c < width; c++) {
			unsigned tile_c = tiles * tile_idx(width_excess, width_bound, tile_w, c);
			out->data[c + r * width] = source->data[tile_c + tile_r * width];
		}
	}
	free(args);
	return out;
}

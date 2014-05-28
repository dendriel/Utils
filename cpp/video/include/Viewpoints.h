/*
 * Viewpoints.h
 *
 *  Created on: 15/05/2014
 *      Author: vitor
 */

#ifndef VIEWPOINTS_H_
#define VIEWPOINTS_H_

#include <string>
#include <vector>
#include <tr1/cstdint>

#include "SDL/SDL.h"

#include "VisualElement.h"


//!< Define color keys.
#define RED 255
#define GREEN 0
#define BLUE 255
#define BPP 32

//!< Represents a cartographic position.
typedef struct {
	int x;
	int y;
} st_element_pos;

class Viewpoints {
public:
	/**
	 * Constructor.
	 */
	Viewpoints();

	/**
	 * Destructor.
	 */
	virtual ~Viewpoints();

	/**
	 * \brief Build a viewpoint array from the raw image.
	 * \param source The raw image that will be used as data source.
	 * \param position Number of positions in the source.
	 * \param views Number of views per positions.
	 * \param viewpoints The array that will be filled.
	 * \return 0 if successfully filled the array; -1 if any error has occurred.
	 */
	static int build_viewpoints(const char *source,
			const uint32_t positions,
			const uint32_t views,
			SDL_Surface **viewpoints);

	/**
	 * \brief Build a scenery layer.
	 * \param layer The surface that will contain the builded layer.
	 * \param layer_bounds Contain the matrix maximum x and y.
	 * \param tile_list The tile array.
	 * \param tile_size Size of the tile to be blit from the source image.
	 * \param source The tile set data source.
	 * \return 0 for success; -1 for error.
	 */
	static int build_layer(SDL_Surface **layer,
			const st_element_pos layer_bounds,
			const uint32_t *tile_list,
			st_element_pos& tile_size,
			const std::string& source);


	/**
	 * \brief Build a scenery layer.
	 * \param layer The surface that will contain the builded layer.
	 * \param layer_bounds Contain the matrix maximum x and y.
	 * \param tile_list The tile array.
	 * \param source The tile set data source.
	 * \return 0 for success; -1 for error.
	 */
	static int build_layer_hex(SDL_Surface **layer,
			const st_element_pos layer_bounds,
			const uint32_t *tile_list,
			const std::string& source,
			const st_element_pos tile_size);

	/**
	 * \brief Loads an optimized SDL surface from a source.
	 * \param source The source image (bmp) to load from.
	 * \param surface A pointer to put the optimized surface.
	 * \return 0 if could successfully load the image; -1 otherwise.
	 */
	static int load_surface(const char *source, SDL_Surface **surface);

	/**
	 * \brief Creates a SDL Surface.
	 * \param width The width of the surface.
	 * \param height The height of the surface.
	 * \return A pointer to the surface.
	 */
	static SDL_Surface *create_surface(const uint32_t& width, const uint32_t& height);

	/**
	 * \brief Draw all elements from a VisualElement's list in the destination SDL Surface.
	 * \param source The list with VisualElements
	 * \param destn The SDL Surface that will received the list.
	 */
	static void draw_visual_list(std::vector <VisualElement *>& source, SDL_Surface *destn);


	/**
	 * \brief Paint the whole surface with a color.
	 * \param surface The surface to be painted.
	 * \param r Red color.
	 * \param g Blue color.
	 * \param b Green color.
	 * \param color A color value.
	 */
	static void paint_surface(SDL_Surface *surface, const uint8_t r=RED, const uint8_t g=GREEN, const uint8_t b=BLUE);
	static void paint_surface(SDL_Surface *surface, uint32_t color);

};


#endif /* VIEWPOINTS_H_ */

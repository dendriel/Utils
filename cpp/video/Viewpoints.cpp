/*
 * Viewpoints.cpp
 *
 *  Created on: 15/05/2014
 *      Author: vitor
 */

#include "Viewpoints.h"
#include <iostream>
#include <assert.h>

using namespace std;

/* SDL interprets each pixel as a 32-bit number, so our masks must depend
 * on the endianness (byte order) of the machine.
 */
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
#define rmask 0xff000000
#define gmask 0x00ff0000
#define bmask 0x0000ff00
#define amask 0x000000ff
#else
#define rmask 0x000000ff
#define gmask 0x0000ff00
#define bmask 0x00ff0000
#define amask 0xff000000
#endif

/*************************************************************************************************/

//!< Checks and print error if the operation fails.
#define CHK_NULL(_f)\
{\
	if ((_f) == NULL) {\
		cout << "[ViewPoints] Error occurred at " << #_f << endl;\
		assert(0);\
	}\
}

//!< Checks and print error if the operation fails.
#define CHK(_f, _r)\
{\
	if ((_f) == (_r)) {\
		cout << "[ViewPoints] Error occurred at " << #_f << "; Returned: " << _r << endl;\
		return -1;\
	}\
}

#define ASSERT(_f)\
{\
	int32_t _ret;\
	if ((_ret = _f) != 0) {\
		cout << "[ViewPoints] Error occurred at " << #_f << "; Returned: " << _ret << endl;\
		assert(0);\
	}\
}

#define hex_x_offset(px, py, hw) ((py%2 == 0)? px*hw : ((hw/2)+px*hw))
#define hex_y_offset(px, py, hh) ((py%2 == 0)? ((py == 0)? 0 : (py/2)*hh + (py/2)*hh/2) : (hh/4)*3*py)

/*************************************************************************************************/
/**
 * \brief Find x from a matrix with the array index.
 * \parameter index The position of the element in array representation.
 * \parameter x_max The width of the matrix.
 * \return The x coordinate in matrix representation.
 */
inline uint32_t find_x(const uint32_t index, const uint32_t x_max)
{
	return ((index % x_max) );
}

/*************************************************************************************************/
/**
 * \brief Find y from a matrix with the array index.
 * \parameter index The position of the element in array representation.
 * \parameter x_max The width of the matrix.
 * \parameter x The x coordinate that must be previous calculated.
 * \return The y coordinate in matrix representation.
 */
inline uint32_t find_y(const uint32_t index, const uint32_t x_max, const uint32_t x)
{
	return (((index - x)/x_max) );
}

/*************************************************************************************************/
/**
 * \brief Find the array position from matrix coordinates.
 * \parameter p The position (row) index.
 * \parameter v The view (column) index.
 * \return The array position.
 */
inline uint32_t find_index(const uint32_t p, const uint32_t v, const uint32_t vmax)
{
	/*
	 * Can't remember if was working on the way that is.. but this new formula works very well:
	 * return ((y * xmax) + x );
	 */
	return ((( (p * vmax) - vmax ) + v ) - 1);
}

/*************************************************************************************************/

int Viewpoints::build_viewpoints(const char *source, const uint32_t positions, const uint32_t views, SDL_Surface **viewpoints)
{
	SDL_Surface *image_source = NULL;
	SDL_Rect viewpoint_size;
	SDL_Surface* optimizedImage = NULL;
	SDL_Surface *viewpoint = NULL;
	uint32_t position;

	CHK_NULL(image_source = SDL_LoadBMP(source));

	viewpoint_size.h = image_source->h/positions;
	viewpoint_size.w = image_source->w/views;

	for (unsigned char p_index = 1; p_index <= positions; ++p_index) {

		for (unsigned char v_index = 1; v_index <= views; ++v_index) {

			CHK_NULL((viewpoint = SDL_CreateRGBSurface(SDL_SWSURFACE, viewpoint_size.w, viewpoint_size.h,
					BPP, rmask, gmask, bmask, amask)));

			SDL_SetColorKey(viewpoint , SDL_SRCCOLORKEY, SDL_MapRGB(viewpoint->format, RED, GREEN, BLUE));

			position = find_index(p_index, v_index, views);
			viewpoint_size.x = viewpoint_size.w*(v_index-1);
			viewpoint_size.y = viewpoint_size.h*(p_index-1);;

			CHK_NULL((optimizedImage = SDL_DisplayFormat(viewpoint)));

			SDL_FreeSurface(viewpoint);

			CHK(SDL_BlitSurface(image_source, &viewpoint_size, optimizedImage, NULL), -1);
			viewpoints[position] = optimizedImage;
		}
	}

	return 0;
}

/*************************************************************************************************/

int Viewpoints::build_layer(SDL_Surface **layer,
		const st_element_pos layer_bounds,
		const uint32_t *tile_list,
		st_element_pos& tile_size,
		const string& source)
{
	SDL_Surface *source_tileset = NULL;
	SDL_Surface *layer_temp = NULL;
	SDL_Rect tile_data = {0, 0, tile_size.x, tile_size.y};
	SDL_Rect draw_offset = {0, 0, 0, 0};
	const uint32_t tile_list_size = layer_bounds.x * layer_bounds.y;

	/* Load map tile set. */
	CHK_NULL((source_tileset = SDL_LoadBMP(source.c_str())));

	/* Create layer surface.*/
	CHK_NULL((layer_temp = SDL_CreateRGBSurface(SDL_SWSURFACE,
			layer_bounds.x*tile_size.x, layer_bounds.y*tile_size.y,
			BPP, rmask, gmask, bmask, amask)));

	for (uint32_t i = 0; i < tile_list_size; ++i) {
		uint32_t x;
		uint32_t y;

		x = find_x(i, layer_bounds.x);
		y = find_y(i, layer_bounds.x, x);
		draw_offset.x = (x * tile_size.x);
		draw_offset.y = (y * tile_size.y);

		tile_data.x = tile_size.x * find_x(tile_list[i], 4); //               \/ Don't know if is tile_size. (x or y)
		tile_data.y = tile_size.y * find_y(tile_list[i], 4, (tile_data.x/tile_size.x));

		CHK(SDL_BlitSurface(source_tileset, &tile_data, layer_temp, &draw_offset), -1);
	}

	CHK_NULL((*layer = SDL_DisplayFormat(layer_temp)));

	return 0;
}

/*************************************************************************************************/

int Viewpoints::build_layer_hex(SDL_Surface **layer, const st_element_pos layer_bounds,
		const uint32_t *tile_list,
		const string& source,
		const st_element_pos tile_size)
{
	SDL_Surface *source_tileset = NULL;
	SDL_Surface *layer_temp = NULL;
	SDL_Rect tile_data = {0, 0, tile_size.x, tile_size.y};
	SDL_Rect draw_offset = {0, 0, 0, 0};
	const uint32_t tile_list_size = layer_bounds.x * layer_bounds.y;

	/* Load map tile set. */
	load_surface(source.c_str(), &source_tileset);

	CHK_NULL(layer_temp = create_surface(layer_bounds.x*tile_size.x + tile_size.x, layer_bounds.y*tile_size.y));

	for (uint32_t i = 0; i < tile_list_size; ++i) {
		uint32_t x;
		uint32_t y;

		x = find_x(i, layer_bounds.x);
		y = find_y(i, layer_bounds.x, x);
		draw_offset.x = hex_x_offset(x, y, tile_size.x);
		draw_offset.y = hex_y_offset(x, y, tile_size.y);

		tile_data.x = tile_size.x * find_x(tile_list[i], 4);
		tile_data.y = 0;

		SDL_BlitSurface(source_tileset, &tile_data, layer_temp, &draw_offset);
	}

	CHK_NULL(*layer = SDL_DisplayFormat(layer_temp));

	delete layer_temp;

	return 0;
}

/*************************************************************************************************/

int Viewpoints::load_surface(const char *source, SDL_Surface **surface)
{
	SDL_Surface *image_source = NULL;

	CHK_NULL(image_source = SDL_LoadBMP(source));
	CHK_NULL(*surface = SDL_DisplayFormat(image_source));

	delete image_source;

	assert(SDL_SetColorKey(*surface , SDL_SRCCOLORKEY, SDL_MapRGB((*surface)->format, RED, GREEN, BLUE)) == 0);

	return 0;
}

/*************************************************************************************************/

SDL_Surface *Viewpoints::create_surface(const uint32_t& width, const uint32_t& height)
{
	SDL_Surface *optimized_surface = NULL;
	SDL_Surface *surface = SDL_CreateRGBSurface(SDL_SWSURFACE, width, height, 32, rmask, gmask, bmask, amask);

	CHK_NULL(optimized_surface = SDL_DisplayFormat(surface));

	delete surface;

	assert(SDL_SetColorKey(optimized_surface , SDL_SRCCOLORKEY, SDL_MapRGB(optimized_surface->format, RED, GREEN, BLUE)) == 0);

	/* Make surface transparent. */
	Viewpoints::paint_surface(optimized_surface);

	assert(optimized_surface != NULL);

	return optimized_surface;
}

/*************************************************************************************************/

void Viewpoints::draw_visual_list(vector <VisualElement *>& source, SDL_Surface *destn)
{
	for (vector<VisualElement *>::iterator iter = source.begin(); iter != source.end(); ++iter) {
			SDL_Rect elem_offset = (*iter)->get_offset();
			SDL_Surface *elem_viewpoint = (*iter)->get_viewpoint();
			SDL_BlitSurface(elem_viewpoint, NULL, destn, &elem_offset);
	}
}

/*************************************************************************************************/
//!< Facility interface.
void Viewpoints::paint_surface(SDL_Surface *surface, const uint8_t r, const uint8_t g, const uint8_t b)
{
	paint_surface(surface, SDL_MapRGB(surface->format, r, g, b));
}

void Viewpoints::paint_surface(SDL_Surface *surface, uint32_t color)
{
	/* Fill whole surface with color. */
	ASSERT(SDL_FillRect(surface, NULL, color));
}

/*************************************************************************************************/

#ifdef RED
#undef RED
#endif /* RED */
#ifdef GREEN
#undef GREEN
#endif /* GREEN */
#ifdef BLUE
#undef BLUE
#endif /* BLUE */
#ifdef BPP
#undef BPP
#endif /* BPP */

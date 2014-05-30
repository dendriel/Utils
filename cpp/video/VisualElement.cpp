#include "VisualElement.h"
#include "Viewpoints.h"

#include <iostream>
#include <assert.h>

#include "debug.h"

using namespace std;

unsigned int VisualElement::s_visualElement_ids;

/*************************************************************************************************/
VisualElement::VisualElement(const int x, const int y, string img_source):
m_Index(0),
m_Id(VisualElement::generate_id())
{
	/* Initialize the viewpoints vector. */
	memset(m_Viewpoints, 0, sizeof(m_Viewpoints));

	m_Offset_lock = SDL_CreateMutex();
	m_Viewpoints_lock = SDL_CreateMutex();
	m_Index_lock = SDL_CreateMutex();

	m_Id = VisualElement::generate_id();

	set_x(x);
	set_y(y);

	/* Load first viewpoint if source is specified. */
	if (img_source != "") {
		SDL_Surface *img = NULL;
		Viewpoints::load_surface(img_source.c_str(), &img);
		if (img == NULL) {
			assert_exit("VisualElement [" << m_Id << "] Failed to load image source from \"" << img_source << "\"");
		}
		else {
			set_viewpoint(img);
		}
	}

	debug("VisualElement [" << m_Id << "] created.");
}

/*************************************************************************************************/
VisualElement::~VisualElement(void)
{
	for (unsigned int i = 0; i < MAX_VIEWPOINTS; ++i) {
		if (m_Viewpoints[i] != NULL) {
			SDL_FreeSurface(m_Viewpoints[i]);
			m_Viewpoints[i] = NULL;
		}
	}

	SDL_DestroyMutex(m_Offset_lock);
	SDL_DestroyMutex(m_Viewpoints_lock);
	SDL_DestroyMutex(m_Index_lock);

	debug("VisualElement [" << m_Id << "] destroyed.");
}

/*************************************************************************************************/
void VisualElement::set_x(const int x)
{
	SDL_LockMutex(m_Offset_lock);
	m_Offset.x = x;
	SDL_UnlockMutex(m_Offset_lock);
}

/*************************************************************************************************/

int VisualElement::get_x(void)
{
	int x;

	SDL_LockMutex(m_Offset_lock);
	x = m_Offset.x;
	SDL_UnlockMutex(m_Offset_lock);

	return x;
}

/*************************************************************************************************/
void VisualElement::set_y(const int y)
{
	SDL_LockMutex(m_Offset_lock);
	m_Offset.y = y;
	SDL_UnlockMutex(m_Offset_lock);
}

/*************************************************************************************************/
int VisualElement::get_y(void)
{
	int y;

	SDL_LockMutex(m_Offset_lock);
	y = m_Offset.x;
	SDL_UnlockMutex(m_Offset_lock);

	return y;
}

/*************************************************************************************************/
int VisualElement::set_viewposition(const unsigned int position)
{
	if (position >= MAX_VIEWPOINTS) {
		return -1;
	}
	SDL_LockMutex(m_Index_lock);
	m_Index = position;
	SDL_UnlockMutex(m_Index_lock);

	return 0;
}

/*************************************************************************************************/
unsigned int VisualElement::get_viewposition(void)
{
	unsigned int position;

	SDL_LockMutex(m_Index_lock);
	position = m_Index;
	SDL_UnlockMutex(m_Index_lock);

	return position;
}


/*************************************************************************************************/
void VisualElement::set_offset(const int16_t x, const int16_t y)
{
	SDL_LockMutex(m_Offset_lock);
	m_Offset.x = x;
	m_Offset.y = y;
	SDL_UnlockMutex(m_Offset_lock);
}

/*************************************************************************************************/
void VisualElement::add_offset(const int16_t x, const int16_t y)
{
	SDL_LockMutex(m_Offset_lock);
	m_Offset.x = m_Offset.x + x;
	m_Offset.y = m_Offset.y + y;
	SDL_UnlockMutex(m_Offset_lock);
}

/*************************************************************************************************/
SDL_Rect VisualElement::get_offset(void)
{
	SDL_Rect offset;

	SDL_LockMutex(m_Offset_lock);
	offset = m_Offset;
	SDL_UnlockMutex(m_Offset_lock);

	return offset;
}

/*************************************************************************************************/

int VisualElement::set_viewpoint(SDL_Surface *image, const unsigned int position)
{
	if ((position >= MAX_VIEWPOINTS) || (image == NULL)) {
		return -1;
	}

	SDL_LockMutex(m_Viewpoints_lock);
	m_Viewpoints[position] = image;
	SDL_UnlockMutex(m_Viewpoints_lock);

	return 0;
}

/*************************************************************************************************/

int VisualElement::update_viewpoint(SDL_Surface *image, const unsigned int position)
{
	if ((position >= MAX_VIEWPOINTS) || (image == NULL)) {
		return -1;
	}

	if (m_Viewpoints[position] == NULL) {
		assert_exit("NULL POINTER DEFERENCE. [" << m_Id << "] Trying to update a NULL surface in the visual element.");
	}

	SDL_LockMutex(m_Viewpoints_lock);
	assert(SDL_BlitSurface(image, NULL, m_Viewpoints[position], NULL) == 0);
	SDL_UnlockMutex(m_Viewpoints_lock);

	return 0;
}

/*************************************************************************************************/

SDL_Surface *VisualElement::get_viewpoint(void)
{
	SDL_Surface *viewpoint = NULL;

	SDL_LockMutex(m_Viewpoints_lock);
	viewpoint = m_Viewpoints[get_viewposition()];
	SDL_UnlockMutex(m_Viewpoints_lock);

	return viewpoint;
}

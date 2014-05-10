#include "VisualElement.h"

#include <iostream>

/*************************************************************************************************/
	VisualElement::VisualElement(const int x, const int y):
	m_Index(0)
	{
		/* Initialize the viewpoints vector. */
		memset(m_Viewpoints, 0, sizeof(m_Viewpoints));

		m_Offset_lock = SDL_CreateMutex();
		m_Viewpoints_lock = SDL_CreateMutex();
		m_Index_lock = SDL_CreateMutex();

		set_x(x);
		set_y(y);

		cout << "VisualElement initialized with x: " << x << "; y: " << y << endl;
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
	void VisualElement::set_offset(const int x, const int y)
	{
		SDL_LockMutex(m_Offset_lock);
		set_x(x);
		set_y(y);
		SDL_UnlockMutex(m_Offset_lock);
	}

/*************************************************************************************************/
	void VisualElement::add_offset(const int x, const int y)
	{
		SDL_LockMutex(m_Offset_lock);
		set_x(m_Offset.x + x);
		set_y(m_Offset.y + y);
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
		if (position >= MAX_VIEWPOINTS) {
			return -1;
		}

		SDL_LockMutex(m_Viewpoints_lock);
		m_Viewpoints[position] = image;
		SDL_UnlockMutex(m_Viewpoints_lock);

		return 0;
	}

/*************************************************************************************************/
	SDL_Surface *VisualElement::get_viewpoint(void)
	{
		return m_Viewpoints[get_viewposition()];
	}

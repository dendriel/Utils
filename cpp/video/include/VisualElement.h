#ifndef VISUALELEMENT_H
#define VISUALELEMENT_H

#include "SDL/SDL.h"
#include "SDL/SDL_mutex.h"

using namespace std;

#define MAX_VIEWPOINTS 30


class VisualElement {
private:
	SDL_Rect m_Offset;
	unsigned int m_Index;

	SDL_mutex *m_Offset_lock;
	SDL_mutex *m_Viewpoints_lock; //!< Is not allowed to free the viewpoints until the object ceases to exist.
	SDL_mutex *m_Index_lock;

public:
	VisualElement(const int x = 0, const int y = 0);
	~VisualElement(void);

	void set_x(const int x);
	int get_x(void);

	void set_y(const int y);
	int get_y(void);

	void set_offset(const int x, const int y);
	void add_offset(const int x, const int y);
	SDL_Rect get_offset(void);

	int set_viewpoint(SDL_Surface *image, const unsigned int position);

	int set_viewposition(const unsigned int position);
	unsigned int get_viewposition(void);
	SDL_Surface *get_viewpoint(void);

public:
	SDL_Surface *m_Viewpoints[MAX_VIEWPOINTS];
};

#endif /* VISUALELEMENT_H */

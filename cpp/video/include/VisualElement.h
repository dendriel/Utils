#ifndef VISUALELEMENT_H
#define VISUALELEMENT_H

#include <string>
#include <tr1/cstdint>

#include "SDL/SDL.h"
#include "SDL/SDL_mutex.h"


#define MAX_VIEWPOINTS 30


class VisualElement {
private:
	static unsigned int s_visualElement_ids;

	SDL_Rect m_Offset;
	unsigned int m_Index;
	unsigned int m_Id;

	SDL_mutex *m_Offset_lock;
	SDL_mutex *m_Viewpoints_lock; //!< Is not allowed to free the viewpoints until the object ceases to exist.
	SDL_mutex *m_Index_lock;

public:
	VisualElement(const int x = 0, const int y = 0, std::string img_source="");
	~VisualElement(void);

	void set_x(const int x);
	int get_x(void);

	void set_y(const int y);
	int get_y(void);

	void set_offset(const int16_t x, const int16_t y);
	void add_offset(const int16_t x, const int16_t y);
	SDL_Rect get_offset(void);

	int set_viewpoint(SDL_Surface *image, const unsigned int position=0);

	int update_viewpoint(SDL_Surface *image, const unsigned int position=0);

	int set_viewposition(const unsigned int position);
	unsigned int get_viewposition(void);
	SDL_Surface *get_viewpoint(void);

public:
	/*
	 * \brief Get VisualElement identifier.
	 * \return The VisualElement identifier.
	 */
	inline unsigned int get_Id(void)
	{
		return m_Id;
	}

	SDL_Surface *m_Viewpoints[MAX_VIEWPOINTS];

	/*
	 * \brief Get an unique VisualElement ID.
	 * \return An unique ID.
	 */
	static int generate_id(void)
	{
		return s_visualElement_ids++;
	}
};

#endif /* VISUALELEMENT_H */

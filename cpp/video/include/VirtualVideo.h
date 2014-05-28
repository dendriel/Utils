/*
 * VirtualVideo.h
 *
 *  Created on: 23/05/2014
 *      Author: vitor
 */
#ifndef VIRTUALVIDEO_H_
#define VIRTUALVIDEO_H_

#include <iostream>
#include <tr1/cstdint>

#include "SDL/SDL.h"
#include "SDL/SDL_thread.h"

#include "Video.h"
#include "VisualElement.h"


class VirtualVideo: public Video {
private:
	static uint32_t s_VirtualVideo_ids;

	VisualElement m_VirtualScreen;
	Video *m_RealVideo;			//!< Represents the real monitor screen when initialized as virtual screen.

public:
	/**
	 * \brief Class constructor for a virtual screen. Virtual screen will be the concept given to surfaces
	 * that will be drawn inside the real screen and will represent sub-screens.
	 */
	VirtualVideo(Video *real_screen,
			const uint16_t& width,
			const uint16_t& height,
			const int16_t& x=0,
			const int16_t& y=0,
			const uint32_t& interval = SCREEN_UPDATE_DEFAULT_TIME_MS);

	virtual ~VirtualVideo();

	/**
	 * \brief Initialize the screen.
	 * \param auto_start Informs if automatically start updating the screen after initializing.
	 */
	void init(bool auto_start=true);

	/**
	 * \brief Update the screen offset.
	 * \param x Horizontal offset.
	 * \param y Vertical offset.
	 * \note The own visual element that represents the screen have a mutex to lock before updating
	 * its offset value.
	 */
	void set_offset(const int16_t x, const int16_t y);

	/**
	 * \brief Get the virtual screen displaying offset.
	 * \return A SDL_Rect value containing the offset and dimensions of the screen.
	 */
	SDL_Rect get_offset(void);

	/**
	 * \brief Thread wrapper. Probably there is an easier way to do this.
	 */
	static int video_thread_wrapper(void* pParam)
	{
		return ((VirtualVideo*)pParam)->virtual_video_thread();
	}

private:

	int virtual_video_thread(void);
};

#endif /* VIRTUALVIDEO_H_ */

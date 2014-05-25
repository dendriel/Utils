/*
 * VirtualVideo.h
 *
 *  Created on: 23/05/2014
 *      Author: vitor
 */
#ifndef VIRTUALVIDEO_H_
#define VIRTUALVIDEO_H_

#include <iostream>

#include "SDL/SDL.h"
#include "SDL/SDL_thread.h"

#include "Video.h"
#include "VisualElement.h"


class VirtualVideo: Video {
private:
	static uint32_t s_VirtualVideo_ids;

	VisualElement m_VirtualScreen;
	Video *m_RealVideo;			//!< Represents the real monitor screen when initialized as virtual screen

public:
	/*
	 * \brief Class constructor for a virtual screen. Virtual screen will be the concept given to surfaces
	 * that will be drawn inside the real screen and will represent sub-screens.
	 */
	VirtualVideo(Video *real_screen, const uint32_t& width, const uint32_t& height, const uint32_t& interval = SCREEN_UPDATE_DEFAULT_TIME_MS);

	virtual ~VirtualVideo();

	void init(en_screen_mode mode=S_MODE_REAL);

	/*
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

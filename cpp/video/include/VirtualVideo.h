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


class VirtualVideo {
private:
	static unsigned int s_VirtualVideo_ids;

	VisualElement m_VirtualScreen;
	vector <VisualElement *> m_VisualElement_list;
	vector <VisualElement *> m_UnderLayer_list;
	Video *m_RealVideo;			//!< Represents the real monitor screen when initialized as virtual screen.
	int m_UpdateInterval_ms;	//!< Update interval in mili seconds.
	bool m_UpdateScreen_f;		//!< True: update the screen; False: hold until is true.
	bool m_KeepRunning;			//!< Flag to halt the update screen thread.
	unsigned int m_Id;			//!< Virtual Video unique identifier.

	SDL_Thread *m_Updater_tid;
	SDL_mutex *m_UpdateScreen_f_lock;

public:
	/*
	 * \brief Class constructor for a virtual screen. Virtual screen will be the concept given to surfaces
	 * that will be drawn inside the real screen and will represent sub-screens.
	 */
	VirtualVideo(Video *real_screen, const unsigned int& screen_width, const unsigned int& screen_height,
			const unsigned int& update_interval = SCREEN_UPDATE_DEFAULT_TIME_MS);

	virtual ~VirtualVideo();

	/*
	 * \brief Get Video identifier.
	 * \return The Video identifier.
	 */
	inline unsigned int get_Id(void)
	{
		return m_Id;
	}

	/*
	 * \brief Thread wrapper. Probably there is an easier way to do this.
	 */
	static int video_thread_wrapper(void* pParam)
	{
		return ((VirtualVideo*)pParam)->virtual_video_thread();
	}

private:

	int virtual_video_thread(void);

	bool get_updateScreen(void);
	void set_updateScreen(bool value);


	/*
	 * \brief Get an unique Video ID.
	 * \return An unique ID.
	 */
	static unsigned int generate_id(void)
	{
		return s_VirtualVideo_ids++;
	}
};

#endif /* VIRTUALVIDEO_H_ */

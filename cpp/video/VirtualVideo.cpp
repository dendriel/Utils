/*
 * VirtualVideo.cpp
 *
 *  Created on: 23/05/2014
 *      Author: vitor
 */

#include "VirtualVideo.h"

#include <iostream>
#include <unistd.h>
#include <assert.h>
#include <tr1/cstdint>

#include "Viewpoints.h"
#include "debug.h"

using namespace std;


VirtualVideo::VirtualVideo(Video *real_screen, const uint16_t& width, const uint16_t& height, const int16_t& x, const int16_t& y, const uint32_t& interval):
Video(width, height, "", interval),
m_RealVideo(real_screen)
{
	m_Screen_size.x = x;
	m_Screen_size.y = y;
	m_Screen_size.w = width;
	m_Screen_size.h = height;

	debug("Video [" << m_Id << "]" << " is Virtual.");
}

VirtualVideo::~VirtualVideo()
{
	set_updateScreen(false);

	/* Signal to update screen thread to exit. */
	int ret = 0;
	m_KeepRunning = false;
	SDL_WaitThread(m_Updater_tid, &ret);

	/* Clear but not delete. Maybe Video destructor also is executed.*/
	m_VisualElement_list.clear();
	m_UnderLayer_list.clear();
	m_Message_list.clear();

	m_RealVideo->rem_visualElement(m_VirtualScreen.get_Id());

	delete m_Screen;

	debug("Virtual Video [" << m_Id << "] Thread ret(" << ret << ") destroyed.");
}

/*************************************************************************************************/
void VirtualVideo::init(bool auto_start)
{
	/* Create layer surface.*/
	m_VirtualScreen.set_viewpoint(Viewpoints::create_surface(m_Screen_size.w, m_Screen_size.h));

	m_Screen = Viewpoints::create_surface(m_Screen_size.w, m_Screen_size.h);

	/* Set virtual screen offset. */
	m_VirtualScreen.set_offset(m_Screen_size.x, m_Screen_size.y);

	m_RealVideo->push_under_layer(&m_VirtualScreen);

	m_Updater_tid = SDL_CreateThread(&video_thread_wrapper, this);

	if (auto_start) {
		start();
	}

	debug("Virtual Video [" << m_Id << "] initialized.");
}

/*************************************************************************************************/

void VirtualVideo::set_offset(const int16_t x, const int16_t y)
{
	m_VirtualScreen.set_offset(x, y);
}

/*************************************************************************************************/

int VirtualVideo::virtual_video_thread(void)
{

	while(m_KeepRunning) {

		if (get_updateScreen() == true) {
			SDL_LockMutex(m_UnderLayer_list_lock);
			Viewpoints::draw_visual_list(m_UnderLayer_list, m_Screen);
			SDL_UnlockMutex(m_UnderLayer_list_lock);

			SDL_LockMutex(m_VisualElement_list_lock);
			Viewpoints::draw_visual_list(m_VisualElement_list, m_Screen);
			SDL_UnlockMutex(m_VisualElement_list_lock);

			SDL_LockMutex(m_Message_list_Lock);
			Viewpoints::draw_visual_list(m_Message_list, m_Screen);
			SDL_UnlockMutex(m_Message_list_Lock);

			assert(m_VirtualScreen.update_viewpoint(m_Screen) == 0);
		}

		usleep(m_UpdateInterval_ms*1000);
	}

	return 0;
}

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


using namespace std;


VirtualVideo::VirtualVideo(Video *real_screen, const uint32_t& width, const uint32_t& height, const uint32_t& interval):
Video(width, height, "", interval),
m_RealVideo(real_screen)
{
	cout << "Video [" << get_Id() << "]" << " is Virtual." << endl;
}

VirtualVideo::~VirtualVideo()
{
	set_updateScreen(false);

	/* Signal to update screen thread to exit. */
	int ret = 0;
	m_KeepRunning = false;
	SDL_WaitThread(m_Updater_tid, &ret);

	/* Clear but not delete. */
	m_VisualElement_list.clear();
	m_UnderLayer_list.clear();

	m_RealVideo->rem_visualElement(m_VirtualScreen.get_Id());

	delete m_Screen;

	cout << "REMOVED VirtualVideo [" << m_Id << "] Thread ret(" << ret << ")" << endl;
}

/*************************************************************************************************/
void VirtualVideo::init(void)
{
	/* Create layer surface.*/
	m_VirtualScreen.set_viewpoint(Viewpoints::create_surface(m_Screen_size.x, m_Screen_size.y));
	if(m_VirtualScreen.get_viewpoint() == NULL) {
		cout << "Failed to create surface for Virtual Video." << endl;
		assert(0);
	}

	m_Screen = Viewpoints::create_surface(m_Screen_size.x, m_Screen_size.y);

	m_RealVideo->push_under_layer(&m_VirtualScreen);

	m_Updater_tid = SDL_CreateThread(&video_thread_wrapper, this);

	start();

	cout << "INITIALIZED Virtual Video [" << get_Id() << "]" << endl;
}

/*************************************************************************************************/
int VirtualVideo::virtual_video_thread(void)
{

	while(m_KeepRunning) {

		if (get_updateScreen() == true) {
			SDL_LockMutex(m_UnderLayer_list_lock);
			Viewpoints::draw_visual_list(m_UnderLayer_list, m_Screen);
			SDL_UnlockMutex(m_UnderLayer_list_lock);
			SDL_Flip(m_Screen);

			SDL_LockMutex(m_VisualElement_list_lock);
			Viewpoints::draw_visual_list(m_VisualElement_list, m_Screen);
			SDL_UnlockMutex(m_VisualElement_list_lock);

			assert(m_VirtualScreen.update_viewpoint(m_Screen) == 0);
		}

		usleep(m_UpdateInterval_ms*1000);
	}

	return 0;
}

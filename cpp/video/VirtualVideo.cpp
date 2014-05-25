/*
 * VirtualVideo.cpp
 *
 *  Created on: 23/05/2014
 *      Author: vitor
 */

#include "VirtualVideo.h"

#include <iostream>
#include <unistd.h>

#include "Viewpoints.h"


using namespace std;

unsigned int VirtualVideo::s_VirtualVideo_ids;

VirtualVideo::VirtualVideo(Video *real_screen,
		const unsigned int& screen_width,
		const unsigned int& screen_height,
		const unsigned int& update_interval):
m_RealVideo(real_screen),
m_UpdateInterval_ms(update_interval),
m_UpdateScreen_f(true),
m_KeepRunning(true),
m_Id(generate_id())
{
	cout << "CREATED Virtual Video [" << get_Id() << "]" << endl;

	/* Create layer surface.*/
	m_VirtualScreen.set_viewpoint(Viewpoints::create_surface(screen_width, screen_height), 0);
	if(m_VirtualScreen.get_viewpoint() == NULL) {
		cout << "Failed to set SDL Video Mode." << endl;
	}

	// This is the problem.
	m_RealVideo->push_under_layer(&m_VirtualScreen);

	m_UpdateScreen_f_lock = SDL_CreateMutex();
	m_Updater_tid = SDL_CreateThread(&video_thread_wrapper, this);
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

	SDL_DestroyMutex(m_UpdateScreen_f_lock);
	cout << "REMOVED VirtualVideo [" << m_Id << "] Thread ret(" << ret << ")" << endl;

//	unsigned int i;
//	unsigned int vec_size = m_VisualElement_list.size();
//	for (i = 0; i < vec_size; i++) {
//		VisualElement *elem = m_VisualElement_list[i];
//		delete elem;
//	}
//	m_VisualElement_list.clear();
//
//	vec_size = m_UnderLayer_list.size();
//	for (i = 0; i < vec_size; i++) {
//		VisualElement *elem = m_UnderLayer_list[i];
//		delete elem;
//	}
//	m_UnderLayer_list.clear();
//
//	cout << "[" << m_Id << "] Will remove UnderLayer " << m_VirtualScreen.get_Id() << endl;
//	m_RealVideo->rem_visualElement(m_VirtualScreen.get_Id());
}

/*************************************************************************************************/
void VirtualVideo::set_updateScreen(bool value)
{

	SDL_LockMutex(m_UpdateScreen_f_lock);
	m_UpdateScreen_f = value;
	SDL_UnlockMutex(m_UpdateScreen_f_lock);
}

/*************************************************************************************************/
bool VirtualVideo::get_updateScreen(void)
{
	bool update_screen;

	SDL_LockMutex(m_UpdateScreen_f_lock);
	update_screen = m_UpdateScreen_f;
	SDL_UnlockMutex(m_UpdateScreen_f_lock);

	return update_screen;
}

/*************************************************************************************************/
int VirtualVideo::virtual_video_thread(void)
{
	while(m_KeepRunning) {

		if (get_updateScreen() == true) {
			Viewpoints::draw_visual_list(m_UnderLayer_list, m_VirtualScreen.m_Viewpoints[0]);
			Viewpoints::draw_visual_list(m_VisualElement_list, m_VirtualScreen.m_Viewpoints[0]);
			//SDL_Flip(m_Screen);
		}

		usleep(m_UpdateInterval_ms*1000);
	}

	return 0;
}

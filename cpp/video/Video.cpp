#include "Video.h"
#include <iostream>
#include <assert.h>
#include <unistd.h>
#include <pthread.h>

/*************************************************************************************************/

	Video::Video(const int update_interval, string title):
	m_UpdateInterval_ms(update_interval),
	m_UpdateScreen_f(false),
	m_Title(title)
	{
		m_Screen = SDL_SetVideoMode(VIDEO_SCREEN_WIDTH, VIDEO_SCREEN_HEIGHT, VIDEO_SCREEN_BPP,
				VIDEO_SCREEN_FLAGS);
		if(m_Screen == NULL) {
			cout << "Failed to set SDL Video Mode." << endl;
		}

		SDL_WM_SetCaption(m_Title.c_str(), NULL);

		m_UpdateScreen_f_lock = SDL_CreateMutex();
		m_Updater_tid = SDL_CreateThread(&video_thread_wrapper, this);
		cout << "Thread launched!! " << m_Updater_tid << endl;
	}

/*************************************************************************************************/
	Video::~Video()
	{
		SDL_DestroyMutex(m_UpdateScreen_f_lock);
		cout << "Video destroyed." << endl;
	}


/*************************************************************************************************/
	void Video::add_visualElement(VisualElement *element)
	{
		assert(element != NULL);
		m_VisualElement_list.push_back(element);
	}

/*************************************************************************************************/
	int Video::push_under_layer(SDL_Surface *layer)
	{
		if (m_UnderLayer_list.size() == UNDERLAYER_MAX_SIZE) {
			return -1;
		}
		m_UnderLayer_list.push_back(layer);

		return 0;
	}

/*************************************************************************************************/
	void Video::pop_under_layer(void)
	{
		m_UnderLayer_list.pop_back();
	}

/*************************************************************************************************/
	void Video::start(void)
	{
		set_updateScreen(true);
	}

/*************************************************************************************************/
	void Video::freeze(void)
	{
		set_updateScreen(false);
	}

/*************************************************************************************************/
	void Video::set_updateScreen(bool value)
	{

		SDL_LockMutex(m_UpdateScreen_f_lock);
		m_UpdateScreen_f = value;
		SDL_UnlockMutex(m_UpdateScreen_f_lock);
	}

/*************************************************************************************************/
	bool Video::get_updateScreen(void)
	{
		bool update_screen;

		SDL_LockMutex(m_UpdateScreen_f_lock);
		update_screen = m_UpdateScreen_f;
		SDL_UnlockMutex(m_UpdateScreen_f_lock);

		return update_screen;
	}

/*************************************************************************************************/
	void Video::draw_underLayer(void)
	{
		// verificar offset. layers as visualElements instead of Surfaces.
		for (vector<SDL_Surface *>::iterator iter = m_UnderLayer_list.begin();
				iter != m_UnderLayer_list.end(); ++iter) {
				SDL_BlitSurface(*iter, NULL, m_Screen, NULL);
		}
	}

/*************************************************************************************************/
	void Video::draw_visualElements(void)
	{
		for (vector<VisualElement *>::iterator iter = m_VisualElement_list.begin();
				iter != m_VisualElement_list.end(); ++iter) {
				SDL_Rect element_offset = (*iter)->get_offset();
				SDL_BlitSurface((*iter)->get_viewpoint(), NULL, m_Screen, &element_offset);
		}
	}


/*************************************************************************************************/
	int Video::video_thread(void)
	{
		while(true) {
			if (get_updateScreen() == true) {
				draw_underLayer();
				draw_visualElements();
				SDL_Flip(m_Screen);
			}

			usleep(m_UpdateInterval_ms*1000);
		}

		return 0;
	}


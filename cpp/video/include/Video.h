#ifndef VIDEO_H
#define VIDEO_H

#include <string>
#include <vector>
#include "SDL/SDL.h"
#include "SDL/SDL_thread.h"

#include "VisualElement.h"

using namespace std;

#define VIDEO_SCREEN_WIDTH 640
#define VIDEO_SCREEN_HEIGHT 480
#define VIDEO_SCREEN_BPP 32
#define VIDEO_SCREEN_FLAGS SDL_SWSURFACE|SDL_DOUBLEBUF
#define VIDEO_SCREEN_TITLE (string)"My Game"
#define UNDERLAYER_MAX_SIZE 5

class Video {
private:
	vector <VisualElement *> m_VisualElement_list;
	vector <SDL_Surface *> m_UnderLayer_list;
	SDL_Surface *m_Screen;		//!< Represents the monitor screen.
	int m_UpdateInterval_ms;	//!< Update interval in mili seconds.
	bool m_UpdateScreen_f;		//!< True: update the screen; False: hold until is true.
	string m_Title;
	SDL_Thread *m_Updater_tid;

	SDL_mutex *m_UpdateScreen_f_lock;

public:

	Video(const int update_interval = 20, string title=VIDEO_SCREEN_TITLE);
	~Video(void);

	void add_visualElement(VisualElement *element);

	int push_under_layer(SDL_Surface *layer);
	void pop_under_layer(void);

	void start(void);
	void freeze(void);


	static int video_thread_wrapper(void* pParam)
	{
		return ((Video*)pParam)->video_thread();
	}


private:
	int video_thread(void);
	void draw_underLayer(void);
	void draw_visualElements(void);

	bool get_updateScreen(void);
	void set_updateScreen(bool value);

};

#endif /* VIDEO_H */

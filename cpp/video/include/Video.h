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
#define SCREEN_UPDATE_DEFAULT_TIME_MS 20
#define VIDEO_SCREEN_BPP 32
#define VIDEO_SCREEN_FLAGS SDL_SWSURFACE|SDL_DOUBLEBUF
#define VIDEO_SCREEN_TITLE (string)"My Game"
#define UNDERLAYER_MAX_SIZE 5

typedef enum st_virtual_screen {
	VIDEO_REAL_SCREEN = 0,
	VIDEO_VIRTUAL_SCREEN = 1
} st_screen_mode;

class Video {
private:
	vector <VisualElement *> m_VisualElement_list;
	vector <SDL_Surface *> m_UnderLayer_list;
	SDL_Surface *m_Screen;		//!< Represents the monitor screen.
	Video *m_RealVideo;		//!< Represents the real monitor screen when initialized as virtual screen.
	st_screen_mode m_ScreenMode;//!< Tells if the screen is "real" or "virtual" (read the constructors for more info).
	int m_ScreeWidth;			//!< Screen width in pixels.
	int m_ScreeHeight;			//!< Screen height in pixels.
	int m_UpdateInterval_ms;	//!< Update interval in mili seconds.
	bool m_UpdateScreen_f;		//!< True: update the screen; False: hold until is true.
	string m_Title;				//!< Screen caption.
	SDL_Thread *m_Updater_tid;

	SDL_mutex *m_UpdateScreen_f_lock;

public:

	/*
	 * \brief Class constructor for real screen. Real screen will be the SDL screen representation.
	 */
	Video(const unsigned int& screen_width = VIDEO_SCREEN_WIDTH,
			const unsigned int& screen_height = VIDEO_SCREEN_HEIGHT,
			const string& title=VIDEO_SCREEN_TITLE,
			const unsigned int& update_interval = SCREEN_UPDATE_DEFAULT_TIME_MS);

	/*
	 * \brief Class constructor for a virtual screen. Virtual screen will be the concept given to surfaces
	 * that will be drawn inside the real screen and will represent sub-screens.
	 */
	Video(st_virtual_screen is_virtual,
			Video *real_screen,
			const unsigned int& screen_width = VIDEO_SCREEN_WIDTH,
			const unsigned int& screen_height = VIDEO_SCREEN_HEIGHT,
			const unsigned int& update_interval = SCREEN_UPDATE_DEFAULT_TIME_MS);

	/*
	 * \brief Class destructor.
	 */
	~Video(void);

	void add_visualElement(VisualElement *element);

	int push_under_layer(SDL_Surface *layer);
	void pop_under_layer(void);

	/*
	 * \brief Start updating the screen with layers and visual elements.
	 */
	void start(void);

	/*
	 * \brief Stop updating the screen.
	 */
	void freeze(void);

	/*
	 * \brief Set screen caption.
	 */
	void set_caption(const string& caption);

	/*
	 * \brief Change the screen update interval.
	 * \note THIS OPERATION WILL FREEZE THE SCREEN WHILE UPDATING THE VALUE.
	 */
	void set_update_interval(const int& interval);

	/*
	 * \brief Thread wrapper. Probably there is an easier way to do this.
	 */
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

#ifndef VIDEO_H
#define VIDEO_H

#include <iostream>
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


class Video {
protected:
	static uint32_t s_Video_ids;
	vector <VisualElement *> m_VisualElement_list;
	vector <VisualElement *> m_UnderLayer_list;
	SDL_Surface *m_Screen;		//!< Represents the monitor screen.
	string m_Caption;			//!< Window title
	SDL_Rect m_Screen_size;		//!< Screen size in pixels.
	int m_UpdateInterval_ms;	//!< Update interval in mili seconds.
	uint32_t m_Id;				//!< Video unique identifier.
	bool m_UpdateScreen_f;		//!< True: update the screen; False: hold until is true.
	bool m_KeepRunning;			//!< Flag to halt the update screen thread.

	SDL_Thread *m_Updater_tid;
	SDL_mutex *m_UpdateScreen_f_lock;
	SDL_mutex *m_VisualElement_list_lock;
	SDL_mutex *m_UnderLayer_list_lock;

public:

	/*
	 * \brief Class constructor for real screen. Real screen will be the SDL screen representation.
	 */
	Video(const uint32_t& screen_width = VIDEO_SCREEN_WIDTH,
			const uint32_t& screen_height = VIDEO_SCREEN_HEIGHT,
			const string& title=VIDEO_SCREEN_TITLE,
			const uint32_t& update_interval = SCREEN_UPDATE_DEFAULT_TIME_MS);

	/*
	 * \brief Class destructor.
	 */
	~Video(void);

	void add_visualElement(VisualElement *element);

	/*
	 * \brief Removes a visual element from the list.
	 * \param id The visual element unique ID.
	 * \return 0 if found; -1 if the visual element was not found in the list.
	 */
	int rem_visualElement(const uint32_t& id);

	/*
	 * \brief Removes all the visual elements.
	 */
	void rem_visualElement_all(void);

	int push_under_layer(VisualElement *layer);
	void pop_under_layer(void);

	/*
	 * \brief Initialize the screen. Must be called before starting.
	 * \param mode The screen execution mode.
	 */
	void init(void);

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

	/*
	 * \brief Get Video identifier.
	 * \return The Video identifier.
	 */
	inline uint32_t get_Id(void)
	{
		return m_Id;
	}

protected:
	int video_thread(void);

	/*
	 * \brief Removes a visual element from the visualElement's list.
	 * \param element The element to be removed.
	 */
	void rem_visualElement_gen(VisualElement *element);

	/*
	 * \brief Removes a visual element from the underLayer's list.
	 * \param element The element to be removed.
	 */
	void rem_underLayer_gen(VisualElement *element);

	bool get_updateScreen(void);
	void set_updateScreen(bool value);

	/*
	 * \brief Get an unique Video ID.
	 * \return An unique ID.
	 */
	static uint32_t generate_id(void)
	{
		return s_Video_ids++;
	}
};

#endif /* VIDEO_H */

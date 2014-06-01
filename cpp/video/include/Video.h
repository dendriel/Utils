#ifndef VIDEO_H
#define VIDEO_H

#include <iostream>
#include <string>
#include <vector>
#include <tr1/cstdint>

#include "SDL/SDL.h"
#include "SDL/SDL_ttf.h"
#include "SDL/SDL_thread.h"

#include "VisualElement.h"
#include "Message.h"

#define VIDEO_SCREEN_WIDTH 640
#define VIDEO_SCREEN_HEIGHT 480
#define SCREEN_UPDATE_DEFAULT_TIME_MS 20
#define VIDEO_SCREEN_BPP 32
#define VIDEO_SCREEN_FLAGS SDL_SWSURFACE|SDL_DOUBLEBUF
#define VIDEO_SCREEN_TITLE (std::string)"My Game"
#define UNDERLAYER_MAX_SIZE 5


class Video {
protected:
	static uint32_t s_Video_ids;
	std::vector <VisualElement *> m_VisualElement_list;	//!< A list of VisualElements to be displayed in the screen.
	std::vector <VisualElement *> m_UnderLayer_list;	//!< A list of VisualElements that will be printed in the screen before any others.
	std::vector <VisualElement *> m_Message_list;				//!< A list of messages that will be being displayed in the screen(after everything).
	SDL_Surface *m_Screen;		//!< Represents the monitor screen.
	std::string m_Caption;			//!< Window title
	SDL_Rect m_Screen_size;		//!< Screen size in pixels. *Also keeps the offset while dealing with virtual screens.
	int m_UpdateInterval_ms;	//!< Update interval in mili seconds.
	TTF_Font *m_Font;
	uint32_t m_Id;				//!< Video unique identifier.
	bool m_UpdateScreen_f;		//!< True: update the screen; False: hold until is true.
	bool m_KeepRunning;			//!< Flag to halt the update screen thread.

	SDL_Thread *m_Updater_tid;
	SDL_mutex *m_UpdateScreen_f_lock;
	SDL_mutex *m_VisualElement_list_lock;
	SDL_mutex *m_UnderLayer_list_lock;
	SDL_mutex *m_Message_list_Lock;

public:

	/**
	 * \brief Class constructor for real screen. Real screen will be the SDL screen representation.
	 */
	Video(const uint16_t& width = VIDEO_SCREEN_WIDTH,
			const uint16_t& height = VIDEO_SCREEN_HEIGHT,
			const std::string& title=VIDEO_SCREEN_TITLE,
			const uint32_t& update_interval = SCREEN_UPDATE_DEFAULT_TIME_MS,
			const int16_t& x=0,
			const int16_t& y=0);

	/**
	 * \brief Class destructor.
	 */
	~Video(void);

	/**
	 * \brief Adds a VisualElement to the visual element's list.
	 * \param layer The VisualElement to be added.
	 */
	void add_visualElement(VisualElement *element);

	/**
	 * \brief Removes a visual element from the any of the visual element's list (includes the under layer's list).
	 * \param id The visual element unique ID.
	 * \return 0 if found; -1 if the visual element was not found in the list.
	 */
	int rem_visualElement(const uint32_t& id);

	/**
	 * \brief Erase a text from the screen (also will DELETE its content).
	 */
	void erase(const uint32_t msgid);

	/**
	 * \brief Removes and FREE (delete) all the visual elements from the list.
	 */
	void remf_visualElement_all(void);
	void remf_underLayer_all(void);
	void remf_message_all(void);

	/**
	 * \brief Find a visual element in every list by its ID.
	 * \param id Unique ID of the visual element.
	 * \return The visual element if fount in any list. NULL if not found.
	 */
	VisualElement *get_visualElement(const uint32_t id);

	/**
	 * \brief Adds a VisualElement to the under layer's list.
	 * \param layer The VisualElement to be added.
	 */
	int push_under_layer(VisualElement *layer);

	/**
	 * \brief Removes the lastest VisualElement added in the under layer's list.
	 */
	void pop_under_layer(void);

	/**
	 * \brief Initialize the screen.
	 * \param auto_start Informs if automatically start updating the screen after initializing.
	 */
	void init(bool auto_start=true);

	/**
	 * \brief Start updating the screen with layers and visual elements.
	 */
	void start(void);

	/**
	 * \brief Stop updating the screen.
	 */
	void freeze(void);

	/***
	 * \brief Set screen caption.
	 */
	void set_caption(const std::string& caption);

	/**
	 * \brief Change the screen update interval.
	 * \note THIS OPERATION WILL FREEZE THE SCREEN WHILE UPDATING THE VALUE.
	 */
	void set_update_interval(const int& interval);

	/**
	 * \brief Paint the base screen surface with the given color (just one time).
	 * \note If a huge visualElement is being displayed at the screen, maybe it will
	 * cover the base surface and prevent from seeing its color.
	 */
	void fill_with_color(const uint8_t r, const uint8_t g, const uint8_t b);

	/**
	 * \brief Loads a font to write.
	 * \param font The path to the font to be loaded
	 */
	void load_font(std::string font, const int32_t size=28);

	/**
	 * \brief Write a text in the screen.
	 * \param content The message to be displayed.
	 * \param color The color of the message.
	 * \param x Horizontal position of the message in the screen.
	 * \param y Vertical position of the message in the screen.
	 * \return An unique ID that to identify the message.
	 */
	unsigned int write(std::string content, SDL_Color color, int16_t x=0, int16_t y=0);

	/**
	 * \brief Thread wrapper. Probably there is an easier way to do this.
	 */
	static int video_thread_wrapper(void* pParam)
	{
		return ((Video*)pParam)->video_thread();
	}

	/**
	 * \brief Get Video identifier.
	 * \return The Video identifier.
	 */
	inline uint32_t get_Id(void)
	{
		return m_Id;
	}

protected:
	int video_thread(void);

	/**
	 * \brief Removes a visual element from the visualElement's list.
	 * \param element The element to be removed.
	 */
	void rem_visualElement_gen(VisualElement *element);

	/**
	 * \brief Removes a visual element from the underLayer's list.
	 * \param element The element to be removed.
	 */
	void rem_underLayer_gen(VisualElement *element);

	/**
	 * \brief Removes a visual element from the message's list.
	 * \param element The element to be removed.
	 */
	void rem_message_gen(VisualElement *element);

	bool get_updateScreen(void);
	void set_updateScreen(bool value);

	/**
	 * \brief Get an unique Video ID.
	 * \return An unique ID.
	 */
	static uint32_t generate_id(void)
	{
		return s_Video_ids++;
	}
};

#endif /* VIDEO_H */

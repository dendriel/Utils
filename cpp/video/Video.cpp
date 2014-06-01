#include "Video.h"

#include <algorithm>
#include <assert.h>
#include <unistd.h>

#include "Viewpoints.h"

#include "debug.h"

uint32_t Video::s_Video_ids;

using namespace std;

Video::Video(const uint16_t& width, const uint16_t& height, const string& title, const uint32_t& update_interval, const int16_t& x, const int16_t& y):
m_Caption(title),
m_UpdateInterval_ms(update_interval),
m_Font(NULL),
m_Id(Video::generate_id()),
m_UpdateScreen_f(false),
m_KeepRunning(true)
{
	m_Screen_size.w = width;
	m_Screen_size.h = height;
	m_Screen_size.x = x;
	m_Screen_size.y = y;

	m_UpdateScreen_f_lock = SDL_CreateMutex();
	m_VisualElement_list_lock = SDL_CreateMutex();
	m_UnderLayer_list_lock = SDL_CreateMutex();
	m_Message_list_Lock = SDL_CreateMutex();


	debug("Video [" << m_Id << "] created.");
}

/*************************************************************************************************/
Video::~Video()
{
	set_updateScreen(false);

	/* Signal to update screen thread to exit. */
	int ret = 0;
	m_KeepRunning = false;
	SDL_WaitThread(m_Updater_tid, &ret);

	/* Clear but not delete. (i believe that this means nothing in this point. But serves to keep
	 * this guys at sight.
	 */
	m_VisualElement_list.clear();
	m_UnderLayer_list.clear();
	m_Message_list.clear();

	delete m_Screen;

	SDL_DestroyMutex(m_UpdateScreen_f_lock);
	SDL_DestroyMutex(m_VisualElement_list_lock);
	SDL_DestroyMutex(m_UnderLayer_list_lock);
	SDL_DestroyMutex(m_Message_list_Lock);

	if (m_Font != NULL) {
		TTF_CloseFont(m_Font);
	}
	TTF_Quit();

	debug("Video [" << m_Id << "] Thread ret(" << ret << ") destroyed.");
}

/*************************************************************************************************/
void Video::init(const bool auto_start)
{
	/* Initialize video. */
	m_Screen = SDL_SetVideoMode(m_Screen_size.w, m_Screen_size.h, VIDEO_SCREEN_BPP, VIDEO_SCREEN_FLAGS);
	if(m_Screen == NULL) {
		cout << "Failed to set SDL Video Mode." << endl;
		assert(0);
	}
	SDL_WM_SetCaption(m_Caption.c_str(), NULL);

	m_Updater_tid = SDL_CreateThread(&video_thread_wrapper, this);

	/* Initialize fonts. */
	assert(TTF_Init() == 0);

	if (auto_start) {
		start();
	}

	debug("Real Video [" << m_Id << "] initialized.");
}

/*************************************************************************************************/
void Video::start(void)
{
	set_updateScreen(true);
	debug("Video [" << m_Id << "] started.");
}

/*************************************************************************************************/
void Video::freeze(void)
{
	set_updateScreen(false);
	debug("Real Video [" << m_Id << "] freeze.");
}

/*************************************************************************************************/
void Video::add_visualElement(VisualElement *element)
{
	assert(element != NULL);

	SDL_LockMutex(m_VisualElement_list_lock);
	m_VisualElement_list.push_back(element);
	SDL_UnlockMutex(m_VisualElement_list_lock);

	debug("Real Video [" << m_Id << "] - Added new visual element (" << element->get_Id() << ")");
}

/*************************************************************************************************/
void Video::rem_visualElement_gen(VisualElement *element)
{
	SDL_LockMutex(m_VisualElement_list_lock);
	m_VisualElement_list.erase(std::remove(m_VisualElement_list.begin(), m_VisualElement_list.end(), element), m_VisualElement_list.end());
	SDL_UnlockMutex(m_VisualElement_list_lock);

	debug("Real Video [" << m_Id << "] - Removed visual element (" << element->get_Id() << ")");
}

void Video::rem_underLayer_gen(VisualElement *element)
{
	SDL_LockMutex(m_UnderLayer_list_lock);
	m_UnderLayer_list.erase(std::remove(m_UnderLayer_list.begin(), m_UnderLayer_list.end(), element), m_UnderLayer_list.end());
	SDL_UnlockMutex(m_UnderLayer_list_lock);

	debug("Real Video [" << m_Id << "] - Removed under layer (" << element->get_Id() << ")");
}

void Video::rem_message_gen(VisualElement *element)
{
	SDL_LockMutex(m_Message_list_Lock);
	m_Message_list.erase(std::remove(m_Message_list.begin(), m_Message_list.end(), element), m_Message_list.end());
	SDL_UnlockMutex(m_Message_list_Lock);

	debug("Real Video [" << m_Id << "] - Removed message (" << element->get_Id() << ")");
}

int Video::rem_visualElement(const uint32_t& id)
{
	VisualElement *elem = NULL;

	for (vector<VisualElement *>::iterator iter = m_VisualElement_list.begin();
			iter != m_VisualElement_list.end(); ++iter) {
		elem = (*iter);

		if (elem->get_Id() == id) {
			rem_visualElement_gen(elem);
			return 0;
		}
	}

	for (vector<VisualElement *>::iterator iter = m_UnderLayer_list.begin();
			iter != m_UnderLayer_list.end(); ++iter) {
		elem = (*iter);

		if (elem->get_Id() == id) {
			rem_underLayer_gen(elem);
			return 0;
		}
	}

	for (vector<VisualElement *>::iterator iter = m_Message_list.begin();
			iter != m_Message_list.end(); ++iter) {
		elem = (*iter);

		if (elem->get_Id() == id) {
			rem_message_gen(elem);
			return 0;
		}
	}

	debug("Real Video [" << m_Id << "] - No element found to be removed.");

	return -1;
}

void Video::erase(const uint32_t msgid)
{
	(void) rem_visualElement(msgid);

}

void Video::remf_visualElement_all(void)
{
	SDL_LockMutex(m_VisualElement_list_lock);
	/* Interact over all the elements. */
	for (vector<VisualElement *>::iterator iter = m_VisualElement_list.begin();
			iter != m_VisualElement_list.end(); ++iter) {
		VisualElement *elem = *iter;

		/* Delete data, but keep the reference, so we don't mess the list.*/
		delete elem;
	}

	/* Clear all the references (that now are invalid). */
	m_VisualElement_list.clear();
	SDL_UnlockMutex(m_VisualElement_list_lock);
}

void Video::remf_underLayer_all(void)
{
	SDL_LockMutex(m_UnderLayer_list_lock);
	/* Interact over all the elements. */
	for (vector<VisualElement *>::iterator iter = m_UnderLayer_list.begin();
			iter != m_VisualElement_list.end(); ++iter) {
		VisualElement *elem = *iter;

		/* Delete data, but keep the reference, so we don't mess the list.*/
		delete elem;
	}

	/* Clear all the references (that now are invalid). */
	m_UnderLayer_list.clear();
	SDL_UnlockMutex(m_UnderLayer_list_lock);
}

void Video::remf_message_all(void)
{
	VisualElement *elem = NULL;
	SDL_LockMutex(m_Message_list_Lock);
	/* Interact over all the elements. */
	for (vector<VisualElement *>::iterator iter = m_Message_list.begin();
			iter != m_Message_list.end(); ++iter, elem = (*iter)) {

		/* Delete data, but keep the reference, so we don't mess the list.*/
		delete elem;
	}

	/* Clear all the references (that now are invalid). */
	m_Message_list.clear();
	SDL_UnlockMutex(m_Message_list_Lock);
}

/*************************************************************************************************/

VisualElement *Video::get_visualElement(const uint32_t id)
{
	VisualElement *elem = NULL;

	for (vector<VisualElement *>::iterator iter = m_VisualElement_list.begin();
			iter != m_VisualElement_list.end(); ++iter) {
		elem = (*iter);

		if (elem->get_Id() == id) {
			return elem;
		}
	}

	for (vector<VisualElement *>::iterator iter = m_UnderLayer_list.begin();
			iter != m_UnderLayer_list.end(); ++iter) {
		elem = (*iter);

		if (elem->get_Id() == id) {
			return elem;
		}
	}

	for (vector<VisualElement *>::iterator iter = m_Message_list.begin();
			iter != m_Message_list.end(); ++iter) {
		elem = (*iter);

		if (elem->get_Id() == id) {
			return elem;
		}
	}

	return NULL;
}

/*************************************************************************************************/
int Video::push_under_layer(VisualElement *layer)
{
	if (m_UnderLayer_list.size() == UNDERLAYER_MAX_SIZE) {
		debug("Real Video [" << m_Id << "] - Under layer's list is full!");
		return -1;
	}

	SDL_LockMutex(m_UnderLayer_list_lock);
	m_UnderLayer_list.push_back(layer);
	SDL_UnlockMutex(m_UnderLayer_list_lock);

	debug("Real Video [" << m_Id << "] - Added new under layer (" << layer->get_Id() << ")");

	return 0;
}

/*************************************************************************************************/
void Video::pop_under_layer(void)
{
	SDL_LockMutex(m_UnderLayer_list_lock);
	m_UnderLayer_list.pop_back();
	SDL_UnlockMutex(m_UnderLayer_list_lock);
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

void Video::set_caption(const string& caption)
{
	SDL_WM_SetCaption(caption.c_str(), NULL);
}

/*************************************************************************************************/

void Video::set_update_interval(const int& interval)
{
	set_updateScreen(false);
	m_UpdateInterval_ms = interval;
	set_updateScreen(true);
}

/*************************************************************************************************/

void Video::fill_with_color(const uint8_t r, const uint8_t g, const uint8_t b)
{
	Viewpoints::paint_surface(m_Screen, r, g, b);
}

/*************************************************************************************************/

void Video::load_font(string font, const int32_t size)
{
	/* Free past font before loading if there is one. */
	if (m_Font != NULL) {
		TTF_CloseFont(m_Font);
	}

	m_Font = TTF_OpenFont(font.c_str(), size);

	if (m_Font == NULL) {
		assert_exit("Failed to load the font from \"" << font << "\"");
	}

	debug("Real Video [" << m_Id << "] - Loaded new font \"" << font << "\"");
}

/*************************************************************************************************/

unsigned int Video::write(string content, SDL_Color color, int16_t x, int16_t y)
{
	if (m_Font == NULL) {
		assert_exit("Error. Trying to write before loading any font!");
	}

	Message *message = new Message(content, color, m_Font, x, y);

	SDL_LockMutex(m_Message_list_Lock);
	m_Message_list.push_back(message);
	SDL_UnlockMutex(m_Message_list_Lock);

	debug("Real Video [" << m_Id << "] - Added new message \"" << content << "\"");

	return message->get_Id();
}

/*************************************************************************************************/
int Video::video_thread(void)
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

			SDL_Flip(m_Screen);
		}

		usleep(m_UpdateInterval_ms*1000);
	}

	return 0;
}


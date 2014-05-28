#include "Video.h"

#include <algorithm>
#include <assert.h>
#include <unistd.h>

#include "Viewpoints.h"

#include "debug.h"

uint32_t Video::s_Video_ids;


Video::Video(const uint16_t& width, const uint16_t& height, const string& title, const uint32_t& update_interval, const int16_t& x, const int16_t& y):
m_Caption(title),
m_UpdateInterval_ms(update_interval),
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

	/* Clear but not delete. */
	m_VisualElement_list.clear();
	m_UnderLayer_list.clear();

	delete m_Screen;

	SDL_DestroyMutex(m_UpdateScreen_f_lock);
	SDL_DestroyMutex(m_VisualElement_list_lock);
	SDL_DestroyMutex(m_UnderLayer_list_lock);

	debug("Video [" << m_Id << "] Thread ret(" << ret << ") destroyed.");
}

/*************************************************************************************************/
void Video::init(const bool auto_start)
{
	m_Screen = SDL_SetVideoMode(m_Screen_size.w, m_Screen_size.h, VIDEO_SCREEN_BPP, VIDEO_SCREEN_FLAGS);
	if(m_Screen == NULL) {
		cout << "Failed to set SDL Video Mode." << endl;
		assert(0);
	}
	SDL_WM_SetCaption(m_Caption.c_str(), NULL);

	m_Updater_tid = SDL_CreateThread(&video_thread_wrapper, this);

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

int Video::rem_visualElement(const uint32_t& id)
{
	for (vector<VisualElement *>::iterator iter = m_VisualElement_list.begin();
			iter != m_VisualElement_list.end(); ++iter) {
		VisualElement *elem = *iter;

		if (elem->get_Id() == id) {
			rem_visualElement_gen(elem);
			return 0;
		}
	}

	for (vector<VisualElement *>::iterator iter = m_UnderLayer_list.begin();
			iter != m_UnderLayer_list.end(); ++iter) {
		VisualElement *elem = *iter;

		if (elem->get_Id() == id) {
			rem_underLayer_gen(elem);
			return 0;
		}
	}

	debug("Real Video [" << m_Id << "] - No element found to be removed.");

	return -1;
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

			SDL_Flip(m_Screen);
		}

		usleep(m_UpdateInterval_ms*1000);
	}

	return 0;
}


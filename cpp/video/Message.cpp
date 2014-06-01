/*
 * Message.cpp
 *
 *  Created on: 31/05/2014
 *      Author: vitor
 */

#include "Message.h"

#include "debug.h"

using namespace std;

Message::Message(string content, SDL_Color color, TTF_Font *font, int16_t x, int16_t y):
VisualElement(x, y),
m_Content(content),
m_Color(color)
{
	m_Offset.x = x;
	m_Offset.y = y;

	SDL_Surface *message =  TTF_RenderText_Solid(font, content.c_str(), color);
	if (message == NULL) {
		assert_exit("Failed to render text \"" << content << "\"");
	}

	this->set_viewpoint(message);

	/* Message ID is the same of its VisualElement. */
	debug("Message [" << this->get_Id() << "] created. [\"" << content << "\"]");
}

Message::~Message() {
	debug("Message [" << this->get_Id() << "] destroyed. [\"" << m_Content << "\"]");
}


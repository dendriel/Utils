/*
 * Message.h
 *
 *  Created on: 31/05/2014
 *      Author: vitor
 */

#ifndef MESSAGE_H_
#define MESSAGE_H_

#include <string>
#include <tr1/cstdint>

#include "SDL/SDL.h"
#include "SDL/SDL_ttf.h"

#include "VisualElement.h"

class Message: public VisualElement {
private:
	std::string m_Content;	//!< Content of the message.
	SDL_Color m_Color;		//!< Color of the message.
	SDL_Rect m_Offset;		//!< Position of the message.

public:
	/**
	 * \brief Class constructor.
	 */
	Message(std::string content, SDL_Color color, TTF_Font *font, int16_t x, int16_t y);

	/**
	 * \brief Class destructor.
	 */
	virtual ~Message();
};

#endif /* MESSAGE_H_ */

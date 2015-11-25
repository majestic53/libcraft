/**
 * libcraft
 * Copyright (C) 2015 David Jolly
 * ----------------------
 *
 * libcraft is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * libcraft is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "../include/craft.h"
#include "../include/craft_mouse_type.h"

namespace CRAFT {

	namespace COMPONENT {

		static const std::string MOUSE_BUTTON_STR[] = {
			STRING_UNKNOWN ,"L", "M", "R",
			};

		#define MOUSE_BUTTON_MAX SDL_BUTTON_RIGHT

		#define MOUSE_BUTTON_STRING(_TYPE_) \
			((_TYPE_) > MOUSE_BUTTON_MAX ? STRING_UNKNOWN : \
			STRING_CHECK(MOUSE_BUTTON_STR[_TYPE_]))

		_craft_mouse *_craft_mouse::m_instance = NULL;

		_craft_mouse::_craft_mouse(void) :
			m_double_click(false),
			m_initialized(false),
			m_parent(NULL),
			m_position({0, 0}),
			m_position_clicked({0, 0}),
			m_relative(false),
			m_scroll({0, 0}),
			m_state(0)
		{
			std::atexit(craft_mouse::_delete);
		}

		_craft_mouse::~_craft_mouse(void)
		{

			if(m_initialized) {
				uninitialize();
			}
		}

		void 
		_craft_mouse::_delete(void)
		{

			if(craft_mouse::m_instance) {
				delete craft_mouse::m_instance;
				craft_mouse::m_instance = NULL;
			}
		}

		_craft_mouse *
		_craft_mouse::acquire(void)
		{

			if(!craft_mouse::m_instance) {

				craft_mouse::m_instance = new craft_mouse;
				if(!craft_mouse::m_instance) {
					THROW_CRAFT_MOUSE_EXCEPTION(CRAFT_MOUSE_EXCEPTION_ALLOCATED);
				}
			}

			return craft_mouse::m_instance;
		}

		void 
		_craft_mouse::clear(void)
		{

			if(!m_initialized) {
				THROW_CRAFT_MOUSE_EXCEPTION(CRAFT_MOUSE_EXCEPTION_UNINITIALIZED);
			}

			m_double_click = false;
			m_position = glm::ivec2{0, 0};
			m_position_clicked = glm::ivec2{0, 0};
			m_relative = false;
			m_scroll = glm::ivec2{0, 0};
			m_state = 0;
		}

		void 
		_craft_mouse::initialize(
			__in SDL_Window *parent,
			__in_opt bool relative
			)
		{

			if(m_initialized) {
				THROW_CRAFT_MOUSE_EXCEPTION(CRAFT_MOUSE_EXCEPTION_INITIALIZED);
			}

			if(!parent) {
				THROW_CRAFT_MOUSE_EXCEPTION(CRAFT_MOUSE_EXCEPTION_PARENT);
			}

			if(SDL_SetRelativeMouseMode(relative ? SDL_TRUE : SDL_FALSE)) {
				THROW_CRAFT_MOUSE_EXCEPTION_FORMAT(CRAFT_MOUSE_EXCEPTION_EXTERNAL,
					"SDL_SetRelativeMouseMode failed: %s", SDL_GetError());
			}

			m_initialized = true;
			clear();
			m_parent = parent;
			m_relative = relative;
			SDL_SetWindowGrab(m_parent, SDL_TRUE);
			reset();
		}

		bool 
		_craft_mouse::is_allocated(void)
		{
			return (craft_mouse::m_instance != NULL);
		}

		bool 
		_craft_mouse::is_double_click(void)
		{
			bool result;

			if(!m_initialized) {
				THROW_CRAFT_MOUSE_EXCEPTION(CRAFT_MOUSE_EXCEPTION_UNINITIALIZED);
			}

			result = m_double_click;
			m_double_click = false;

			return result;
		}

		bool 
		_craft_mouse::is_initialized(void)
		{
			return m_initialized;
		}

		bool 
		_craft_mouse::is_left_click(void)
		{

			if(!m_initialized) {
				THROW_CRAFT_MOUSE_EXCEPTION(CRAFT_MOUSE_EXCEPTION_UNINITIALIZED);
			}

			return (m_state & SDL_BUTTON_LMASK);
		}

		bool 
		_craft_mouse::is_middle_click(void)
		{

			if(!m_initialized) {
				THROW_CRAFT_MOUSE_EXCEPTION(CRAFT_MOUSE_EXCEPTION_UNINITIALIZED);
			}

			return (m_state & SDL_BUTTON_MMASK);
		}

		bool 
		_craft_mouse::is_relative(void)
		{

			if(!m_initialized) {
				THROW_CRAFT_MOUSE_EXCEPTION(CRAFT_MOUSE_EXCEPTION_UNINITIALIZED);
			}

			return m_relative;
		}

		bool 
		_craft_mouse::is_right_click(void)
		{

			if(!m_initialized) {
				THROW_CRAFT_MOUSE_EXCEPTION(CRAFT_MOUSE_EXCEPTION_UNINITIALIZED);
			}

			return (m_state & SDL_BUTTON_RMASK);
		}

		bool 
		_craft_mouse::is_scrolling(void)
		{

			if(!m_initialized) {
				THROW_CRAFT_MOUSE_EXCEPTION(CRAFT_MOUSE_EXCEPTION_UNINITIALIZED);
			}

			return (m_scroll != glm::ivec2{0, 0});
		}

		void 
		_craft_mouse::on_event(
			__in const SDL_MouseButtonEvent &event
			)
		{

			if(!m_initialized) {
				THROW_CRAFT_MOUSE_EXCEPTION(CRAFT_MOUSE_EXCEPTION_UNINITIALIZED);
			}

			switch(event.button) {
				case SDL_BUTTON_X1:
					m_scroll.y = 1;
					break;
				case SDL_BUTTON_X2:
					m_scroll.y = -1;
					break;
				default:

					if(event.state == SDL_PRESSED) {
						m_state |= SDL_BUTTON(event.button);
					} else {
						m_state &= ~SDL_BUTTON(event.button);
					}

					m_double_click = (event.clicks == EVENT_DOUBLE_CLICK);
					m_position_clicked = {event.x, event.y};
					break;
			}
		}

		void 
		_craft_mouse::on_event(
			__in const SDL_MouseMotionEvent &event
			)
		{

			if(!m_initialized) {
				THROW_CRAFT_MOUSE_EXCEPTION(CRAFT_MOUSE_EXCEPTION_UNINITIALIZED);
			}

			if(m_relative) {
				m_position = {event.xrel, event.yrel};
			} else {
				m_position = {event.x, event.y};
			}
		}

		void 
		_craft_mouse::on_event(
			__in const SDL_MouseWheelEvent &event
			)
		{

			if(!m_initialized) {
				THROW_CRAFT_MOUSE_EXCEPTION(CRAFT_MOUSE_EXCEPTION_UNINITIALIZED);
			}

			m_scroll = {event.x, event.y};
		}

		glm::ivec2 
		_craft_mouse::position(void)
		{
			glm::ivec2 result;

			if(!m_initialized) {
				THROW_CRAFT_MOUSE_EXCEPTION(CRAFT_MOUSE_EXCEPTION_UNINITIALIZED);
			}

			result = m_position;
			m_position = {0, 0};

			return result;	
		}

		glm::ivec2 
		_craft_mouse::position_clicked(void)
		{
			glm::ivec2 result;

			if(!m_initialized) {
				THROW_CRAFT_MOUSE_EXCEPTION(CRAFT_MOUSE_EXCEPTION_UNINITIALIZED);
			}

			result = m_position_clicked;
			m_position_clicked = {0, 0};

			return result;
		}

		void 
		_craft_mouse::reset(void)
		{

			if(!m_initialized) {
				THROW_CRAFT_MOUSE_EXCEPTION(CRAFT_MOUSE_EXCEPTION_UNINITIALIZED);
			}

			set_position(MOUSE_DEFAULT_X, MOUSE_DEFAULT_Y);
		}

		glm::ivec2 
		_craft_mouse::scroll(void)
		{
			glm::ivec2 result;

			if(!m_initialized) {
				THROW_CRAFT_MOUSE_EXCEPTION(CRAFT_MOUSE_EXCEPTION_UNINITIALIZED);
			}

			result = m_scroll;
			m_scroll = {0, 0};

			return result;
		}

		void 
		_craft_mouse::set_position(
			__in int x,
			__in int y
			)
		{

			if(!m_initialized) {
				THROW_CRAFT_MOUSE_EXCEPTION(CRAFT_MOUSE_EXCEPTION_UNINITIALIZED);
			}

			SDL_WarpMouseInWindow(m_parent, x, y);
			update();
		}

		std::string 
		_craft_mouse::to_string(
			__in_opt bool verbose
			)
		{
			bool down = false;
			std::stringstream result;
			int iter = SDL_BUTTON_LEFT;

			result << CRAFT_MOUSE_HEADER << " (" << (m_initialized ? "INITIALIZED" : "UNINITIALIZED")
				<< ", " << (m_relative ? "RELATIVE" : "ABSOLUTE");

			if(verbose) {
				result << ", PTR. 0x" << SCALAR_AS_HEX(craft_mouse *, this);
			}

			if(m_initialized) {
				result << ", PAR. 0x" << SCALAR_AS_HEX(SDL_Window *, m_parent)
					<< ", POS. {" << m_position.x << ", " << m_position.y << "}"
					<< ", CLK. {" << m_position_clicked.x << ", " << m_position_clicked.y << "}, "
					<< (m_double_click ? "DOUBLE" : "SINGLE")
					<< ", ST. {";

				for(; iter <= MOUSE_BUTTON_MAX; ++iter) {

					if(m_state & SDL_BUTTON(iter)) {

						if(down) {
							result << "|";
						} else {
							down = true;
						}

						result << MOUSE_BUTTON_STRING(iter);
					}
				}

				result << "}, SCR. {" << m_scroll.x << ", " << m_scroll.y << "}";
			}

			result << ")";

			return result.str();
		}

		void 
		_craft_mouse::uninitialize(void)
		{

			if(!m_initialized) {
				THROW_CRAFT_MOUSE_EXCEPTION(CRAFT_MOUSE_EXCEPTION_UNINITIALIZED);
			}

			clear();
			SDL_SetWindowGrab(m_parent, SDL_FALSE);
			m_parent = NULL;
			m_initialized = false;
		}

		void 
		_craft_mouse::update(void)
		{

			if(!m_initialized) {
				THROW_CRAFT_MOUSE_EXCEPTION(CRAFT_MOUSE_EXCEPTION_UNINITIALIZED);
			}

			if(m_relative) {
				m_state = SDL_GetRelativeMouseState(&m_position.x, &m_position.y);
			} else {
				m_state = SDL_GetMouseState(&m_position.x, &m_position.y);
			}

			m_double_click = false;
			m_position_clicked = {0, 0};
			m_scroll = {0, 0};
		}
	}
}

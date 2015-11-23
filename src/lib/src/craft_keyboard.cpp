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
#include "../include/craft_keyboard_type.h"

namespace CRAFT {

	namespace COMPONENT {

		_craft_keyboard *_craft_keyboard::m_instance = NULL;

		_craft_keyboard::_craft_keyboard(void) :
			m_initialized(false)
		{
			std::atexit(craft_keyboard::_delete);
		}

		_craft_keyboard::~_craft_keyboard(void)
		{

			if(m_initialized) {
				uninitialize();
			}
		}

		void 
		_craft_keyboard::_delete(void)
		{

			if(craft_keyboard::m_instance) {
				delete craft_keyboard::m_instance;
				craft_keyboard::m_instance = NULL;
			}
		}

		_craft_keyboard *
		_craft_keyboard::acquire(void)
		{

			if(!craft_keyboard::m_instance) {

				craft_keyboard::m_instance = new craft_keyboard;
				if(!craft_keyboard::m_instance) {
					THROW_CRAFT_KEYBOARD_EXCEPTION(CRAFT_KEYBOARD_EXCEPTION_ALLOCATED);
				}
			}

			return craft_keyboard::m_instance;
		}

		void 
		_craft_keyboard::add(
			__in const SDL_Keycode &id
			)
		{
			const Uint8 *state = NULL;
			std::map<SDL_Keycode, bool>::iterator iter;

			if(!m_initialized) {
				THROW_CRAFT_KEYBOARD_EXCEPTION(CRAFT_KEYBOARD_EXCEPTION_UNINITIALIZED);
			}

			iter = m_key_map.find(id);
			if(iter == m_key_map.end()) {

				state = SDL_GetKeyboardState(NULL);
				if(!state) {
					THROW_CRAFT_KEYBOARD_EXCEPTION_FORMAT(CRAFT_KEYBOARD_EXCEPTION_EXTERNAL,
						"SDL_GetKeyboardState failed: %s (0x%X)", KEY_AS_STRING(id), id);	
				}

				m_key_map.insert(std::pair<SDL_Keycode, bool>(id, state[SDL_GetScancodeFromKey(id)]));
			}
		}

		void 
		_craft_keyboard::clear(void)
		{

			if(!m_initialized) {
				THROW_CRAFT_KEYBOARD_EXCEPTION(CRAFT_KEYBOARD_EXCEPTION_UNINITIALIZED);
			}

			m_key_map.clear();
		}

		bool 
		_craft_keyboard::contains(
			__in const SDL_Keycode &id
			)
		{

			if(!m_initialized) {
				THROW_CRAFT_KEYBOARD_EXCEPTION(CRAFT_KEYBOARD_EXCEPTION_UNINITIALIZED);
			}

			return (m_key_map.find(id) != m_key_map.end());
		}

		std::map<SDL_Keycode, bool>::iterator 
		_craft_keyboard::find(
			__in const SDL_Keycode &id
			)
		{
			std::map<SDL_Keycode, bool>::iterator result;

			if(!m_initialized) {
				THROW_CRAFT_KEYBOARD_EXCEPTION(CRAFT_KEYBOARD_EXCEPTION_UNINITIALIZED);
			}

			result = m_key_map.find(id);
			if(result == m_key_map.end()) {
				THROW_CRAFT_KEYBOARD_EXCEPTION_FORMAT(CRAFT_KEYBOARD_EXCEPTION_NOT_FOUND,
					"%s (0x%x)", KEY_AS_STRING(id), id);
			}

			return result;
		}

		void 
		_craft_keyboard::initialize(
			__in_opt const std::set<SDL_Keycode> &ids
			)
		{
			std::set<SDL_Keycode>::const_iterator iter;

			if(m_initialized) {
				THROW_CRAFT_KEYBOARD_EXCEPTION(CRAFT_KEYBOARD_EXCEPTION_INITIALIZED);
			}

			m_initialized = true;

			for(iter = ids.begin(); iter != ids.end(); ++iter) {
				add(*iter);
			}
		}

		bool 
		_craft_keyboard::is_allocated(void)
		{
			return (craft_keyboard::m_instance != NULL);
		}

		bool 
		_craft_keyboard::is_initialized(void)
		{
			return m_initialized;
		}

		bool 
		_craft_keyboard::is_pressed(
			__in const SDL_Keycode &id
			)
		{

			if(!m_initialized) {
				THROW_CRAFT_KEYBOARD_EXCEPTION(CRAFT_KEYBOARD_EXCEPTION_UNINITIALIZED);
			}

			return find(id)->second;
		}

		void 
		_craft_keyboard::on_event(
			__in const SDL_KeyboardEvent &event
			)
		{

			if(contains(event.keysym.sym)) {
				find(event.keysym.sym)->second = (event.state == SDL_PRESSED);
			}
		}

		void 
		_craft_keyboard::remove(
			__in const SDL_Keycode &id
			)
		{

			if(!m_initialized) {
				THROW_CRAFT_KEYBOARD_EXCEPTION(CRAFT_KEYBOARD_EXCEPTION_UNINITIALIZED);
			}

			m_key_map.erase(find(id));
		}

		void 
		_craft_keyboard::reset(void)
		{
			std::map<SDL_Keycode, bool>::iterator iter;

			if(!m_initialized) {
				THROW_CRAFT_KEYBOARD_EXCEPTION(CRAFT_KEYBOARD_EXCEPTION_UNINITIALIZED);
			}

			for(iter = m_key_map.begin(); iter != m_key_map.end(); ++iter) {
				iter->second = false;
			}
		}

		size_t 
		_craft_keyboard::size(void)
		{

			if(!m_initialized) {
				THROW_CRAFT_KEYBOARD_EXCEPTION(CRAFT_KEYBOARD_EXCEPTION_UNINITIALIZED);
			}

			return m_key_map.size();
		}

		std::string 
		_craft_keyboard::to_string(
			__in_opt bool verbose
			)
		{
			std::stringstream result;
			std::map<SDL_Keycode, bool>::iterator iter;

			result << CRAFT_KEYBOARD_HEADER << " (" << (m_initialized ? "INITIALIZED" : "UNINITIALIZED");

			if(verbose) {
				result << ", PTR. 0x" << SCALAR_AS_HEX(craft_keyboard *, this);
			}

			if(m_initialized && !m_key_map.empty()) {
				result << ", KEYS[" << m_key_map.size() << "] {";

				for(iter = m_key_map.begin(); iter != m_key_map.end(); ++iter) {

					if(iter != m_key_map.begin()) {
						result << ", ";
					}

					result << KEY_AS_STRING(iter->first) << "[0x" << SCALAR_AS_HEX(SDL_Keycode, iter->first) 
						<< "](" << iter->second << ")";
				}

				result << "}";
			}

			result << ")";

			return result.str();
		}

		void 
		_craft_keyboard::uninitialize(void)
		{

			if(!m_initialized) {
				THROW_CRAFT_KEYBOARD_EXCEPTION(CRAFT_KEYBOARD_EXCEPTION_UNINITIALIZED);
			}

			clear();
			m_initialized = false;
		}

		void 
		_craft_keyboard::update(void)
		{
			const Uint8 *state = NULL;
			std::map<SDL_Keycode, bool>::iterator iter;

			if(!m_initialized) {
				THROW_CRAFT_KEYBOARD_EXCEPTION(CRAFT_KEYBOARD_EXCEPTION_UNINITIALIZED);
			}

			state = SDL_GetKeyboardState(NULL);
			if(!state) {
				THROW_CRAFT_KEYBOARD_EXCEPTION_FORMAT(CRAFT_KEYBOARD_EXCEPTION_EXTERNAL,
					"%s", "SDL_GetKeyboardState failed");	
			}

			for(iter = m_key_map.begin(); iter != m_key_map.end(); ++iter) {
				iter->second = state[SDL_GetScancodeFromKey(iter->first)];
			}
		}
	}
}

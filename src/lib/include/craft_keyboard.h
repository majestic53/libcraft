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

#ifndef CRAFT_KEYBOARD_H_
#define CRAFT_KEYBOARD_H_

namespace CRAFT {

	namespace COMPONENT {

		typedef class _craft_keyboard {

			public:

				~_craft_keyboard(void);

				static _craft_keyboard *acquire(void);

				void add(
					__in const SDL_Keycode &id
					);

				void clear(void);

				bool contains(
					__in const SDL_Keycode &id
					);

				void initialize(
					__in_opt const std::set<SDL_Keycode> &ids = std::set<SDL_Keycode>()
					);

				static bool is_allocated(void);

				bool is_initialized(void);

				bool is_pressed(
					__in const SDL_Keycode &id
					);

				void on_event(
					__in const SDL_KeyboardEvent &event
					);

				void remove(
					__in const SDL_Keycode &id
					);

				void reset(void);

				size_t size(void);

				std::string to_string(
					__in_opt bool verbose = false
					);

				void uninitialize(void);

				void update(void);

			protected:

				_craft_keyboard(void);

				_craft_keyboard(
					__in const _craft_keyboard &other
					);

				_craft_keyboard &operator=(
					__in const _craft_keyboard &other
					);

				static void _delete(void);

				std::map<SDL_Keycode, bool>::iterator find(
					__in const SDL_Keycode &id
					);

				bool m_initialized;

				static _craft_keyboard *m_instance;

				std::map<SDL_Keycode, bool> m_key_map;

		} craft_keyboard;
	}
}

#endif // CRAFT_KEYBOARD_H_

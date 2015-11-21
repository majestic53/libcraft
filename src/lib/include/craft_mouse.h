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

#ifndef CRAFT_MOUSE_H_
#define CRAFT_MOUSE_H_

namespace CRAFT {

	namespace COMPONENT {

		typedef class _craft_mouse {

			public:

				~_craft_mouse(void);

				static _craft_mouse *acquire(void);

				void initialize(void);

				static bool is_allocated(void);

				bool is_initialized(void);

				void on_event(
					__in const SDL_MouseButtonEvent &event
					);

				void on_event(
					__in const SDL_MouseMotionEvent &event
					);

				void on_event(
					__in const SDL_MouseWheelEvent &event
					);

				std::string to_string(
					__in_opt bool verbose = false
					);

				void uninitialize(void);

			protected:

				_craft_mouse(void);

				_craft_mouse(
					__in const _craft_mouse &other
					);

				_craft_mouse &operator=(
					__in const _craft_mouse &other
					);

				static void _delete(void);

				bool m_initialized;

				static _craft_mouse *m_instance;

		} craft_mouse;
	}
}

#endif // CRAFT_MOUSE_H_

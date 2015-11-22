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

				void clear(void);

				void initialize(
					__in SDL_Window *parent,
					__in_opt bool relative = MOUSE_DEFAULT_RELATIVE
					);

				static bool is_allocated(void);

				bool is_double_click(void);

				bool is_initialized(void);

				bool is_left_click(void);

				bool is_middle_click(void);

				bool is_relative(void);

				bool is_right_click(void);

				bool is_scrolling(void);

				void on_event(
					__in const SDL_MouseButtonEvent &event
					);

				void on_event(
					__in const SDL_MouseMotionEvent &event
					);

				void on_event(
					__in const SDL_MouseWheelEvent &event
					);

				glm::ivec2 position(void);

				glm::ivec2 position_clicked(void);

				void reset(void);

				glm::ivec2 scroll(void);

				void set_position(
					__in int x,
					__in int y
					);

				std::string to_string(
					__in_opt bool verbose = false
					);

				void uninitialize(void);

				void update(void);

			protected:

				_craft_mouse(void);

				_craft_mouse(
					__in const _craft_mouse &other
					);

				_craft_mouse &operator=(
					__in const _craft_mouse &other
					);

				static void _delete(void);

				bool m_double_click;

				bool m_initialized;

				static _craft_mouse *m_instance;

				SDL_Window *m_parent;

				glm::ivec2 m_position;

				glm::ivec2 m_position_clicked;

				bool m_relative;

				glm::ivec2 m_scroll;

				Uint32 m_state;

		} craft_mouse;
	}
}

#endif // CRAFT_MOUSE_H_

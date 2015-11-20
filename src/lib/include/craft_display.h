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

#ifndef CRAFT_DISPLAY_H_
#define CRAFT_DISPLAY_H_

namespace CRAFT {

	namespace COMPONENT {

		typedef class _craft_display {

			public:

				~_craft_display(void);

				static _craft_display *acquire(void);

				SDL_GLContext context(void);

				void initialize(void);

				static bool is_allocated(void);

				bool is_initialized(void);

				bool is_running(void);

				void on_event(
					__in const SDL_WindowEvent &event
					);

				void start(
					__in const std::string &title,
					__in size_t left,
					__in size_t top,
					__in size_t width,
					__in size_t height,
					__in uint32_t flags
					);

				void stop(void);

				std::string to_string(
					__in_opt bool verbose = false
					);

				void uninitialize(void);

				SDL_Window *window(void);

			protected:

				_craft_display(void);

				_craft_display(
					__in const _craft_display &other
					);

				_craft_display &operator=(
					__in const _craft_display &other
					);

				static void _delete(void);

				void clear(void);

				void setup(
					__in const std::string &title,
					__in size_t left,
					__in size_t top,
					__in size_t width,
					__in size_t height,
					__in uint32_t flags
					);

				void teardown(void);

				SDL_GLContext m_context;

				bool m_initialized;

				static _craft_display *m_instance;

				bool m_running;

				SDL_Window *m_window;

		} craft_display;
	}
}

#endif // CRAFT_DISPLAY_H_

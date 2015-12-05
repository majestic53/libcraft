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

#ifndef CRAFT_WORLD_H_
#define CRAFT_WORLD_H_

namespace CRAFT {

	namespace COMPONENT {

		typedef class _craft_position_key {

			public:

				size_t operator()(
					__in const glm::vec2 &position
					) const;

				bool operator()(
					__in const glm::vec2 &left,
					__in const glm::vec2 &right
					) const;
		} craft_position_key;

		typedef class _craft_world {

			public:

				~_craft_world(void);

				static _craft_world *acquire(void);

				void clear(void);

				void initialize(
					__in uint32_t seed,
					__in double dimension,
					__in uint32_t octaves,
					__in double amplitude,
					__in double persistence,
					__in_opt bool bicubic = true
					);

				static bool is_allocated(void);

				bool is_initialized(void);

				void on_event(
					__in const SDL_KeyboardEvent &event
					);

				void on_event(
					__in const SDL_MouseButtonEvent &event
					);

				void on_event(
					__in const SDL_MouseMotionEvent &event
					);

				void on_event(
					__in const SDL_MouseWheelEvent &event
					);

				void poll_input(void);

				void render(void);

				void reset(void);

				std::string to_string(
					__in_opt bool verbose = false
					);

				void uninitialize(void);

				void update(
					__in GLfloat delta
					);

				void update_input(
					__in GLfloat delta
					);

				void update_world(
					__in GLfloat delta
					);

			protected:

				_craft_world(void);

				_craft_world(
					__in const _craft_world &other
					);

				_craft_world &operator=(
					__in const _craft_world &other
					);

				static void _delete(void);

				void setup(
					__in uint32_t seed,
					__in double dimension,
					__in uint32_t octaves,
					__in double amplitude,
					__in double persistence,
					__in_opt bool bicubic = true
					);

				void teardown(void);

				std::unordered_map<glm::vec2, craft_chunk, craft_position_key, 
					craft_position_key> m_chunk_map;

				std::vector<double> m_height_list;

				bool m_initialized;

				static _craft_world *m_instance;

				craft_camera *m_instance_camera;

				craft_keyboard *m_instance_keyboard;

				craft_mouse *m_instance_mouse;

				craft_random *m_instance_random;

				craft_test *m_instance_test;

				glm::mat4 m_mvp;

				SDL_Window *m_window;

		} craft_world;
	}
}

#endif // CRAFT_WORLD_H_

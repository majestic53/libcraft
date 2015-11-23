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

		typedef class _craft_world {

			public:

				~_craft_world(void);

				static _craft_world *acquire(void);

				void clear(void);

				void initialize(void);

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

				void reset(void);

				void uninitialize(void);

				glm::mat4 update(
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

				void setup(void);

				void teardown(void);

				bool m_initialized;

				static _craft_world *m_instance;

				craft_camera *m_instance_camera;

				craft_keyboard *m_instance_keyboard;

				craft_mouse *m_instance_mouse;

				glm::vec2 m_motion;

				GLfloat m_speed;

		} craft_world;
	}
}

#endif // CRAFT_WORLD_H_

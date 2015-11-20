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

#ifndef CRAFT_H_
#define CRAFT_H_

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <GLFW/glfw3.h>
#include <SDL2/SDL.h>

//#define NDEBUG

#ifndef CRAFT
#define CRAFT libcraft
#endif // CRAFT

#include "craft_define.h"
#include "craft_exception.h"
#include "craft_gl.h"

using namespace CRAFT;

#ifndef COMPONENT
#define COMPONENT component
#endif // COMPONENT

#include "craft_display.h"

using namespace CRAFT::COMPONENT;

namespace CRAFT {

	typedef class _craft {

		public:

			~_craft(void);

			static _craft *acquire(void);

			void initialize(void);

			static bool is_allocated(void);

			bool is_initialized(void);

			bool is_running(void);

			void start(void);

			void stop(void);

			std::string to_string(
				__in_opt bool verbose = false
				);

			void uninitialize(void);

			static std::string version(void);

		protected:

			_craft(void);

			_craft(
				__in const _craft &other
				);

			_craft &operator=(
				__in const _craft &other
				);

			static void _delete(void);

			void clear(void);

			void render(void);

			void run(void);

			void setup(void);

			void setup_external(void);

			void teardown(void);

			void teardown_external(void);

			void update(
				__in GLfloat delta
				);

			bool m_initialized;

			bool m_initialized_external;

			static _craft *m_instance;

			craft_display *m_instance_display;

			bool m_running;

	} craft;
}

#endif // CRAFT_H_

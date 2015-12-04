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

//#define NDEBUG

#ifndef CRAFT
#define CRAFT libcraft
#endif // CRAFT

#include "craft_define.h"
#include "craft_exception.h"
#include "craft_gl.h"
#include "craft_random.h"

using namespace CRAFT;

#ifndef TEST
#define TEST test
#endif // TEST

#include "craft_test.h"

using namespace CRAFT::TEST;

#ifndef COMPONENT
#define COMPONENT component
#endif // COMPONENT

#include "craft_chunk.h"
#include "craft_display.h"
#include "craft_keyboard.h"
#include "craft_mouse.h"
#include "craft_camera.h"
#include "craft_world.h"

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

			void start(
				__in uint32_t seed,
				__in bool fullscreen,
				__in_opt size_t width = WINDOW_WIDTH_MIN,
				__in_opt size_t height = WINDOW_HEIGHT_MIN
				);

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

			void run(
				__in uint32_t seed,
				__in bool fullscreen,
				__in_opt size_t width = WINDOW_WIDTH_MIN,
				__in_opt size_t height = WINDOW_HEIGHT_MIN
				);

			void setup(
				__in uint32_t seed,
				__in bool fullscreen,
				__in_opt size_t width = WINDOW_WIDTH_MIN,
				__in_opt size_t height = WINDOW_HEIGHT_MIN
				);

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

			craft_gl *m_instance_gl;

			craft_world *m_instance_world;

			bool m_running;

	} craft;
}

#endif // CRAFT_H_

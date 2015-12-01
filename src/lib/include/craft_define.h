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

#ifndef CRAFT_DEFINE_H_
#define CRAFT_DEFINE_H_

#include <cstdint>
#include <iomanip>
#include <iostream>
#include <map>
#include <set>
#include <sstream>
#include <string>
#include <vector>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/noise.hpp> 
#include <glm/gtc/type_ptr.hpp>
#include <GLFW/glfw3.h>
#include <SDL2/SDL.h>

namespace CRAFT {

#ifndef _WIN32
	#define __in
	#define __in_opt
	#define __inout
	#define __inout_opt
	#define __out
	#define __out_opt
	#define STRUCT_PACK(_NAME_, _STRUCT_) \
		typedef struct __attribute((packed)) _STRUCT_ _NAME_
#else
	#define STRUCT_PACK(_NAME_, _STRUCT_) \
		__pragma(pack(push, 1)) typedef struct _STRUCT_ _NAME_ __pragma(pack(pop))
#endif // _WIN32

	#define CAMERA_FOV 45.f
	#define CAMERA_HEIGHT 1.5f
	#define CAMERA_PITCH 0.f
	#define CAMERA_PITCH_MAX 89.f
	#define CAMERA_POSITION {0.f, CAMERA_HEIGHT, 0.f}
	#define CAMERA_SENSITIVITY 1.f
	#define CAMERA_SPEED 3.f
	#define CAMERA_TARGET {0.f, 0.f, 0.f}
	#define CAMERA_UP {0.f, 1.f, 0.f}
	#define CAMERA_YAW 0.f

	#define DISPLAY_ACCELERATE_VISUAL 1
	#define DISPLAY_DEPTH_SIZE 16
	#define DISPLAY_DOUBLE_BUFFER 1
	#define DISPLAY_MAJOR_VERSION 3
	#define DISPLAY_MINOR_VERSION 2
	#define DISPLAY_SWAP_INTERVAL 1
	#define DISPLAY_GL_VERSION GLEW_VERSION_3_2

	#define EVENT_DOUBLE_CLICK 2
	#define EVENT_FILTER \
		(SDL_INIT_AUDIO | SDL_INIT_EVENTS | SDL_INIT_TIMER | SDL_INIT_VIDEO)

	#define GL_VERSION_VALID() ((DISPLAY_GL_VERSION) == GL_TRUE)

	#define KEY_AS_STRING(_KEY_) \
		SDL_GetScancodeName(SDL_GetScancodeFromKey(_KEY_))

	#define MAT_INITIAL glm::make_mat4(MAT_EMPTY)
	#define MAT_UNIT 1.f

	#define MOUSE_DEFAULT_RELATIVE true
	#define MOUSE_DEFAULT_X 0
	#define MOUSE_DEFAULT_Y 0

	#define PERLIN_FREQ_SCALE 2.0
	#define PERLIN_SAMPLE_COUNT 16
	#define PERLIN_WEIGHT_0 (1.0 / 16.0)
	#define PERLIN_WEIGHT_1 (1.0 / 8.0)
	#define PERLIN_WEIGHT_2 (1.0 / 4.0)

	#define REFERENCE_INITIAL 1

	#define RESOLUTION_BUFFER 20

	#define SCALAR_INVALID(_TYPE_) ((_TYPE_) -1)

	#define SCALAR_AS_HEX(_TYPE_, _VAL_) \
		std::setw(sizeof(_TYPE_) * 2) << std::setfill('0') << std::hex \
		<< (uintmax_t) ((_TYPE_) (_VAL_)) << std::dec

	#define STRING_CHECK(_STR_) (_STR_.empty() ? STRING_EMPTY : _STR_.c_str())

	#define _STRING_CONCAT(_STR_) # _STR_
	#define STRING_CONCAT(_STR_) _STRING_CONCAT(_STR_)

	#define STRING_EMPTY "<EMPTY>"
	#define STRING_UNKNOWN "<UNKNOWN>"

	#define VERSION_MAJOR 0
	#define VERSION_MINOR 1
	#define VERSION_REVISION 1
	#define VERSION_STRING \
		STRING_CONCAT(VERSION_MAJOR) "." STRING_CONCAT(VERSION_MINOR) \
		"." STRING_CONCAT(VERSION_TICK) "." STRING_CONCAT(VERSION_REVISION)
	#define VERSION_TICK 1549

	#define WINDOW_FLAGS (SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN)// | SDL_WINDOW_INPUT_GRABBED)
	#define WINDOW_HEIGHT_MIN 480
	#define WINDOW_LEFT SDL_WINDOWPOS_CENTERED
	#define WINDOW_TITLE "LIBCRAFT -- " VERSION_STRING
	#define WINDOW_TOP SDL_WINDOWPOS_CENTERED
	#define WINDOW_WIDTH_MIN 640

	enum {
		KEY_QUIT = 0,
		KEY_FORWARD,
		KEY_LEFT,
		KEY_BACKWARD,
		KEY_RIGHT,
	};

	#define KEY_MAX KEY_RIGHT

	static const SDL_Keycode KEYS[] = {
		SDLK_ESCAPE, SDLK_w, SDLK_a, SDLK_s, SDLK_d,
		};

	static const std::set<SDL_Keycode> KEY_SET(
		KEYS, KEYS + (KEY_MAX + 1)
		);

	#define KEY_CODE(_KEY_) \
		((_KEY_) > KEY_MAX ? SCALAR_INVALID(SDL_Keycode) : KEYS[_KEY_])

	static const float MAT_EMPTY[] = {
		0.f, 0.f, 0.f, 0.f,
		0.f, 0.f, 0.f, 0.f,
		0.f, 0.f, 0.f, 0.f,
		0.f, 0.f, 0.f, 0.f,
		};
}

#endif // CRAFT_DEFINE_H_

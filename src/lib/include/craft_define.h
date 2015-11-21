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
#include <sstream>
#include <string>
#include <vector>

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

	#define DISPLAY_ACCELERATE_VISUAL 1
	#define DISPLAY_DEPTH_SIZE 16
	#define DISPLAY_DOUBLE_BUFFER 1
	#define DISPLAY_MAJOR_VERSION 3
	#define DISPLAY_MINOR_VERSION 2
	#define DISPLAY_SWAP_INTERVAL 1
	#define DISPLAY_GL_VERSION GLEW_VERSION_3_2

	#define EVENT_FILTER \
		(SDL_INIT_AUDIO | SDL_INIT_EVENTS | SDL_INIT_TIMER | SDL_INIT_VIDEO)

	#define GL_VERSION_VALID() ((DISPLAY_GL_VERSION) == GL_TRUE)

	#define REFERENCE_INITIAL 1

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
	#define VERSION_REVISION 4
	#define VERSION_STRING \
		STRING_CONCAT(VERSION_MAJOR) "." STRING_CONCAT(VERSION_MINOR) \
		"." STRING_CONCAT(VERSION_TICK) "." STRING_CONCAT(VERSION_REVISION)
	#define VERSION_TICK 1547

	#define WINDOW_FLAGS (SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN)// | SDL_WINDOW_INPUT_GRABBED)
	#define WINDOW_HEIGHT 768
	#define WINDOW_LEFT SDL_WINDOWPOS_CENTERED
	#define WINDOW_TITLE "LIBCRAFT -- " STRING_CONCAT(WINDOW_WIDTH) "x" \
		STRING_CONCAT(WINDOW_HEIGHT)
	#define WINDOW_TOP SDL_WINDOWPOS_CENTERED
	#define WINDOW_WIDTH 1024
}

#endif // CRAFT_DEFINE_H_

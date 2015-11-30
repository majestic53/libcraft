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

#ifndef CRAFT_RANDOM_TYPE_H_
#define CRAFT_RANDOM_TYPE_H_

namespace CRAFT {

#ifndef NDEBUG
	#define CRAFT_RANDOM_EXCEPTION_HEADER CRAFT_RANDOM_HEADER
#else
	#define CRAFT_RANDOM_EXCEPTION_HEADER EXCEPTION_HEADER
#endif // NDEBUG
	#define CRAFT_PERLIN_HEADER "<PERLIN>"
	#define CRAFT_RANDOM_HEADER "<RANDOM>"

	enum {
		CRAFT_RANDOM_EXCEPTION_ALLOCATED = 0,
		CRAFT_RANDOM_EXCEPTION_FILE_NOT_FOUND,
		CRAFT_RANDOM_EXCEPTION_INITIALIZED,
		CRAFT_RANDOM_EXCEPTION_INVALID_DIMENSIONS,
		CRAFT_RANDOM_EXCEPTION_INVALID_RANGE,
		CRAFT_RANDOM_EXCEPTION_UNINITIALIZED,
	};

	#define CRAFT_RANDOM_EXCEPTION_MAX CRAFT_RANDOM_EXCEPTION_UNINITIALIZED

	static const std::string CRAFT_RANDOM_EXCEPTION_STR[] = {
		CRAFT_RANDOM_EXCEPTION_HEADER " Failed to allocate random component",
		CRAFT_RANDOM_EXCEPTION_HEADER " File does not exist",
		CRAFT_RANDOM_EXCEPTION_HEADER " Random component is initialized",
		CRAFT_RANDOM_EXCEPTION_HEADER " Invalid dimensions",
		CRAFT_RANDOM_EXCEPTION_HEADER " Invalid range",
		CRAFT_RANDOM_EXCEPTION_HEADER " Random component is uninitialized",
		};

	#define CRAFT_RANDOM_EXCEPTION_STRING(_TYPE_) \
		((_TYPE_) > CRAFT_RANDOM_EXCEPTION_MAX ? EXCEPTION_UNKNOWN : \
		STRING_CHECK(CRAFT_RANDOM_EXCEPTION_STR[_TYPE_]))

	#define THROW_CRAFT_RANDOM_EXCEPTION(_EXCEPT_) \
		THROW_EXCEPTION(CRAFT_RANDOM_EXCEPTION_STRING(_EXCEPT_))
	#define THROW_CRAFT_RANDOM_EXCEPTION_FORMAT(_EXCEPT_, _FORMAT_, ...) \
		THROW_EXCEPTION_FORMAT(CRAFT_RANDOM_EXCEPTION_STRING(_EXCEPT_), \
		_FORMAT_, __VA_ARGS__)
}

#endif // CRAFT_RANDOM_TYPE_H_

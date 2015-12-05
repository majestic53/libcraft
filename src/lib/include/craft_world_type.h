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

#ifndef CRAFT_WORLD_TYPE_H_
#define CRAFT_WORLD_TYPE_H_

namespace CRAFT {

	namespace COMPONENT {

#ifndef NDEBUG
		#define CRAFT_WORLD_EXCEPTION_HEADER CRAFT_WORLD_HEADER
#else
		#define CRAFT_WORLD_EXCEPTION_HEADER EXCEPTION_HEADER
#endif // NDEBUG
		#define CRAFT_WORLD_HEADER "<WORLD>"

		enum {
			CRAFT_WORLD_EXCEPTION_ALLOCATED = 0,
			CRAFT_WORLD_EXCEPTION_INITIALIZED,
			CRAFT_WORLD_EXCEPTION_INVALID_DIMENSION,
			CRAFT_WORLD_EXCEPTION_UNINITIALIZED,
		};

		#define CRAFT_WORLD_EXCEPTION_MAX CRAFT_WORLD_EXCEPTION_UNINITIALIZED

		static const std::string CRAFT_WORLD_EXCEPTION_STR[] = {
			CRAFT_WORLD_EXCEPTION_HEADER " Failed to allocate world component",
			CRAFT_WORLD_EXCEPTION_HEADER " World component initialized",
			CRAFT_WORLD_EXCEPTION_HEADER " Invalid dimension",
			CRAFT_WORLD_EXCEPTION_HEADER " World component uninitialized",
			};

		#define CRAFT_WORLD_EXCEPTION_STRING(_TYPE_) \
			((_TYPE_) > CRAFT_WORLD_EXCEPTION_MAX ? EXCEPTION_UNKNOWN : \
			STRING_CHECK(CRAFT_WORLD_EXCEPTION_STR[_TYPE_]))

		#define THROW_CRAFT_WORLD_EXCEPTION(_EXCEPT_) \
			THROW_EXCEPTION(CRAFT_WORLD_EXCEPTION_STRING(_EXCEPT_))
		#define THROW_CRAFT_WORLD_EXCEPTION_FORMAT(_EXCEPT_, _FORMAT_, ...) \
			THROW_EXCEPTION_FORMAT(CRAFT_WORLD_EXCEPTION_STRING(_EXCEPT_), \
			_FORMAT_, __VA_ARGS__)
	}
}

#endif // CRAFT_WORLD_TYPE_H_

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

#ifndef CRAFT_CHUNK_TYPE_H_
#define CRAFT_CHUNK_TYPE_H_

namespace CRAFT {

	namespace COMPONENT {

#ifndef NDEBUG
		#define CRAFT_CHUNK_EXCEPTION_HEADER CRAFT_CHUNK_HEADER
#else
		#define CRAFT_CHUNK_EXCEPTION_HEADER EXCEPTION_HEADER
#endif // NDEBUG
		#define CRAFT_CHUNK_HEADER "<CHUNK>"

		enum {
			CRAFT_CHUNK_EXCEPTION_INVALID_DIMENSION = 0,
			CRAFT_CHUNK_EXCEPTION_INVALID_HEIGHT_MAP,
			CRAFT_CHUNK_EXCEPTION_INVALID_POSITION,
			CRAFT_CHUNK_EXCEPTION_INVALID_TYPE,
		};

		#define CRAFT_CHUNK_EXCEPTION_MAX CRAFT_CHUNK_EXCEPTION_INVALID_TYPE

		static const std::string CRAFT_CHUNK_EXCEPTION_STR[] = {
			CRAFT_CHUNK_EXCEPTION_HEADER " Invalid dimension",
			CRAFT_CHUNK_EXCEPTION_HEADER " Invalid height map",
			CRAFT_CHUNK_EXCEPTION_HEADER " Invalid position",
			CRAFT_CHUNK_EXCEPTION_HEADER " Invalid type",
			};

		#define CRAFT_CHUNK_EXCEPTION_STRING(_TYPE_) \
			((_TYPE_) > CRAFT_CHUNK_EXCEPTION_MAX ? EXCEPTION_UNKNOWN : \
			STRING_CHECK(CRAFT_CHUNK_EXCEPTION_STR[_TYPE_]))

		#define THROW_CRAFT_CHUNK_EXCEPTION(_EXCEPT_) \
			THROW_EXCEPTION(CRAFT_CHUNK_EXCEPTION_STRING(_EXCEPT_))
		#define THROW_CRAFT_CHUNK_EXCEPTION_FORMAT(_EXCEPT_, _FORMAT_, ...) \
			THROW_EXCEPTION_FORMAT(CRAFT_CHUNK_EXCEPTION_STRING(_EXCEPT_), \
			_FORMAT_, __VA_ARGS__)
	}
}

#endif // CRAFT_CHUNK_TYPE_H_

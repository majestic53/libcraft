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

#ifndef CRAFT_TEXT_TYPE_H_
#define CRAFT_TEXT_TYPE_H_

namespace CRAFT {

	namespace COMPONENT {

#ifndef NDEBUG
		#define CRAFT_TEXT_EXCEPTION_HEADER CRAFT_TEXT_HEADER
#else
		#define CRAFT_TEXT_EXCEPTION_HEADER EXCEPTION_HEADER
#endif // NDEBUG
		#define CRAFT_TEXT_HEADER "<TEXT>"

		enum {
			CRAFT_TEXT_EXCEPTION_ALLOCATED = 0,
			CRAFT_TEXT_EXCEPTION_EXTERNAL,
			CRAFT_TEXT_EXCEPTION_INITIALIZED,
			CRAFT_TEXT_EXCEPTION_UNINITIALIZED,
		};

		#define CRAFT_TEXT_EXCEPTION_MAX CRAFT_TEXT_EXCEPTION_ALLOCATED

		static const std::string CRAFT_TEXT_EXCEPTION_STR[] = {
			CRAFT_TEXT_EXCEPTION_HEADER " Failed to allocate text component",
			CRAFT_TEXT_EXCEPTION_HEADER " External exception",
			CRAFT_TEXT_EXCEPTION_HEADER " Text component is initialized",
			CRAFT_TEXT_EXCEPTION_HEADER " Text component is uninitialized"
			};

		#define CRAFT_TEXT_EXCEPTION_STRING(_TYPE_) \
			((_TYPE_) > CRAFT_TEXT_EXCEPTION_MAX ? EXCEPTION_UNKNOWN : \
			STRING_CHECK(CRAFT_TEXT_EXCEPTION_STR[_TYPE_]))

		#define THROW_CRAFT_TEXT_EXCEPTION(_EXCEPT_) \
			THROW_EXCEPTION(CRAFT_TEXT_EXCEPTION_STRING(_EXCEPT_))
		#define THROW_CRAFT_TEXT_EXCEPTION_FORMAT(_EXCEPT_, _FORMAT_, ...) \
			THROW_EXCEPTION_FORMAT(CRAFT_TEXT_EXCEPTION_STRING(_EXCEPT_), \
			_FORMAT_, __VA_ARGS__)
	}
}

#endif // CRAFT_TEXT_TYPE_H_

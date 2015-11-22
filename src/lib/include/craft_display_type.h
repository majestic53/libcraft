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

#ifndef CRAFT_DISPLAY_TYPE_H_
#define CRAFT_DISPLAY_TYPE_H_

namespace CRAFT {

	namespace COMPONENT {

#ifndef NDEBUG
		#define CRAFT_DISPLAY_EXCEPTION_HEADER CRAFT_DISPLAY_HEADER
#else
		#define CRAFT_DISPLAY_EXCEPTION_HEADER EXCEPTION_HEADER
#endif // NDEBUG
		#define CRAFT_DISPLAY_HEADER "<DISPLAY>"

		enum {
			CRAFT_DISPLAY_EXCEPTION_ALLOCATED = 0,
			CRAFT_DISPLAY_EXCEPTION_EXTERNAL,
			CRAFT_DISPLAY_EXCEPTION_INITIALIZED,
			CRAFT_DISPLAY_EXCEPTION_INVALID,
			CRAFT_DISPLAY_EXCEPTION_MISSING,
			CRAFT_DISPLAY_EXCEPTION_MISSING_MODE,
			CRAFT_DISPLAY_EXCEPTION_STARTED,
			CRAFT_DISPLAY_EXCEPTION_STOPPED,
			CRAFT_DISPLAY_EXCEPTION_UNINITIALIZED,
		};

		#define CRAFT_DISPLAY_EXCEPTION_MAX CRAFT_DISPLAY_EXCEPTION_UNINITIALIZED

		static const std::string CRAFT_DISPLAY_EXCEPTION_STR[] = {
			CRAFT_DISPLAY_EXCEPTION_HEADER " Failed to allocate display component",
			CRAFT_DISPLAY_EXCEPTION_HEADER " External exception",
			CRAFT_DISPLAY_EXCEPTION_HEADER " Display component is initialized",
			CRAFT_DISPLAY_EXCEPTION_HEADER " Display resolution is invalid",
			CRAFT_DISPLAY_EXCEPTION_HEADER " Display is missing",
			CRAFT_DISPLAY_EXCEPTION_HEADER " Display mode is missing",
			CRAFT_DISPLAY_EXCEPTION_HEADER " Display component is started",
			CRAFT_DISPLAY_EXCEPTION_HEADER " Display component is stopped",
			CRAFT_DISPLAY_EXCEPTION_HEADER " Display component is uninitialized",
			};

		#define CRAFT_DISPLAY_EXCEPTION_STRING(_TYPE_) \
			((_TYPE_) > CRAFT_DISPLAY_EXCEPTION_MAX ? EXCEPTION_UNKNOWN : \
			STRING_CHECK(CRAFT_DISPLAY_EXCEPTION_STR[_TYPE_]))

		#define THROW_CRAFT_DISPLAY_EXCEPTION(_EXCEPT_) \
			THROW_EXCEPTION(CRAFT_DISPLAY_EXCEPTION_STRING(_EXCEPT_))
		#define THROW_CRAFT_DISPLAY_EXCEPTION_FORMAT(_EXCEPT_, _FORMAT_, ...) \
			THROW_EXCEPTION_FORMAT(CRAFT_DISPLAY_EXCEPTION_STRING(_EXCEPT_), \
			_FORMAT_, __VA_ARGS__)
	}
}

#endif // CRAFT_DISPLAY_TYPE_H_

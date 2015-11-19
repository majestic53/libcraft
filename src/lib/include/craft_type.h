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

#ifndef CRAFT_TYPE_H_
#define CRAFT_TYPE_H_

namespace CRAFT {

#ifndef NDEBUG
	#define CRAFT_EXCEPTION_HEADER CRAFT_HEADER
#else
	#define CRAFT_EXCEPTION_HEADER EXCEPTION_HEADER
#endif // NDEBUG
	#define CRAFT_HEADER "<CRAFT>"

	enum {
		CRAFT_EXCEPTION_ALLOCATED = 0,
		CRAFT_EXCEPTION_EXTERNAL,
		CRAFT_EXCEPTION_EXTERNAL_INITIALIZED,
		CRAFT_EXCEPTION_EXTERNAL_UNINITIALIZED,
		CRAFT_EXCEPTION_INITIALIZED,
		CRAFT_EXCEPTION_STARTED,
		CRAFT_EXCEPTION_STOPPED,
		CRAFT_EXCEPTION_UNINITIALIZED,
	};

	#define CRAFT_EXCEPTION_MAX CRAFT_EXCEPTION_UNINITIALIZED

	static const std::string CRAFT_EXCEPTION_STR[] = {
		CRAFT_EXCEPTION_HEADER " Failed to allocate library",
		CRAFT_EXCEPTION_HEADER " External exception",
		CRAFT_EXCEPTION_HEADER " External components are initialized",
		CRAFT_EXCEPTION_HEADER " External components are uninitialized",
		CRAFT_EXCEPTION_HEADER " Library is initialized",
		CRAFT_EXCEPTION_HEADER " Library is started",
		CRAFT_EXCEPTION_HEADER " Library is stopped",
		CRAFT_EXCEPTION_HEADER " Library is uninitialized",
		};

	#define CRAFT_EXCEPTION_STRING(_TYPE_) \
		((_TYPE_) > CRAFT_EXCEPTION_MAX ? EXCEPTION_UNKNOWN : \
		STRING_CHECK(CRAFT_EXCEPTION_STR[_TYPE_]))

	#define THROW_CRAFT_EXCEPTION(_EXCEPT_) \
		THROW_EXCEPTION(CRAFT_EXCEPTION_STRING(_EXCEPT_))
	#define THROW_CRAFT_EXCEPTION_FORMAT(_EXCEPT_, _FORMAT_, ...) \
		THROW_EXCEPTION_FORMAT(CRAFT_EXCEPTION_STRING(_EXCEPT_), \
		_FORMAT_, __VA_ARGS__)
}

#endif // CRAFT_TYPE_H_

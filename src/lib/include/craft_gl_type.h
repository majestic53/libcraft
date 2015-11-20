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

#ifndef CRAFT_GL_TYPE_H_
#define CRAFT_GL_TYPE_H_

namespace CRAFT {

#ifndef NDEBUG
	#define CRAFT_GL_EXCEPTION_HEADER CRAFT_GL_HEADER
#else
	#define CRAFT_GL_EXCEPTION_HEADER EXCEPTION_HEADER
#endif // NDEBUG
	#define CRAFT_GL_HEADER "<GL>"

	enum {
		CRAFT_GL_EXCEPTION_ALLOCATED = 0,
		CRAFT_GL_EXCEPTION_EXTERNAL,
		CRAFT_GL_EXCEPTION_INITIALIZED,
		CRAFT_GL_EXCEPTION_UNINITIALIZE,
	};

	#define CRAFT_GL_EXCEPTION_MAX CRAFT_GL_EXCEPTION_UNINITIALIZE

	static const std::string CRAFT_GL_EXCEPTION_STR[] = {
		CRAFT_GL_EXCEPTION_HEADER " Failed to allocate gl component",
		CRAFT_GL_EXCEPTION_HEADER " External exception",
		CRAFT_GL_EXCEPTION_HEADER " Gl component is initialized",
		CRAFT_GL_EXCEPTION_HEADER " Gl component is uninitialized",
		};

	#define CRAFT_GL_EXCEPTION_STRING(_TYPE_) \
		((_TYPE_) > CRAFT_GL_EXCEPTION_MAX ? EXCEPTION_UNKNOWN : \
		STRING_CHECK(CRAFT_GL_EXCEPTION_STR[_TYPE_]))

	#define THROW_CRAFT_GL_EXCEPTION(_EXCEPT_) \
		THROW_EXCEPTION(CRAFT_GL_EXCEPTION_STRING(_EXCEPT_))
	#define THROW_CRAFT_GL_EXCEPTION_FORMAT(_EXCEPT_, _FORMAT_, ...) \
		THROW_EXCEPTION_FORMAT(CRAFT_GL_EXCEPTION_STRING(_EXCEPT_), \
		_FORMAT_, __VA_ARGS__)
}

#endif // CRAFT_GL_TYPE_H_

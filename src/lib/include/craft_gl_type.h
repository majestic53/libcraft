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
		CRAFT_GL_EXCEPTION_FILE_COMPRESSION,
		CRAFT_GL_EXCEPTION_FILE_DEPTH,
		CRAFT_GL_EXCEPTION_FILE_DIMENSIONS,
		CRAFT_GL_EXCEPTION_FILE_EMPTY,
		CRAFT_GL_EXCEPTION_FILE_MALFORMED,
		CRAFT_GL_EXCEPTION_FILE_NOT_FOUND,
		CRAFT_GL_EXCEPTION_INITIALIZED,
		CRAFT_GL_EXCEPTION_PROGRAM_ATTRIBUTE_NOT_FOUND,
		CRAFT_GL_EXCEPTION_PROGRAM_NOT_FOUND,
		CRAFT_GL_EXCEPTION_PROGRAM_UNIFORM_NOT_FOUND,
		CRAFT_GL_EXCEPTION_SHADER_NOT_FOUND,
		CRAFT_GL_EXCEPTION_TEXTURE_NOT_FOUND,
		CRAFT_GL_EXCEPTION_UNINITIALIZED,
	};

	#define CRAFT_GL_EXCEPTION_MAX CRAFT_GL_EXCEPTION_UNINITIALIZED

	static const std::string CRAFT_GL_EXCEPTION_STR[] = {
		CRAFT_GL_EXCEPTION_HEADER " Failed to allocate gl component",
		CRAFT_GL_EXCEPTION_HEADER " External exception",
		CRAFT_GL_EXCEPTION_HEADER " Unsupported image compression",
		CRAFT_GL_EXCEPTION_HEADER " Invalid image color depth",
		CRAFT_GL_EXCEPTION_HEADER " Invalid image dimensions",
		CRAFT_GL_EXCEPTION_HEADER " File is empty",
		CRAFT_GL_EXCEPTION_HEADER " File is malformed",
		CRAFT_GL_EXCEPTION_HEADER " File does not exist",
		CRAFT_GL_EXCEPTION_HEADER " Gl component is initialized",
		CRAFT_GL_EXCEPTION_HEADER " Program attribute does not exist",
		CRAFT_GL_EXCEPTION_HEADER " Program does not exist",
		CRAFT_GL_EXCEPTION_HEADER " Program uniform does not exist",
		CRAFT_GL_EXCEPTION_HEADER " Shader does not exist",
		CRAFT_GL_EXCEPTION_HEADER " Texture does not exist",
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

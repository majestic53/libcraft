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

#ifndef CRAFT_GL_H_
#define CRAFT_GL_H_

namespace CRAFT {

	typedef class _craft_gl {

		public:

			~_craft_gl(void);

			static _craft_gl *acquire(void);

			GLuint add_pogram(
				__in GLuint fragment,
				__in GLuint vertex
				);

			GLuint add_shader(
				__in const std::string &source,
				__in GLenum type
				);

			GLuint add_texture(
				__in const std::string &path,
				__in_opt GLint filter = GL_LINEAR,
				__in_opt GLint wrap = GL_CLAMP_TO_EDGE
				);

			GLuint add_texture(
				__in const std::vector<uint8_t> &source,
				__in GLfloat width,
				__in GLfloat height,
				__in_opt GLint format = GL_RGBA,
				__in_opt GLint filter = GL_LINEAR,
				__in_opt GLint wrap = GL_CLAMP_TO_EDGE
				);

			void clear(void);

			bool contains_program(
				__in GLuint id
				);

			bool contains_shader(
				__in GLuint id
				);

			bool contains_texture(
				__in GLuint id
				);

			size_t decrement_program_reference(
				__in GLuint id
				);

			size_t decrement_shader_reference(
				__in GLuint id
				);

			size_t decrement_texture_reference(
				__in GLuint id
				);

			size_t increment_program_reference(
				__in GLuint id
				);

			size_t increment_shader_reference(
				__in GLuint id
				);

			size_t increment_texture_reference(
				__in GLuint id
				);

			void initialize(void);

			static void initialize_external(
				__in uint32_t version
				);

			static bool is_allocated(void);

			bool is_initialized(void);

			GLint program_attribute(
				__in const std::string &name,
				__in GLuint id
				);

			size_t program_count(void);

			size_t program_reference(
				__in GLuint id
				);

			std::pair<GLuint, GLuint> program_shaders(
				__in GLuint id
				);

			GLint program_uniform(
				__in const std::string &name,
				__in GLuint id
				);

			size_t shader_count(void);

			size_t shader_reference(
				__in GLuint id
				);

			GLenum shader_type(
				__in GLuint id
				);

			size_t texture_count(void);

			std::pair<GLfloat, GLfloat> texture_dimensions(
				__in GLuint id
				);

			size_t texture_reference(
				__in GLuint id
				);

			std::string to_string(
				__in_opt bool verbose = false
				);

			void uninitialize(void);

		protected:

			_craft_gl(void);

			_craft_gl(
				__in const _craft_gl &other
				);

			_craft_gl &operator=(
				__in const _craft_gl &other
				);

			static void _delete(void);

			static std::vector<uint8_t> bitmap(
				__in const std::string &path,
				__out GLint &format,
				__out GLfloat &width,
				__out GLfloat &height
				);

			std::map<GLuint, std::pair<std::pair<GLuint, GLuint>, size_t>>::iterator find_program(
				__in GLuint id
				);

			std::map<GLuint, std::pair<GLenum, size_t>>::iterator find_shader(
				__in GLuint id
				);

			std::map<GLuint, std::pair<std::pair<std::pair<GLfloat, GLfloat>, GLint>, size_t>>::iterator find_texture(
				__in GLuint id
				);

			bool m_initialized;

			static _craft_gl *m_instance;

			std::map<GLuint, std::pair<std::pair<GLuint, GLuint>, size_t>> m_program_map;

			std::map<GLuint, std::pair<GLenum, size_t>> m_shader_map;

			std::map<GLuint, std::pair<std::pair<std::pair<GLfloat, GLfloat>, GLint>, size_t>> m_texture_map;

	} craft_gl;
}

#endif // CRAFT_GL_H_

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

#ifndef CRAFT_TEST_H_
#define CRAFT_TEST_H_

namespace CRAFT {

	namespace TEST {

		typedef class _craft_test {

			public:

				~_craft_test(void);

				static _craft_test *acquire(void);

				void initialize(void);

				static bool is_allocated(void);

				bool is_initialized(void);

				void render(
					__in const glm::mat4 &mvp
					);

				std::string to_string(
					__in_opt bool verbose = false
					);

				void uninitialize(void);

				void update(
					__in GLfloat delta
					);

			protected:

				_craft_test(void);

				_craft_test(
					__in const _craft_test &other
					);

				_craft_test &operator=(
					__in const _craft_test &other
					);

				static void _delete(void);

				void setup(void);

				void teardown(void);

				GLuint m_array_vertex;

				GLint m_attribute_coordinate;

				GLint m_attribute_vertex;

				GLuint m_buffer_fragment;

				GLuint m_buffer_vertex;

				bool m_initialized;

				static _craft_test *m_instance;

				GLint m_matrix;

				GLuint m_program;

				GLuint m_shader_fragment;

				GLuint m_shader_vertex;

				GLuint m_texture;

				GLint m_texture_uniform;

		} craft_test;
	}
}

#endif // CRAFT_TEST_H_

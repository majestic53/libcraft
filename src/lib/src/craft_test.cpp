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

#include "../include/craft.h"
#include "../include/craft_test_type.h"

namespace CRAFT {

	namespace TEST {

		#define ATTRIBUTE_COORDINATE "coordinate"
		#define ATTRIBUTE_VERTEX "vertex"
		#define MVP_UNIFORM "mvp"
		#define SHADER_FRAGMENT "./res/test/fragment.glsl"
		#define SHADER_VERTEX "./res/test/vertex.glsl"
		#define TEXTURE "./res/test/block.bmp"

		static const GLfloat data_coordinate[] = {

			// front (+z)
			0.f, 0.f,
			1.f, 0.f,
			0.f, 1.f,
			0.f, 1.f,
			1.f, 0.f,
			1.f, 1.f,

			// back (-z)
			0.f, 0.f,
			0.f, 1.f,
			1.f, 0.f,
			1.f, 0.f,
			0.f, 1.f,
			1.f, 1.f,

			// right (+x)
			0.f, 0.f,
			0.f, 1.f,
			1.f, 0.f,
			1.f, 0.f,
			0.f, 1.f,
			1.f, 1.f,

			// left (-x)
			0.f, 0.f,
			1.f, 0.f,
			0.f, 1.f,
			0.f, 1.f,
			1.f, 0.f,
			1.f, 1.f,

			// bottom (-y)
			0.f, 0.f,
			1.f, 0.f,
			0.f, 1.f,
			0.f, 1.f,
			1.f, 0.f,
			1.f, 1.f,

			// top (+y)
			0.f, 0.f,
			0.f, 1.f,
			1.f, 0.f,
			1.f, 0.f,
			0.f, 1.f,
			1.f, 1.f,
			};

		static const GLfloat data_vertex[] = {

			// front (+z)
			-0.5f, -0.5f, 0.5f,
			0.5f, -0.5f, 0.5f,
			-0.5f, 0.5f, 0.5f,
			-0.5f, 0.5f, 0.5f,
			0.5f, -0.5f, 0.5f,
			0.5f, 0.5f, 0.5f,

			// back (-z)
			-0.5f, -0.5f, -0.5f,
			-0.5f, 0.5f, -0.5f,
			0.5f, -0.5f, -0.5f,
			0.5f, -0.5f, -0.5f,
			-0.5f, 0.5f, -0.5f,
			0.5f, 0.5f, -0.5f,

			// right (+x)
			0.5f, -0.5f, -0.5f, 
			0.5f, 0.5f, -0.5f, 
			0.5f, -0.5f, 0.5f, 
			0.5f, -0.5f, 0.5f, 
			0.5f, 0.5f, -0.5f, 
			0.5f, 0.5f, 0.5f,

			// left (-x)
			-0.5f, -0.5f, -0.5f, 
			-0.5f, -0.5f, 0.5f, 
			-0.5f, 0.5f, -0.5f, 
			-0.5f, 0.5f, -0.5f, 
			-0.5f, -0.5f, 0.5f, 
			-0.5f, 0.5f, 0.5f,

			// bottom (-y)
			-0.5f, -0.5f, -0.5f,
			0.5f, -0.5f, -0.5f,
			-0.5f, -0.5f, 0.5f,
			-0.5f, -0.5f, 0.5f,
			0.5f, -0.5f, -0.5f,
			0.5f, -0.5f, 0.5f,

			// top (+y)
			-0.5f, 0.5f, -0.5f,
			-0.5f, 0.5f, 0.5f,
			0.5f, 0.5f, -0.5f,
			0.5f, 0.5f, -0.5f,
			-0.5f, 0.5f, 0.5f,
			0.5f, 0.5f, 0.5f,
			};

		#define DATA_VERTEX_LEN 36

		_craft_test *_craft_test::m_instance = NULL;

		_craft_test::_craft_test(void) :
			m_array_vertex(0),
			m_attribute_coordinate(0),
			m_attribute_vertex(0),
			m_buffer_fragment(0),
			m_buffer_vertex(0),
			m_initialized(false),
			m_matrix(0),
			m_program(0),
			m_shader_fragment(0),
			m_shader_vertex(0),
			m_texture(0),
			m_texture_uniform(0)
		{
			std::atexit(craft_test::_delete);
		}

		_craft_test::~_craft_test(void)
		{

			if(m_initialized) {
				uninitialize();
			}
		}

		void 
		_craft_test::_delete(void)
		{

			if(craft_test::m_instance) {
				delete craft_test::m_instance;
				craft_test::m_instance = NULL;
			}
		}

		_craft_test *
		_craft_test::acquire(void)
		{

			if(!craft_test::m_instance) {

				craft_test::m_instance = new craft_test;
				if(!craft_test::m_instance) {
					THROW_CRAFT_TEST_EXCEPTION(CRAFT_TEST_EXCEPTION_ALLOCATED);
				}
			}

			return craft_test::m_instance;
		}

		void 
		_craft_test::initialize(void)
		{

			if(m_initialized) {
				THROW_CRAFT_TEST_EXCEPTION(CRAFT_TEST_EXCEPTION_INITIALIZED);
			}

			m_initialized = true;
			setup();
		}

		bool 
		_craft_test::is_allocated(void)
		{
			return (craft_test::m_instance != NULL);
		}

		bool 
		_craft_test::is_initialized(void)
		{
			return m_initialized;
		}

		void 
		_craft_test::render(
			__in const glm::mat4 &mvp
			)
		{

			if(!m_initialized) {
				THROW_CRAFT_TEST_EXCEPTION(CRAFT_TEST_EXCEPTION_UNINITIALIZED);
			}

			glUniformMatrix4fv(m_matrix, 1, GL_FALSE, glm::value_ptr(mvp));
			glEnableVertexAttribArray(m_attribute_vertex);
			glBindBuffer(GL_ARRAY_BUFFER, m_buffer_vertex);
			glVertexAttribPointer(m_attribute_vertex, 3, GL_FLOAT, GL_FALSE, 0, NULL);
			glEnableVertexAttribArray(m_attribute_coordinate);
			glBindBuffer(GL_ARRAY_BUFFER, m_buffer_fragment);
			glVertexAttribPointer(m_attribute_coordinate, 2, GL_FLOAT, GL_FALSE, 0, NULL);
			glDrawArrays(GL_TRIANGLES, 0, DATA_VERTEX_LEN);
			glDisableVertexAttribArray(m_attribute_vertex);
			glDisableVertexAttribArray(m_attribute_coordinate);
		}

		void 
		_craft_test::setup(void)
		{
			craft_gl *inst = NULL;

			if(!m_initialized) {
				THROW_CRAFT_TEST_EXCEPTION(CRAFT_TEST_EXCEPTION_UNINITIALIZED);
			}

			glEnable(GL_DEPTH_TEST);
			glEnable(GL_CULL_FACE);
			glDepthFunc(GL_LESS);
			glPolygonOffset(1, 1);
			glGenVertexArrays(1, &m_array_vertex);
			glBindVertexArray(m_array_vertex);
			inst = craft_gl::acquire();
			m_texture = inst->add_texture(TEXTURE);
			m_shader_fragment = inst->add_shader(SHADER_FRAGMENT, true, GL_FRAGMENT_SHADER);
			m_shader_vertex = inst->add_shader(SHADER_VERTEX, true, GL_VERTEX_SHADER);
			m_program = inst->add_program(m_shader_fragment, m_shader_vertex);
			m_matrix = inst->program_uniform(MVP_UNIFORM, m_program);
			m_attribute_coordinate = inst->program_attribute(ATTRIBUTE_COORDINATE, m_program);
			m_attribute_vertex = inst->program_attribute(ATTRIBUTE_VERTEX, m_program);
			glGenBuffers(1, &m_buffer_vertex);
			glBindBuffer(GL_ARRAY_BUFFER, m_buffer_vertex);
			glBufferData(GL_ARRAY_BUFFER, sizeof(data_vertex), data_vertex, GL_STATIC_DRAW);
			glGenBuffers(1, &m_buffer_fragment);
			glBindBuffer(GL_ARRAY_BUFFER, m_buffer_fragment);
			glBufferData(GL_ARRAY_BUFFER, sizeof(data_coordinate), data_coordinate, GL_STATIC_DRAW);
			glActiveTexture(GL_TEXTURE0);
			glUniform1i(m_texture_uniform, 0);
			glBindTexture(GL_TEXTURE_2D, m_texture);
			glUseProgram(m_program);
		}

		void 
		_craft_test::teardown(void)
		{
			craft_gl *inst = NULL;

			if(!m_initialized) {
				THROW_CRAFT_TEST_EXCEPTION(CRAFT_TEST_EXCEPTION_UNINITIALIZED);
			}

			glDisable(GL_DEPTH_TEST);
			glDisable(GL_CULL_FACE);
			glUseProgram(0);
			glBindVertexArray(0);
			m_matrix = 0;
			m_attribute_coordinate = 0;
			m_attribute_vertex = 0;

			if(m_buffer_fragment) {
				glDeleteBuffers(1, &m_buffer_fragment);
				m_buffer_fragment = 0;
			}

			if(m_buffer_vertex) {
				glDeleteBuffers(1, &m_buffer_vertex);
				m_buffer_vertex = 0;
			}

			if(m_array_vertex) {
				glDeleteVertexArrays(1, &m_array_vertex);
				m_array_vertex = 0;
			}

			inst = craft_gl::acquire();

			if(m_program) {

				if(inst->contains_program(m_program)) {
					inst->decrement_program_reference(m_program);
				} else {
					glDeleteProgram(m_program);
				}

				m_program = 0;
			}

			if(m_shader_fragment) {

				if(inst->contains_shader(m_shader_fragment)) {
					inst->decrement_shader_reference(m_shader_fragment);
				} else {
					glDeleteShader(m_shader_fragment);
				}

				m_shader_fragment = 0;
			}

			if(m_shader_vertex) {

				if(inst->contains_shader(m_shader_vertex)) {
					inst->decrement_shader_reference(m_shader_vertex);
				} else {
					glDeleteShader(m_shader_vertex);
				}

				m_shader_vertex = 0;
			}

			if(m_texture) {

				if(inst->contains_texture(m_texture)) {
					inst->decrement_texture_reference(m_texture);
				} else {
					glDeleteTextures(1, &m_texture);
				}

				m_texture = 0;
			}
		}

		std::string 
		_craft_test::to_string(
			__in_opt bool verbose
			)
		{
			std::stringstream result;

			result << CRAFT_TEST_HEADER << " (" << (m_initialized ? "INITIALIZED" : "UNINITIALIZED");

			if(verbose) {
				result << ", PTR. 0x" << SCALAR_AS_HEX(craft_test *, this);
			}

			result << ")";

			return result.str();
		}

		void 
		_craft_test::uninitialize(void)
		{

			if(!m_initialized) {
				THROW_CRAFT_TEST_EXCEPTION(CRAFT_TEST_EXCEPTION_UNINITIALIZED);
			}

			teardown();
			m_initialized = false;
		}

		void 
		_craft_test::update(
			__in GLfloat delta
			)
		{

			if(!m_initialized) {
				THROW_CRAFT_TEST_EXCEPTION(CRAFT_TEST_EXCEPTION_UNINITIALIZED);
			}

			return;
		}
	}
}

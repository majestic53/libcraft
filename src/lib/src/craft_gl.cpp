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

#include <cstring>
#include <fstream>
#include "../include/craft.h"
#include "../include/craft_gl_type.h"

namespace CRAFT {

	#define BITMAP_FORMAT_RGB 0
	#define BITMAP_FORMAT_RGB_DEPTH 24
	#define BITMAP_FORMAT_RGBA 6
	#define BITMAP_FORMAT_RGBA_DEPTH 32
	#define BITMAP_HEADER "\x42\x4d"
	#define BITMAP_HEADER_OFFSET 0
	#define BITMAP_HEADER_LENGTH 2
	#define BITMAP_INFORMATION_OFFSET 0xe

	STRUCT_PACK(craft_bitmap_header, {
		uint8_t header[BITMAP_HEADER_LENGTH];
		uint32_t length;
		uint32_t reserved;
		uint32_t data;
	});

	STRUCT_PACK(craft_bitmap_information, {
		uint32_t length;
		uint32_t width;
		uint32_t height;
		uint16_t planes;
		uint16_t depth;
		uint32_t compression;
		uint32_t size;
		uint32_t horizontal;
		uint32_t vertical;
		uint32_t palette;
		uint32_t important;
	});

	_craft_gl *_craft_gl::m_instance = NULL;

	_craft_gl::_craft_gl(void) :
		m_initialized(false)
	{
		std::atexit(craft_gl::_delete);
	}

	_craft_gl::~_craft_gl(void)
	{

		if(m_initialized) {
			uninitialize();
		}
	}

	void 
	_craft_gl::_delete(void)
	{

		if(craft_gl::m_instance) {
			delete craft_gl::m_instance;
			craft_gl::m_instance = NULL;
		}
	}

	_craft_gl *
	_craft_gl::acquire(void)
	{

		if(!craft_gl::m_instance) {

			craft_gl::m_instance = new craft_gl;
			if(!craft_gl::m_instance) {
				THROW_CRAFT_GL_EXCEPTION(CRAFT_GL_EXCEPTION_ALLOCATED);
			}
		}

		return craft_gl::m_instance;
	}

	GLuint 
	_craft_gl::add_program(
		__in GLuint fragment,
		__in GLuint vertex
		)
	{
		GLuint result = 0;
		std::string buffer;
		GLint length, status;

		if(!m_initialized) {
			THROW_CRAFT_GL_EXCEPTION(CRAFT_GL_EXCEPTION_UNINITIALIZED);
		}

		result = glCreateProgram();
		if(!result) {
			THROW_CRAFT_GL_EXCEPTION_FORMAT(CRAFT_GL_EXCEPTION_EXTERNAL,
				"%s", "glCreateProgram failed");
		}

		if(contains_shader(fragment)) {
			increment_shader_reference(fragment);
		}

		if(contains_shader(vertex)) {
			increment_shader_reference(vertex);
		}

		glAttachShader(result, fragment);
		glAttachShader(result, vertex);
		glLinkProgram(result);
		glDetachShader(result, fragment);
		glDetachShader(result, vertex);

		glGetProgramiv(result, GL_LINK_STATUS, &status);
		if(status == GL_FALSE) {
			glGetProgramiv(result, GL_INFO_LOG_LENGTH, &length);
			buffer.resize(++length);
			glGetProgramInfoLog(result, length, NULL, (char *) &buffer[0]);
			glDeleteProgram(result);
			THROW_CRAFT_GL_EXCEPTION_FORMAT(CRAFT_GL_EXCEPTION_EXTERNAL,
				"glGetProgramiv failed: %s", buffer.c_str());
		}

		m_program_map.insert(std::pair<GLuint, std::pair<std::pair<GLuint, GLuint>, size_t>>(
			result, std::pair<std::pair<GLuint, GLuint>, size_t>(
			std::pair<GLuint, GLuint>(fragment, vertex), REFERENCE_INITIAL)));

		return result;
	}

	GLuint 
	_craft_gl::add_shader(
		__in const std::string &source,
		__in bool is_file,
		__in GLenum type
		)
	{
		GLuint result = 0;		
		GLint length, status;
		std::string buffer, data;

		if(!m_initialized) {
			THROW_CRAFT_GL_EXCEPTION(CRAFT_GL_EXCEPTION_UNINITIALIZED);
		}

		if(is_file) {

			std::ifstream file(source.c_str(), std::ios::in);
			if(!file) {
				THROW_CRAFT_GL_EXCEPTION_FORMAT(CRAFT_GL_EXCEPTION_FILE_NOT_FOUND,
					"%s", STRING_CHECK(source));
			}

			file.seekg(0, std::ios::end);
			length = file.tellg();
			file.seekg(0, std::ios::beg);
			data.resize(++length);
			file.read((char *) &data[0], length);
			file.close();
		} else {
			data = source;
		}

		result = glCreateShader(type);
		if(!result) {
			THROW_CRAFT_GL_EXCEPTION_FORMAT(CRAFT_GL_EXCEPTION_EXTERNAL,
				"%s", "glCreateShader failed");
		}

		const char *data_addr = data.c_str();
		glShaderSource(result, 1, (const GLchar **) &data_addr, NULL);
		glCompileShader(result);

		glGetShaderiv(result, GL_COMPILE_STATUS, &status);
		if(status == GL_FALSE) {
			glGetShaderiv(result, GL_INFO_LOG_LENGTH, &length);
			buffer.resize(++length);
			glGetShaderInfoLog(result, length, NULL, (char *) &buffer[0]);
			glDeleteShader(result);
			THROW_CRAFT_GL_EXCEPTION_FORMAT(CRAFT_GL_EXCEPTION_EXTERNAL,
				"glGetShaderiv failed: %s", buffer.c_str());
		}

		m_shader_map.insert(std::pair<GLuint, std::pair<GLenum, size_t>>(result, 
			std::pair<GLenum, size_t>(type, REFERENCE_INITIAL)));

		return result;
	}

	GLuint 
	_craft_gl::add_texture(
		__in const std::string &path,
		__in_opt GLint filter,
		__in_opt GLint wrap
		)
	{
		GLint format;
		GLfloat height, width;

		if(!m_initialized) {
			THROW_CRAFT_GL_EXCEPTION(CRAFT_GL_EXCEPTION_UNINITIALIZED);
		}

		return add_texture(craft_gl::bitmap(path, format, width, height), 
			width, height, format, filter, wrap);
	}

	GLuint 
	_craft_gl::add_texture(
		__in const std::vector<uint8_t> &source,
		__in GLfloat width,
		__in GLfloat height,
		__in_opt GLint format,
		__in_opt GLint filter,
		__in_opt GLint wrap
		)
	{
		GLuint result = 0;

		if(!m_initialized) {
			THROW_CRAFT_GL_EXCEPTION(CRAFT_GL_EXCEPTION_UNINITIALIZED);
		}

		glGenTextures(1, &result);
		glBindTexture(GL_TEXTURE_2D, result);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, 
			(uint8_t *) &source[0]);
		glBindTexture(GL_TEXTURE_2D, 0);
		m_texture_map.insert(std::pair<GLuint, std::pair<std::pair<std::pair<GLfloat, GLfloat>, GLint>, size_t>>(
			result, std::pair<std::pair<std::pair<GLfloat, GLfloat>, GLint>, size_t>(
			std::pair<std::pair<GLfloat, GLfloat>, GLint>(std::pair<GLfloat, GLfloat>(width, height), format), 
			REFERENCE_INITIAL)));

		return result;
	}

	std::vector<uint8_t> 
	_craft_gl::bitmap(
		__in const std::string &path,
		__out GLint &format,
		__out GLfloat &width,
		__out GLfloat &height
		)
	{
		int length;
		std::vector<uint8_t> result;
		craft_bitmap_header *head = NULL;
		craft_bitmap_information *info = NULL;

		std::ifstream file(path.c_str(), std::ios::binary | std::ios::in);
		if(!file) {
			THROW_CRAFT_GL_EXCEPTION_FORMAT(CRAFT_GL_EXCEPTION_FILE_NOT_FOUND,
				"%s", STRING_CHECK(path));
		}

		file.seekg(0, std::ios::end);
		length = file.tellg();
		file.seekg(0, std::ios::beg);

		if(length <= 0) {
			file.close();
			THROW_CRAFT_GL_EXCEPTION_FORMAT(CRAFT_GL_EXCEPTION_FILE_EMPTY,
				"%s", STRING_CHECK(path));
		}

		result.resize(length);
		file.read((char *) &result[0], length);
		file.close();

		head = (craft_bitmap_header *) &result[BITMAP_HEADER_OFFSET];
		if(std::memcmp(head, BITMAP_HEADER, BITMAP_HEADER_LENGTH)
				|| (head->data >= head->length)) {
			THROW_CRAFT_GL_EXCEPTION_FORMAT(CRAFT_GL_EXCEPTION_FILE_MALFORMED,
				"%s", STRING_CHECK(path));
		}

		info = (craft_bitmap_information *) &result[BITMAP_INFORMATION_OFFSET];
		if(!info->height || (info->height >= UINT16_MAX) 
				|| !info->width || (info->width > UINT16_MAX)) {
			THROW_CRAFT_GL_EXCEPTION_FORMAT(CRAFT_GL_EXCEPTION_FILE_DIMENSIONS,
				"{%lu, %lu}", info->width, info->height);
		}

		height = info->height;
		width = info->width;

		switch(info->compression) {
			case BITMAP_FORMAT_RGB:
				format = GL_RGB;

				if(info->depth != BITMAP_FORMAT_RGB_DEPTH) {
					THROW_CRAFT_GL_EXCEPTION_FORMAT(CRAFT_GL_EXCEPTION_FILE_DEPTH,
						"%lu:%lu", format, info->depth);
				}
				break;
			case BITMAP_FORMAT_RGBA:
				format = GL_RGBA;

				if(info->depth != BITMAP_FORMAT_RGB_DEPTH) {
					THROW_CRAFT_GL_EXCEPTION_FORMAT(CRAFT_GL_EXCEPTION_FILE_DEPTH,
						"%lu:%lu", format, info->depth);
				}
				break;
			default:
				THROW_CRAFT_GL_EXCEPTION_FORMAT(CRAFT_GL_EXCEPTION_FILE_COMPRESSION,
					"%lu", info->compression);
		}

		return std::vector<uint8_t>(result.begin() + head->data, result.end());
	}

	void 
	_craft_gl::clear(void)
	{
		std::map<GLuint, std::pair<GLenum, size_t>>::iterator shader_iter;
		std::map<GLuint, std::pair<std::pair<GLuint, GLuint>, size_t>>::iterator program_iter;
		std::map<GLuint, std::pair<std::pair<std::pair<GLfloat, GLfloat>, GLint>, size_t>>::iterator texture_iter;

		if(!m_initialized) {
			THROW_CRAFT_GL_EXCEPTION(CRAFT_GL_EXCEPTION_UNINITIALIZED);
		}

		for(program_iter = m_program_map.begin(); program_iter != m_program_map.end();
				++program_iter) {
			glDeleteProgram(program_iter->first);
		}

		for(shader_iter = m_shader_map.begin(); shader_iter != m_shader_map.end();
				++shader_iter) {
			glDeleteShader(shader_iter->first);
		}

		for(texture_iter = m_texture_map.begin(); texture_iter != m_texture_map.end(); 
				++texture_iter) {
			glDeleteTextures(1, &texture_iter->first);
		}

		m_program_map.clear();
		m_shader_map.clear();
		m_texture_map.clear();
	}

	bool 
	_craft_gl::contains_program(
		__in GLuint id
		)
	{

		if(!m_initialized) {
			THROW_CRAFT_GL_EXCEPTION(CRAFT_GL_EXCEPTION_UNINITIALIZED);
		}

		return (m_program_map.find(id) != m_program_map.end());
	}

	bool 
	_craft_gl::contains_shader(
		__in GLuint id
		)
	{

		if(!m_initialized) {
			THROW_CRAFT_GL_EXCEPTION(CRAFT_GL_EXCEPTION_UNINITIALIZED);
		}

		return (m_shader_map.find(id) != m_shader_map.end());
	}

	bool 
	_craft_gl::contains_texture(
		__in GLuint id
		)
	{

		if(!m_initialized) {
			THROW_CRAFT_GL_EXCEPTION(CRAFT_GL_EXCEPTION_UNINITIALIZED);
		}

		return (m_texture_map.find(id) != m_texture_map.end());
	}

	size_t 
	_craft_gl::decrement_program_reference(
		__in GLuint id
		)
	{
		size_t result = 0;
		std::map<GLuint, std::pair<std::pair<GLuint, GLuint>, size_t>>::iterator iter;

		if(!m_initialized) {
			THROW_CRAFT_GL_EXCEPTION(CRAFT_GL_EXCEPTION_UNINITIALIZED);
		}

		iter = find_program(id);
		if(iter->second.second <= REFERENCE_INITIAL) {

			if(contains_shader(iter->second.first.first)) {
				decrement_shader_reference(iter->second.first.first);
			}

			if(contains_shader(iter->second.first.second)) {
				decrement_shader_reference(iter->second.first.second);
			}

			glDeleteProgram(iter->first);
			m_program_map.erase(iter);
		} else {
			result = --iter->second.second;
		}

		return result;
	}

	size_t 
	_craft_gl::decrement_shader_reference(
		__in GLuint id
		)
	{
		size_t result = 0;
		std::map<GLuint, std::pair<GLenum, size_t>>::iterator iter;

		if(!m_initialized) {
			THROW_CRAFT_GL_EXCEPTION(CRAFT_GL_EXCEPTION_UNINITIALIZED);
		}

		iter = find_shader(id);
		if(iter->second.second <= REFERENCE_INITIAL) {
			glDeleteShader(iter->first);
			m_shader_map.erase(iter);
		} else {
			result = --iter->second.second;
		}

		return result;
	}

	size_t 
	_craft_gl::decrement_texture_reference(
		__in GLuint id
		)
	{
		size_t result = 0;
		std::map<GLuint, std::pair<std::pair<std::pair<GLfloat, GLfloat>, GLint>, size_t>>::iterator iter;

		if(!m_initialized) {
			THROW_CRAFT_GL_EXCEPTION(CRAFT_GL_EXCEPTION_UNINITIALIZED);
		}

		iter = find_texture(id);
		if(iter->second.second <= REFERENCE_INITIAL) {
			glDeleteTextures(1, &iter->first);
			m_texture_map.erase(iter);
		} else {
			result = --iter->second.second;
		}

		return result;
	}

	std::map<GLuint, std::pair<std::pair<GLuint, GLuint>, size_t>>::iterator 
	_craft_gl::find_program(
		__in GLuint id
		)
	{
		std::map<GLuint, std::pair<std::pair<GLuint, GLuint>, size_t>>::iterator result;

		if(!m_initialized) {
			THROW_CRAFT_GL_EXCEPTION(CRAFT_GL_EXCEPTION_UNINITIALIZED);
		}

		result = m_program_map.find(id);
		if(result == m_program_map.end()) {
			THROW_CRAFT_GL_EXCEPTION_FORMAT(CRAFT_GL_EXCEPTION_PROGRAM_NOT_FOUND,
				"0x%x", id);
		}

		return result;
	}

	std::map<GLuint, std::pair<GLenum, size_t>>::iterator 
	_craft_gl::find_shader(
		__in GLuint id
		)
	{
		std::map<GLuint, std::pair<GLenum, size_t>>::iterator result;

		if(!m_initialized) {
			THROW_CRAFT_GL_EXCEPTION(CRAFT_GL_EXCEPTION_UNINITIALIZED);
		}

		result = m_shader_map.find(id);
		if(result == m_shader_map.end()) {
			THROW_CRAFT_GL_EXCEPTION_FORMAT(CRAFT_GL_EXCEPTION_SHADER_NOT_FOUND,
				"0x%x", id);
		}

		return result;
	}

	std::map<GLuint, std::pair<std::pair<std::pair<GLfloat, GLfloat>, GLint>, size_t>>::iterator 
	_craft_gl::find_texture(
		__in GLuint id
		)
	{
		std::map<GLuint, std::pair<std::pair<std::pair<GLfloat, GLfloat>, GLint>, size_t>>::iterator result;

		if(!m_initialized) {
			THROW_CRAFT_GL_EXCEPTION(CRAFT_GL_EXCEPTION_UNINITIALIZED);
		}

		result = m_texture_map.find(id);
		if(result == m_texture_map.end()) {
			THROW_CRAFT_GL_EXCEPTION_FORMAT(CRAFT_GL_EXCEPTION_TEXTURE_NOT_FOUND,
				"0x%x", id);
		}

		return result;
	}

	size_t 
	_craft_gl::increment_program_reference(
		__in GLuint id
		)
	{

		if(!m_initialized) {
			THROW_CRAFT_GL_EXCEPTION(CRAFT_GL_EXCEPTION_UNINITIALIZED);
		}

		return ++find_program(id)->second.second;
	}

	size_t 
	_craft_gl::increment_shader_reference(
		__in GLuint id
		)
	{

		if(!m_initialized) {
			THROW_CRAFT_GL_EXCEPTION(CRAFT_GL_EXCEPTION_UNINITIALIZED);
		}

		return ++find_shader(id)->second.second;
	}

	size_t 
	_craft_gl::increment_texture_reference(
		__in GLuint id
		)
	{

		if(!m_initialized) {
			THROW_CRAFT_GL_EXCEPTION(CRAFT_GL_EXCEPTION_UNINITIALIZED);
		}

		return ++find_texture(id)->second.second;
	}

	void 
	_craft_gl::initialize(void)
	{

		if(m_initialized) {
			THROW_CRAFT_GL_EXCEPTION(CRAFT_GL_EXCEPTION_INITIALIZED);
		}

		m_initialized = true;
	}

	void 
	_craft_gl::initialize_external(
		__in uint32_t version
		)
	{
		GLenum error;

		glewExperimental = GL_TRUE;

		error = glewInit();
		if(error != GLEW_OK) {
			THROW_CRAFT_GL_EXCEPTION_FORMAT(CRAFT_GL_EXCEPTION_EXTERNAL,
				"glewInit failed: %s", glewGetErrorString(error));
		}

		if(!GL_VERSION_VALID()) {
			THROW_CRAFT_GL_EXCEPTION_FORMAT(CRAFT_GL_EXCEPTION_EXTERNAL,
				"OpenGL version unsupported: %s", glGetString(GL_VERSION));
		}
	}

	bool 
	_craft_gl::is_allocated(void)
	{
		return (craft_gl::m_instance != NULL);
	}

	bool 
	_craft_gl::is_initialized(void)
	{
		return m_initialized;
	}

	GLint 
	_craft_gl::program_attribute(
		__in const std::string &name,
		__in GLuint id
		)
	{
		GLint result;

		if(!m_initialized) {
			THROW_CRAFT_GL_EXCEPTION(CRAFT_GL_EXCEPTION_UNINITIALIZED);
		}

		result = glGetAttribLocation(id, name.c_str());
		if(result == SCALAR_INVALID(int)) {
			THROW_CRAFT_GL_EXCEPTION_FORMAT(CRAFT_GL_EXCEPTION_PROGRAM_ATTRIBUTE_NOT_FOUND,
				"0x%x:%s", id, STRING_CHECK(name));
		}

		return result;
	}

	size_t 
	_craft_gl::program_count(void)
	{

		if(!m_initialized) {
			THROW_CRAFT_GL_EXCEPTION(CRAFT_GL_EXCEPTION_UNINITIALIZED);
		}

		return m_program_map.size();
	}

	size_t 
	_craft_gl::program_reference(
		__in GLuint id
		)
	{

		if(!m_initialized) {
			THROW_CRAFT_GL_EXCEPTION(CRAFT_GL_EXCEPTION_UNINITIALIZED);
		}

		return find_program(id)->second.second;
	}

	std::pair<GLuint, GLuint> 
	_craft_gl::program_shaders(
		__in GLuint id
		)
	{

		if(!m_initialized) {
			THROW_CRAFT_GL_EXCEPTION(CRAFT_GL_EXCEPTION_UNINITIALIZED);
		}

		return find_program(id)->second.first;
	}

	GLint 
	_craft_gl::program_uniform(
		__in const std::string &name,
		__in GLuint id
		)
	{
		GLint result;

		if(!m_initialized) {
			THROW_CRAFT_GL_EXCEPTION(CRAFT_GL_EXCEPTION_UNINITIALIZED);
		}

		result = glGetUniformLocation(id, name.c_str());
		if(result == SCALAR_INVALID(int)) {
			THROW_CRAFT_GL_EXCEPTION_FORMAT(CRAFT_GL_EXCEPTION_PROGRAM_UNIFORM_NOT_FOUND,
				"0x%x:%s", id, STRING_CHECK(name));
		}

		return result;
	}

	size_t 
	_craft_gl::shader_count(void)
	{

		if(!m_initialized) {
			THROW_CRAFT_GL_EXCEPTION(CRAFT_GL_EXCEPTION_UNINITIALIZED);
		}

		return m_shader_map.size();
	}

	size_t 
	_craft_gl::shader_reference(
		__in GLuint id
		)
	{

		if(!m_initialized) {
			THROW_CRAFT_GL_EXCEPTION(CRAFT_GL_EXCEPTION_UNINITIALIZED);
		}

		return find_shader(id)->second.second;
	}

	GLenum 
	_craft_gl::shader_type(
		__in GLuint id
		)
	{

		if(!m_initialized) {
			THROW_CRAFT_GL_EXCEPTION(CRAFT_GL_EXCEPTION_UNINITIALIZED);
		}

		return find_shader(id)->second.first;
	}

	size_t 
	_craft_gl::texture_count(void)
	{

		if(!m_initialized) {
			THROW_CRAFT_GL_EXCEPTION(CRAFT_GL_EXCEPTION_UNINITIALIZED);
		}

		return m_texture_map.size();
	}

	std::pair<GLfloat, GLfloat> 
	_craft_gl::texture_dimensions(
		__in GLuint id
		)
	{

		if(!m_initialized) {
			THROW_CRAFT_GL_EXCEPTION(CRAFT_GL_EXCEPTION_UNINITIALIZED);
		}

		return find_texture(id)->second.first.first;
	}

	size_t 
	_craft_gl::texture_reference(
		__in GLuint id
		)
	{

		if(!m_initialized) {
			THROW_CRAFT_GL_EXCEPTION(CRAFT_GL_EXCEPTION_UNINITIALIZED);
		}

		return find_texture(id)->second.second;
	}

	std::string 
	_craft_gl::to_string(
		__in_opt bool verbose
		)
	{
		size_t count;
		std::stringstream result;
		std::map<GLuint, std::pair<GLenum, size_t>>::iterator shader_iter;
		std::map<GLuint, std::pair<std::pair<GLuint, GLuint>, size_t>>::iterator program_iter;
		std::map<GLuint, std::pair<std::pair<std::pair<GLfloat, GLfloat>, GLint>, size_t>>::iterator texture_iter;

		result << CRAFT_GL_HEADER << " (" << (m_initialized ? "INITIALIZED" : "UNINITIALIZE");

		if(verbose) {
			result << ", PTR. 0x" << SCALAR_AS_HEX(craft_gl *, this);
		}

		if(m_initialized) {
			result << ", SHAD. " << m_shader_map.size() << ", PROG. " << m_program_map.size();
		}

		result << ")";

		if(m_initialized) {

			if(!m_shader_map.empty()) {

				for(count = 0, shader_iter = m_shader_map.begin(); shader_iter != m_shader_map.end();
						++count, ++shader_iter) {
					std::cout << std::endl << "SHAD[" << count << "] {" 
						<< SCALAR_AS_HEX(GLuint, shader_iter->first) 
						<< "}, TYPE. " << shader_iter->second.first 
						<< ", REF. " << shader_iter->second.second;
				}

				for(count = 0, program_iter = m_program_map.begin(); program_iter != m_program_map.end();
						++count, ++program_iter) {
					std::cout << std::endl << "PROG[" << count << "] {" 
						<< SCALAR_AS_HEX(GLuint, program_iter->first) 
						<< "}, SHAD. {" << program_iter->second.first.first
						<< ", " << program_iter->second.first.second
						<< "}, REF. " << program_iter->second.second;
				}

				for(count = 0, texture_iter = m_texture_map.begin(); texture_iter != m_texture_map.end();
						++count, ++texture_iter) {
					std::cout << std::endl << "TEX[" << count << "] {" 
						<< SCALAR_AS_HEX(GLuint, texture_iter->first) 
						<< "}, DIM. {" << texture_iter->second.first.first.first
						<< ", " << texture_iter->second.first.first.second
						<< "}, FMT. " << texture_iter->second.first.second
						<< ", REF. " << texture_iter->second.second;
				}
			}
		}

		return result.str();
	}

	void 
	_craft_gl::uninitialize(void)
	{

		if(!m_initialized) {
			THROW_CRAFT_GL_EXCEPTION(CRAFT_GL_EXCEPTION_UNINITIALIZED);
		}

		clear();
		m_initialized = false;
	}
}

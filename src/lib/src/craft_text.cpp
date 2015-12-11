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
#include "../include/craft_text_type.h"

namespace CRAFT {

	namespace COMPONENT {

		_craft_text *_craft_text::m_instance = NULL;

		_craft_text::_craft_text(void) :
			m_initialized(false),
			m_library(NULL),
			m_next_id(0),
			m_texture(0),
			m_vertex_buffer(0)
		{
			std::atexit(craft_text::_delete);
		}

		_craft_text::~_craft_text(void)
		{

			if(m_initialized) {
				uninitialize();
			}
		}

		void 
		_craft_text::_delete(void)
		{

			if(craft_text::m_instance) {
				delete craft_text::m_instance;
				craft_text::m_instance = NULL;
			}
		}

		_craft_text *
		_craft_text::acquire(void)
		{

			if(!craft_text::m_instance) {

				craft_text::m_instance = new craft_text;
				if(!craft_text::m_instance) {
					THROW_CRAFT_TEXT_EXCEPTION(CRAFT_TEXT_EXCEPTION_ALLOCATED);
				}
			}

			return craft_text::m_instance;
		}

		craft_font 
		_craft_text::add_face(
			__in const std::string &path,
			__in size_t size
			)
		{
			FT_Face face = NULL;
			craft_font result = 0;
			FT_Error extern_result;

			if(!m_initialized) {
				THROW_CRAFT_TEXT_EXCEPTION(CRAFT_TEXT_EXCEPTION_UNINITIALIZED);
			}

			if(!size) {
				THROW_CRAFT_TEXT_EXCEPTION_FORMAT(CRAFT_TEXT_EXCEPTION_INVALID_SIZE,
					"%lu", size);
			}

			extern_result = FT_New_Face(m_library, path.c_str(), 0, &face);
			if(extern_result != FT_Err_Ok) {
				THROW_CRAFT_TEXT_EXCEPTION_FORMAT(CRAFT_TEXT_EXCEPTION_EXTERNAL,
					"FT_New_Face failed: 0x%x", extern_result);
			}

			extern_result = FT_Set_Pixel_Sizes(face, 0, size);
			if(extern_result != FT_Err_Ok) {
				THROW_CRAFT_TEXT_EXCEPTION_FORMAT(CRAFT_TEXT_EXCEPTION_EXTERNAL,
					"FT_Set_Pixel_Sizes failed: 0x%x", extern_result);
			}

			result = reserve_id();
			m_face_map.insert(std::pair<craft_font, std::pair<std::pair<FT_Face, std::pair<std::string, size_t>>, size_t>>(
				result, std::pair<std::pair<FT_Face, std::pair<std::string, size_t>>, size_t>(
				std::pair<FT_Face, std::pair<std::string, size_t>>(face, std::pair<std::string, size_t>(path, size)), 
				REFERENCE_INITIAL)));

			return result;
		}

		void 
		_craft_text::clear(void)
		{
			craft_gl *inst = NULL;
			FT_Error extern_result;
			std::map<craft_font, std::pair<std::pair<FT_Face, std::pair<std::string, size_t>>, size_t>>::iterator face_iter;

			if(!m_initialized) {
				THROW_CRAFT_TEXT_EXCEPTION(CRAFT_TEXT_EXCEPTION_UNINITIALIZED);
			}

			if(m_library) {

				for(face_iter = m_face_map.begin(); face_iter != m_face_map.end();
						++face_iter) {

					extern_result = FT_Done_Face(face_iter->second.first.first);
					if(extern_result != FT_Err_Ok) {
						THROW_CRAFT_TEXT_EXCEPTION_FORMAT(CRAFT_TEXT_EXCEPTION_EXTERNAL,
							"FT_Done_Face failed: 0x%x", extern_result);
					}
				}
			}

			inst = craft_gl::acquire();

			if(m_texture) {

				if(inst->contains_texture(m_texture)) {
					inst->decrement_texture_reference(m_texture);
				} else {
					glDeleteTextures(1, &m_texture);
				}

				m_texture = 0;
			}

			if(m_vertex_buffer) {
				glDeleteBuffers(1, &m_vertex_buffer);
				m_vertex_buffer = 0;
			}

			m_next_id = 0;
			m_surplus_id.clear();
			m_face_map.clear();
		}

		bool 
		_craft_text::contains_face_reference(
			__in const craft_font &id
			)
		{

			if(!m_initialized) {
				THROW_CRAFT_TEXT_EXCEPTION(CRAFT_TEXT_EXCEPTION_UNINITIALIZED);
			}

			return (m_face_map.find(id) != m_face_map.end());
		}

		size_t 
		_craft_text::decrement_face_reference(
			__in const craft_font &id
			)
		{
			size_t result = 0;
			FT_Error extern_result;
			std::map<craft_font, std::pair<std::pair<FT_Face, std::pair<std::string, size_t>>, size_t>>::iterator iter;

			if(!m_initialized) {
				THROW_CRAFT_TEXT_EXCEPTION(CRAFT_TEXT_EXCEPTION_UNINITIALIZED);
			}

			iter = find_face(id);
			if(iter->second.second <= REFERENCE_INITIAL) {

				extern_result = FT_Done_Face(iter->second.first.first);
				if(extern_result != FT_Err_Ok) {
					THROW_CRAFT_TEXT_EXCEPTION_FORMAT(CRAFT_TEXT_EXCEPTION_EXTERNAL,
						"FT_Done_Face failed: 0x%x", extern_result);
				}

				retire_id(iter->first);
				m_face_map.erase(iter);
			} else {
				result = --iter->second.second;
			}

			return result;
		}

		size_t 
		_craft_text::face_count(void)
		{

			if(!m_initialized) {
				THROW_CRAFT_TEXT_EXCEPTION(CRAFT_TEXT_EXCEPTION_UNINITIALIZED);
			}

			return m_face_map.size();
		}

		std::string 
		_craft_text::face_path(
			__in const craft_font &id
			)
		{

			if(!m_initialized) {
				THROW_CRAFT_TEXT_EXCEPTION(CRAFT_TEXT_EXCEPTION_UNINITIALIZED);
			}

			return find_face(id)->second.first.second.first;
		}

		size_t 
		_craft_text::face_reference(
			__in const craft_font &id
			)
		{

			if(!m_initialized) {
				THROW_CRAFT_TEXT_EXCEPTION(CRAFT_TEXT_EXCEPTION_UNINITIALIZED);
			}

			return find_face(id)->second.second;
		}

		size_t 
		_craft_text::face_size(
			__in const craft_font &id
			)
		{

			if(!m_initialized) {
				THROW_CRAFT_TEXT_EXCEPTION(CRAFT_TEXT_EXCEPTION_UNINITIALIZED);
			}

			return find_face(id)->second.first.second.second;
		}

		std::map<craft_font, std::pair<std::pair<FT_Face, std::pair<std::string, size_t>>, size_t>>::iterator 
		_craft_text::find_face(
			__in const craft_font &id
			)
		{
			std::map<craft_font, std::pair<std::pair<FT_Face, std::pair<std::string, size_t>>, size_t>>::iterator result;

			if(!m_initialized) {
				THROW_CRAFT_TEXT_EXCEPTION(CRAFT_TEXT_EXCEPTION_UNINITIALIZED);
			}

			result = m_face_map.find(id);
			if(result == m_face_map.end()) {
				THROW_CRAFT_TEXT_EXCEPTION_FORMAT(CRAFT_TEXT_EXCEPTION_FACE_NOT_FOUND,
					"0x%x", id);
			}

			return result;
		}

		size_t 
		_craft_text::increment_face_reference(
			__in const craft_font &id
			)
		{

			if(!m_initialized) {
				THROW_CRAFT_TEXT_EXCEPTION(CRAFT_TEXT_EXCEPTION_UNINITIALIZED);
			}

			return ++find_face(id)->second.second;
		}

		void 
		_craft_text::initialize(void)
		{
			FT_Error result;

			if(m_initialized) {
				THROW_CRAFT_TEXT_EXCEPTION(CRAFT_TEXT_EXCEPTION_INITIALIZED);
			}

			result = FT_Init_FreeType(&m_library);
			if(result != FT_Err_Ok) {
				THROW_CRAFT_TEXT_EXCEPTION_FORMAT(CRAFT_TEXT_EXCEPTION_EXTERNAL,
					"FT_Init_FreeType failed: 0x%x", result);
			}
			
			// TODO: allocate texture using craft_gl
			// TODO: allocate vertex buffer

			m_initialized = true;
			clear();
			setup();
		}

		bool 
		_craft_text::is_allocated(void)
		{
			return (craft_text::m_instance != NULL);
		}

		bool 
		_craft_text::is_initialized(void)
		{
			return m_initialized;
		}

		FT_GlyphSlot 
		_craft_text::load_glyph(
			__in const craft_font &id,
			__in FT_ULong character,
			__in_opt FT_Int32 flag
			)
		{
			FT_Face face;
			FT_Error result;

			if(!m_initialized) {
				THROW_CRAFT_TEXT_EXCEPTION(CRAFT_TEXT_EXCEPTION_UNINITIALIZED);
			}

			face = find_face(id)->second.first.first;

			result = FT_Load_Char(face, character, flag);
			if(result != FT_Err_Ok) {
				THROW_CRAFT_TEXT_EXCEPTION_FORMAT(CRAFT_TEXT_EXCEPTION_EXTERNAL,
					"FT_Load_Char failed: 0x%x", result);
			}

			return face->glyph;
		}

		void 
		_craft_text::render(
			__in const glm::mat4 &mvp
			)
		{

			if(!m_initialized) {
				THROW_CRAFT_TEXT_EXCEPTION(CRAFT_TEXT_EXCEPTION_UNINITIALIZED);
			}

			// TODO
		}

		craft_font 
		_craft_text::reserve_id(void)
		{
			craft_font result = 0;
			std::set<craft_font>::iterator iter;

			if(!m_initialized) {
				THROW_CRAFT_TEXT_EXCEPTION(CRAFT_TEXT_EXCEPTION_UNINITIALIZED);
			}

			if(!m_surplus_id.empty()) {
				iter = m_surplus_id.begin();
				result = *iter;
				m_surplus_id.erase(iter);
			} else if(m_next_id < UINT32_MAX) {
				result = m_next_id++;
			} else {
				THROW_CRAFT_TEXT_EXCEPTION(CRAFT_TEXT_EXCEPTION_ID_FULL);
			}

			return result;
		}

		void 
		_craft_text::retire_id(
			__in const craft_font &id
			)
		{

			if(!m_initialized) {
				THROW_CRAFT_TEXT_EXCEPTION(CRAFT_TEXT_EXCEPTION_UNINITIALIZED);
			}

			m_surplus_id.insert(id);
		}

		void 
		_craft_text::set_face_size(
			__in const craft_font &id,
			__in size_t size
			)
		{
			FT_Error extern_result;
			std::map<craft_font, std::pair<std::pair<FT_Face, std::pair<std::string, size_t>>, size_t>>::iterator iter;

			if(!m_initialized) {
				THROW_CRAFT_TEXT_EXCEPTION(CRAFT_TEXT_EXCEPTION_UNINITIALIZED);
			}

			if(!size) {
				THROW_CRAFT_TEXT_EXCEPTION_FORMAT(CRAFT_TEXT_EXCEPTION_INVALID_SIZE,
					"%lu", size);
			}

			iter = find_face(id);
			iter->second.first.second.second = size;

			extern_result = FT_Set_Pixel_Sizes(iter->second.first.first, 0, size);
			if(extern_result != FT_Err_Ok) {
				THROW_CRAFT_TEXT_EXCEPTION_FORMAT(CRAFT_TEXT_EXCEPTION_EXTERNAL,
					"FT_Set_Pixel_Sizes failed: 0x%x", extern_result);
			}
		}

		void 
		_craft_text::setup(void)
		{

			if(!m_initialized) {
				THROW_CRAFT_TEXT_EXCEPTION(CRAFT_TEXT_EXCEPTION_UNINITIALIZED);
			}

			// TODO: crete texture/vertex buffer
		}

		void 
		_craft_text::teardown(void)
		{
			craft_gl *inst = NULL;

			if(!m_initialized) {
				THROW_CRAFT_TEXT_EXCEPTION(CRAFT_TEXT_EXCEPTION_UNINITIALIZED);
			}

			if(m_texture) {

				if(inst->contains_texture(m_texture)) {
					inst->decrement_texture_reference(m_texture);
				} else {
					glDeleteTextures(1, &m_texture);
				}

				m_texture = 0;
			}

			if(m_vertex_buffer) {
				glDeleteBuffers(1, &m_vertex_buffer);
				m_vertex_buffer = 0;
			}
		}

		std::string 
		_craft_text::to_string(
			__in_opt bool verbose
			)
		{
			size_t count;
			std::stringstream result;
			std::map<craft_font, std::pair<std::pair<FT_Face, std::pair<std::string, size_t>>, size_t>>::iterator face_iter;

			result << CRAFT_TEXT_HEADER << " (" << (m_initialized ? "INITIALIZED" : "UNINITIALIZED");

			if(verbose) {
				result << ", PTR. 0x" << SCALAR_AS_HEX(craft_text *, this);
			}

			if(m_initialized) {
				result << ", FACE. " << m_face_map.size();
			}

			result << ")";

			if(m_initialized) {

				if(!m_face_map.empty()) {

					for(count = 0, face_iter = m_face_map.begin(); face_iter != m_face_map.end(); 
							++count, ++face_iter) {
						result << std::endl << "FACE[" << count << "], (0x" 
							<< SCALAR_AS_HEX(craft_font, face_iter->first) << ":0x" 
							<< SCALAR_AS_HEX(FT_Face, face_iter->second.first.first) << ")\"" 
							<< face_iter->second.first.second.first << "\", SIZE. "
							<< face_iter->second.first.second.second << ", REF. "
							<< face_iter->second.second;
					}
				}
			}

			return result.str();
		}

		void 
		_craft_text::uninitialize(void)
		{
			FT_Error result;

			if(!m_initialized) {
				THROW_CRAFT_TEXT_EXCEPTION(CRAFT_TEXT_EXCEPTION_UNINITIALIZED);
			}

			teardown();
			clear();

			if(m_library) {

				result = FT_Done_FreeType(m_library);
				if(result != FT_Err_Ok) {
					THROW_CRAFT_TEXT_EXCEPTION_FORMAT(CRAFT_TEXT_EXCEPTION_EXTERNAL,
						"FT_Done_FreeType failed: 0x%x", result);
				}

				m_library = NULL;
			}

			m_initialized = false;
		}
	}
}

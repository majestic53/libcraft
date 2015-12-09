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
			m_library(NULL)
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

		void 
		_craft_text::clear(void)
		{

			if(!m_initialized) {
				THROW_CRAFT_TEXT_EXCEPTION(CRAFT_TEXT_EXCEPTION_UNINITIALIZED);
			}

			// TODO
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
					"%s", "FT_Init_FreeType failed: 0x%x", result);
			}

			// TODO

			m_initialized = true;
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

		void 
		_craft_text::render(void)
		{

			if(!m_initialized) {
				THROW_CRAFT_TEXT_EXCEPTION(CRAFT_TEXT_EXCEPTION_UNINITIALIZED);
			}

			// TODO
		}

		std::string 
		_craft_text::to_string(
			__in_opt bool verbose
			)
		{
			std::stringstream result;

			result << CRAFT_TEXT_HEADER << " (" << (m_initialized ? "INITIALIZED" : "UNINITIALIZED");

			if(verbose) {
				result << ", PTR. 0x" << SCALAR_AS_HEX(craft_text *, this);
			}

			result << ")";

			return result.str();
		}

		void 
		_craft_text::uninitialize(void)
		{
			FT_Error result;

			if(!m_initialized) {
				THROW_CRAFT_TEXT_EXCEPTION(CRAFT_TEXT_EXCEPTION_UNINITIALIZED);
			}

			// TODO

			if(m_library) {

				result = FT_Done_FreeType(m_library);
				if(result != FT_Err_Ok) {
					THROW_CRAFT_TEXT_EXCEPTION_FORMAT(CRAFT_TEXT_EXCEPTION_EXTERNAL,
						"%s", "FT_Done_FreeType failed: 0x%x", result);
				}

				m_library = NULL;
			}

			m_initialized = false;
		}
	}
}

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
#include "../include/craft_display_type.h"

namespace CRAFT {

	namespace COMPONENT {

		_craft_display *_craft_display::m_instance = NULL;

		_craft_display::_craft_display(void) :
			m_context(NULL),
			m_initialized(false),
			m_running(false),
			m_window(NULL)
		{
			std::atexit(craft_display::_delete);
		}

		_craft_display::~_craft_display(void)
		{

			if(m_initialized) {
				uninitialize();
			}
		}

		void 
		_craft_display::_delete(void)
		{

			if(craft_display::m_instance) {
				delete craft_display::m_instance;
				craft_display::m_instance = NULL;
			}
		}

		_craft_display *
		_craft_display::acquire(void)
		{

			if(!craft_display::m_instance) {

				craft_display::m_instance = new craft_display;
				if(!craft_display::m_instance) {
					THROW_CRAFT_DISPLAY_EXCEPTION(CRAFT_DISPLAY_EXCEPTION_ALLOCATED);
				}
			}

			return craft_display::m_instance;
		}

		void 
		_craft_display::clear(void)
		{

			if(!m_initialized) {
				THROW_CRAFT_DISPLAY_EXCEPTION(CRAFT_DISPLAY_EXCEPTION_UNINITIALIZED);
			}

			if(m_running) {
				THROW_CRAFT_DISPLAY_EXCEPTION(CRAFT_DISPLAY_EXCEPTION_STARTED);
			}
		}

		SDL_GLContext 
		_craft_display::context(void)
		{

			if(!m_initialized) {
				THROW_CRAFT_DISPLAY_EXCEPTION(CRAFT_DISPLAY_EXCEPTION_UNINITIALIZED);
			}

			if(!m_running) {
				THROW_CRAFT_DISPLAY_EXCEPTION(CRAFT_DISPLAY_EXCEPTION_STOPPED);
			}

			return m_context;
		}

		void 
		_craft_display::initialize(void)
		{

			if(m_initialized) {
				THROW_CRAFT_DISPLAY_EXCEPTION(CRAFT_DISPLAY_EXCEPTION_INITIALIZED);
			}

			if(m_running) {
				THROW_CRAFT_DISPLAY_EXCEPTION(CRAFT_DISPLAY_EXCEPTION_STARTED);
			}

			m_initialized = true;
		}

		bool 
		_craft_display::is_allocated(void)
		{
			return (craft_display::m_instance != NULL);
		}

		bool 
		_craft_display::is_initialized(void)
		{
			return m_initialized;
		}

		bool 
		_craft_display::is_running(void)
		{
			return m_running;
		}

		void 
		_craft_display::on_event(
			__in const SDL_WindowEvent &event
			)
		{
			return;
		}

		void 
		_craft_display::setup(
			__in const std::string &title,
			__in size_t left,
			__in size_t top,
			__in size_t width,
			__in size_t height,
			__in uint32_t flags
			)
		{

			if(!m_initialized) {
				THROW_CRAFT_DISPLAY_EXCEPTION(CRAFT_DISPLAY_EXCEPTION_UNINITIALIZED);
			}

			if(!m_running) {
				THROW_CRAFT_DISPLAY_EXCEPTION(CRAFT_DISPLAY_EXCEPTION_STOPPED);
			}

			if(flags & SDL_WINDOW_FULLSCREEN) {
				validate_dimensions(width, height);
			}

			m_window = SDL_CreateWindow(title.c_str(), left, top, width, height, flags);
			if(!m_window) {
				THROW_CRAFT_DISPLAY_EXCEPTION_FORMAT(CRAFT_DISPLAY_EXCEPTION_EXTERNAL,
					"SDL_CreateWindow failed: %s", SDL_GetError());
			}

			m_context = SDL_GL_CreateContext(m_window);
			if(!m_context) {
				THROW_CRAFT_DISPLAY_EXCEPTION_FORMAT(CRAFT_DISPLAY_EXCEPTION_EXTERNAL,
					"SDL_CreateWindow failed: %s", SDL_GetError());
			}

			SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, DISPLAY_ACCELERATE_VISUAL);
			SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, DISPLAY_DEPTH_SIZE);
			SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, DISPLAY_DOUBLE_BUFFER);
			SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, DISPLAY_MAJOR_VERSION);
			SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, DISPLAY_MINOR_VERSION);
			SDL_GL_SetSwapInterval(DISPLAY_SWAP_INTERVAL);
		}

		void 
		_craft_display::start(
			__in const std::string &title,
			__in size_t left,
			__in size_t top,
			__in size_t width,
			__in size_t height,
			__in uint32_t flags
			)
		{

			if(!m_initialized) {
				THROW_CRAFT_DISPLAY_EXCEPTION(CRAFT_DISPLAY_EXCEPTION_UNINITIALIZED);
			}

			if(m_running) {
				THROW_CRAFT_DISPLAY_EXCEPTION(CRAFT_DISPLAY_EXCEPTION_STARTED);
			}

			m_running = true;
			setup(title, left, top, width, height, flags);
		}

		void 
		_craft_display::stop(void)
		{

			if(!m_initialized) {
				THROW_CRAFT_DISPLAY_EXCEPTION(CRAFT_DISPLAY_EXCEPTION_UNINITIALIZED);
			}

			if(!m_running) {
				THROW_CRAFT_DISPLAY_EXCEPTION(CRAFT_DISPLAY_EXCEPTION_STOPPED);
			}

			teardown();
			m_running = false;
		}

		void 
		_craft_display::teardown(void)
		{

			if(!m_initialized) {
				THROW_CRAFT_DISPLAY_EXCEPTION(CRAFT_DISPLAY_EXCEPTION_UNINITIALIZED);
			}

			if(!m_running) {
				THROW_CRAFT_DISPLAY_EXCEPTION(CRAFT_DISPLAY_EXCEPTION_STOPPED);
			}

			if(m_context) {
				SDL_GL_DeleteContext(m_context);
				m_context = NULL;
			}

			if(m_window) {
				SDL_DestroyWindow(m_window);
				m_window = NULL;
			}
		}

		std::string 
		_craft_display::to_string(
			__in_opt bool verbose
			)
		{
			std::stringstream result;

			result << CRAFT_DISPLAY_HEADER << " (" << (m_initialized ? "INITIALIZED" : "UNINITIALIZED")
				<< ", " << (m_running ? "STARTED" : "STOPPED");

			if(verbose) {
				result << ", PTR. 0x" << SCALAR_AS_HEX(craft_display *, this);
			}

			if(m_initialized) {
				result << ", WIN. 0x" << SCALAR_AS_HEX(SDL_Window *, m_window) 
					<< ", CONT. 0x" << SCALAR_AS_HEX(SDL_GLContext, m_context);
			}

			result << ")";

			return result.str();
		}

		void 
		_craft_display::uninitialize(void)
		{

			if(!m_initialized) {
				THROW_CRAFT_DISPLAY_EXCEPTION(CRAFT_DISPLAY_EXCEPTION_UNINITIALIZED);
			}

			if(m_running) {
				stop();
			}

			clear();
			m_initialized = false;
		}

		std::map<int, std::vector<std::pair<int, int>>> 
		_craft_display::supported_resolutions(void)
		{
			SDL_DisplayMode mode;
			std::vector<std::pair<int, int>> modes;
			int disp_count, disp_iter = 0, mode_count, mode_iter;
			std::map<int, std::vector<std::pair<int, int>>> result;

			disp_count = SDL_GetNumVideoDisplays();
			if(!disp_count) {
				THROW_CRAFT_DISPLAY_EXCEPTION(CRAFT_DISPLAY_EXCEPTION_MISSING);
			} else if(disp_count < 0) {
				THROW_CRAFT_DISPLAY_EXCEPTION_FORMAT(CRAFT_DISPLAY_EXCEPTION_EXTERNAL,
					"SDL_GetNumVideoDisplays failed: %s", SDL_GetError());
			}

			for(; disp_iter < disp_count; ++disp_iter) {

				mode_count = SDL_GetNumDisplayModes(disp_iter);
				if(!mode_count) {
					THROW_CRAFT_DISPLAY_EXCEPTION_FORMAT(CRAFT_DISPLAY_EXCEPTION_MISSING_MODE, 
						"%lu", disp_iter);
				} else if(mode_count < 0) {
					THROW_CRAFT_DISPLAY_EXCEPTION_FORMAT(CRAFT_DISPLAY_EXCEPTION_EXTERNAL,
						"SDL_GetNumDisplayModes failed: %s", SDL_GetError());
				}

				modes.clear();

				for(mode_iter = 0; mode_iter < mode_count; ++mode_iter) {

					if(SDL_GetDisplayMode(disp_iter, mode_iter, &mode)) {
						THROW_CRAFT_DISPLAY_EXCEPTION_FORMAT(CRAFT_DISPLAY_EXCEPTION_EXTERNAL,
							"SDL_GetDisplayMode failed: %s", SDL_GetError());
					}

					modes.push_back(std::pair<int, int>(mode.w, mode.h));
				}

				result.insert(std::pair<int, std::vector<std::pair<int, int>>>(disp_iter, modes));
			}

			return result;
		}

		void 
		_craft_display::validate_dimensions(
			__inout size_t &width,
			__inout size_t &height
			)
		{			
			bool approx = false, match = false;
			std::vector<std::pair<int, int>>::iterator mode_iter;
			size_t width_new = width, height_new = height;
			std::map<int, std::vector<std::pair<int, int>>> res_map;
			std::map<int, std::vector<std::pair<int, int>>>::iterator disp_iter;	

			if(!m_initialized) {
				THROW_CRAFT_DISPLAY_EXCEPTION(CRAFT_DISPLAY_EXCEPTION_UNINITIALIZED);
			}

			if(!m_running) {
				THROW_CRAFT_DISPLAY_EXCEPTION(CRAFT_DISPLAY_EXCEPTION_STOPPED);
			}

			res_map = craft_display::supported_resolutions();
			if(!res_map.empty()) {

				for(disp_iter = res_map.begin(); disp_iter != res_map.end();
						++disp_iter) {

					for(mode_iter = disp_iter->second.begin(); mode_iter != disp_iter->second.end();
							++mode_iter) {

						if((mode_iter->first == width) && (mode_iter->second == height)) {
							match = true;
							break;
						} else if(((mode_iter->second >= (height - RESOLUTION_BUFFER))
								&& (mode_iter->second <= (height + RESOLUTION_BUFFER)))
								|| ((mode_iter->first >= (width - RESOLUTION_BUFFER))
									&& (mode_iter->first <= (width + RESOLUTION_BUFFER)))) {
							approx = true;
							height_new = mode_iter->second;
							width_new = mode_iter->first;
						}
					}

					if(match) {
						break;
					}
				}
			}

			if(!approx && !match) {
				height_new = WINDOW_HEIGHT_MIN;
				width_new = WINDOW_WIDTH_MIN;
			}

			if(!match) {
				height = height_new;
				width = width_new;
			}
		}

		SDL_Window *
		_craft_display::window(void)
		{

			if(!m_initialized) {
				THROW_CRAFT_DISPLAY_EXCEPTION(CRAFT_DISPLAY_EXCEPTION_UNINITIALIZED);
			}

			if(!m_running) {
				THROW_CRAFT_DISPLAY_EXCEPTION(CRAFT_DISPLAY_EXCEPTION_STOPPED);
			}

			return m_window;
		}
	}
}

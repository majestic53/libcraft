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
#include "../include/craft_type.h"

namespace CRAFT {

	_craft *_craft::m_instance = NULL;

	_craft::_craft(void) :
		m_initialized(false),
		m_initialized_external(false),
		m_instance_display(craft_display::acquire()),
		m_instance_gl(craft_gl::acquire()),
		m_instance_world(craft_world::acquire()),
		m_running(false)
	{
		std::atexit(craft::_delete);
	}

	_craft::~_craft(void)
	{

		if(m_initialized) {
			uninitialize();
		}
	}

	void 
	_craft::_delete(void)
	{

		if(craft::m_instance) {
			delete craft::m_instance;
			craft::m_instance = NULL;
		}
	}

	_craft *
	_craft::acquire(void)
	{

		if(!craft::m_instance) {

			craft::m_instance = new craft;
			if(!craft::m_instance) {
				THROW_CRAFT_EXCEPTION(CRAFT_EXCEPTION_ALLOCATED);
			}
		}

		return craft::m_instance;
	}

	void 
	_craft::clear(void)
	{

		if(!m_initialized) {
			THROW_CRAFT_EXCEPTION(CRAFT_EXCEPTION_UNINITIALIZED);
		}

		if(m_running) {
			THROW_CRAFT_EXCEPTION(CRAFT_EXCEPTION_STARTED);
		}
	}

	void 
	_craft::initialize(void)
	{

		if(m_initialized) {
			THROW_CRAFT_EXCEPTION(CRAFT_EXCEPTION_INITIALIZED);
		}

		if(m_running) {
			THROW_CRAFT_EXCEPTION(CRAFT_EXCEPTION_STARTED);
		}

		m_instance_display->initialize();
		m_instance_gl->initialize();
		m_initialized = true;
	}

	bool 
	_craft::is_allocated(void)
	{
		return (craft::m_instance != NULL);
	}

	bool 
	_craft::is_initialized(void)
	{
		return m_initialized;
	}

	bool 
	_craft::is_running(void)
	{
		return m_running;
	}

	void 
	_craft::render(void)
	{

		if(!m_initialized) {
			THROW_CRAFT_EXCEPTION(CRAFT_EXCEPTION_UNINITIALIZED);
		}

		if(m_running) {
			m_instance_world->render();
		}
	}

	void 
	_craft::run(
		__in uint32_t seed,
		__in bool fullscreen,
		__in_opt size_t width,
		__in_opt size_t height
		)
	{
		SDL_Event event;
		GLfloat current = 0.f, last = 0.f;

		if(!m_initialized) {
			THROW_CRAFT_EXCEPTION(CRAFT_EXCEPTION_UNINITIALIZED);
		}

		if(m_running) {
			THROW_CRAFT_EXCEPTION(CRAFT_EXCEPTION_STARTED);
		}

		setup(seed, fullscreen, width, height);
		m_running = true;

		while(m_running) {

			while(SDL_PollEvent(&event)) {

				switch(event.type) {
					case SDL_QUIT:
						stop();
						goto quit;
					case SDL_KEYDOWN:
					case SDL_KEYUP:
						m_instance_world->on_event(event.key);
						break;
					case SDL_MOUSEBUTTONDOWN:
					case SDL_MOUSEBUTTONUP:
						m_instance_world->on_event(event.button);
						break;
					case SDL_MOUSEMOTION:
						m_instance_world->on_event(event.motion);
						break;
					case SDL_MOUSEWHEEL:
						m_instance_world->on_event(event.wheel);
						break;
					case SDL_WINDOWEVENT:
						m_instance_display->on_event(event.window);
						break;
				}
			}

			current = glfwGetTime();
			update(current - last);
			last = current;
			render();
		}

	quit:
		teardown();
	}

	void 
	_craft::setup(
		__in uint32_t seed,
		__in bool fullscreen,
		__in_opt size_t width,
		__in_opt size_t height
		)
	{
		uint32_t flags = WINDOW_FLAGS;

		if(!m_initialized) {
			THROW_CRAFT_EXCEPTION(CRAFT_EXCEPTION_UNINITIALIZED);
		}

		if(m_running) {
			THROW_CRAFT_EXCEPTION(CRAFT_EXCEPTION_STARTED);
		}

		if(fullscreen) {
			flags |= SDL_WINDOW_FULLSCREEN;
		}

		setup_external();
		m_instance_display->start(WINDOW_TITLE, WINDOW_LEFT, WINDOW_TOP, 
			width, height, flags);
		craft_gl::initialize_external(DISPLAY_GL_VERSION);
		m_instance_world->initialize(seed);
	}

	void 
	_craft::setup_external(void)
	{

		if(!m_initialized) {
			THROW_CRAFT_EXCEPTION(CRAFT_EXCEPTION_UNINITIALIZED);
		}

		if(m_running) {
			THROW_CRAFT_EXCEPTION(CRAFT_EXCEPTION_STARTED);
		}

		if(m_initialized_external) {
			THROW_CRAFT_EXCEPTION(CRAFT_EXCEPTION_EXTERNAL_INITIALIZED);
		}

		m_initialized_external = true;

		if(SDL_Init(EVENT_FILTER)) {
			THROW_CRAFT_EXCEPTION_FORMAT(CRAFT_EXCEPTION_EXTERNAL,
				"%s", SDL_GetError());
		}

		if(!glfwInit()) {
			THROW_CRAFT_EXCEPTION_FORMAT(CRAFT_EXCEPTION_EXTERNAL,
				"%s", "glfwInit failed");
		}
	}

	void 
	_craft::start(
		__in uint32_t seed,
		__in bool fullscreen,
		__in_opt size_t width,
		__in_opt size_t height
		)
	{

		if(!m_initialized) {
			THROW_CRAFT_EXCEPTION(CRAFT_EXCEPTION_UNINITIALIZED);
		}

		if(m_running) {
			THROW_CRAFT_EXCEPTION(CRAFT_EXCEPTION_STARTED);
		}

		run(seed, fullscreen, width, height);
	}

	void 
	_craft::stop(void)
	{

		if(!m_initialized) {
			THROW_CRAFT_EXCEPTION(CRAFT_EXCEPTION_UNINITIALIZED);
		}

		if(!m_running) {
			THROW_CRAFT_EXCEPTION(CRAFT_EXCEPTION_STOPPED);
		}

		m_running = false;
	}

	void 
	_craft::teardown(void)
	{

		if(!m_initialized) {
			THROW_CRAFT_EXCEPTION(CRAFT_EXCEPTION_UNINITIALIZED);
		}

		if(m_running) {
			THROW_CRAFT_EXCEPTION(CRAFT_EXCEPTION_STARTED);
		}

		m_instance_world->uninitialize();
		m_instance_display->stop();
		teardown_external();
	}

	void 
	_craft::teardown_external(void)
	{

		if(!m_initialized) {
			THROW_CRAFT_EXCEPTION(CRAFT_EXCEPTION_UNINITIALIZED);
		}

		if(m_running) {
			THROW_CRAFT_EXCEPTION(CRAFT_EXCEPTION_STARTED);
		}

		if(!m_initialized_external) {
			THROW_CRAFT_EXCEPTION(CRAFT_EXCEPTION_EXTERNAL_UNINITIALIZED);
		}

		glfwTerminate();
		SDL_Quit();
		m_initialized_external = false;
	}

	std::string 
	_craft::to_string(
		__in_opt bool verbose
		)
	{
		std::stringstream result;

		result << CRAFT_HEADER << " (" << (m_initialized ? "INITIALIZED" : "UNINITIALIZED")
			<< ", " << (m_running ? "STARTED" : "STOPPED");

		if(verbose) {
			result << "PTR. 0x" << SCALAR_AS_HEX(craft *, this);
		}

		result << ")";

		return result.str();
	}

	void 
	_craft::uninitialize(void)
	{

		if(!m_initialized) {
			THROW_CRAFT_EXCEPTION(CRAFT_EXCEPTION_UNINITIALIZED);
		}

		if(m_running) {
			stop();
		}

		if(m_initialized_external) {
			teardown_external();
		}

		clear();
		m_instance_gl->uninitialize();
		m_instance_display->uninitialize();
		m_initialized = false;
	}

	void 
	_craft::update(
		__in GLfloat delta
		)
	{

		if(!m_initialized) {
			THROW_CRAFT_EXCEPTION(CRAFT_EXCEPTION_UNINITIALIZED);
		}

		if(m_running) {
			m_instance_world->update(delta);
		}
	}

	std::string 
	_craft::version(void)
	{
		return VERSION_STRING;
	}
}

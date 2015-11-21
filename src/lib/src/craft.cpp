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

		// TODO: clear child components
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

		// TODO: initialize child components

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
		// TODO
	}

	void 
	_craft::run(void)
	{
		SDL_Event event;
		GLfloat current = 0.f, last = 0.f;

		if(!m_initialized) {
			THROW_CRAFT_EXCEPTION(CRAFT_EXCEPTION_UNINITIALIZED);
		}

		if(m_running) {
			THROW_CRAFT_EXCEPTION(CRAFT_EXCEPTION_STARTED);
		}

		setup();
		m_running = true;

		while(m_running) {

			while(SDL_PollEvent(&event)) {

				switch(event.type) {
					case SDL_QUIT:
						stop();
						break;
					case SDL_WINDOWEVENT:
						m_instance_display->on_event(event.window);
						break;

					// TODO: handle input events
				}
			}

			current = glfwGetTime();
			update(current - last);
			last = current;
			render();
		}

		teardown();
	}

	void 
	_craft::setup(void)
	{

		if(!m_initialized) {
			THROW_CRAFT_EXCEPTION(CRAFT_EXCEPTION_UNINITIALIZED);
		}

		if(m_running) {
			THROW_CRAFT_EXCEPTION(CRAFT_EXCEPTION_STARTED);
		}

		setup_external();
		m_instance_display->start(WINDOW_TITLE, WINDOW_LEFT, WINDOW_TOP, 
			WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_FLAGS);
		craft_gl::initialize_external(DISPLAY_GL_VERSION);

		// TODO: setup child components
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

		if(SDL_Init(SDL_INIT_EVERYTHING)) {
			THROW_CRAFT_EXCEPTION_FORMAT(CRAFT_EXCEPTION_EXTERNAL,
				"%s", SDL_GetError());
		}

		if(!glfwInit()) {
			THROW_CRAFT_EXCEPTION_FORMAT(CRAFT_EXCEPTION_EXTERNAL,
				"%s", "glfwInit failed");
		}
	}

	void 
	_craft::start(void)
	{

		if(!m_initialized) {
			THROW_CRAFT_EXCEPTION(CRAFT_EXCEPTION_UNINITIALIZED);
		}

		if(m_running) {
			THROW_CRAFT_EXCEPTION(CRAFT_EXCEPTION_STARTED);
		}

		run();
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

		// TODO: teardown child components

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

		// TODO: uninitialize child components

		m_instance_gl->uninitialize();
		m_instance_display->uninitialize();
		clear();
		m_initialized = false;
	}

	void 
	_craft::update(
		__in GLfloat delta
		)
	{
		// TODO
	}

	std::string 
	_craft::version(void)
	{
		return VERSION_STRING;
	}
}

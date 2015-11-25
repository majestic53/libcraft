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
#include "../include/craft_world_type.h"

namespace CRAFT {

	namespace COMPONENT {

		_craft_world *_craft_world::m_instance = NULL;

		_craft_world::_craft_world(void) :
			m_initialized(false),
			m_instance_camera(craft_camera::acquire()),
			m_instance_keyboard(craft_keyboard::acquire()),
			m_instance_mouse(craft_mouse::acquire()),
			m_instance_test(craft_test::acquire()),
			m_window(NULL)
		{
			std::atexit(craft_world::_delete);
		}

		_craft_world::~_craft_world(void)
		{

			if(m_initialized) {
				uninitialize();
			}
		}

		void 
		_craft_world::_delete(void)
		{

			if(craft_world::m_instance) {
				delete craft_world::m_instance;
				craft_world::m_instance = NULL;
			}
		}

		_craft_world *
		_craft_world::acquire(void)
		{

			if(!craft_world::m_instance) {

				craft_world::m_instance = new craft_world;
				if(!craft_world::m_instance) {
					THROW_CRAFT_WORLD_EXCEPTION(CRAFT_WORLD_EXCEPTION_ALLOCATED);
				}
			}

			return craft_world::m_instance;
		}

		void 
		_craft_world::clear(void)
		{

			if(!m_initialized) {
				THROW_CRAFT_WORLD_EXCEPTION(CRAFT_WORLD_EXCEPTION_UNINITIALIZED);
			}

			m_instance_camera->clear();
			m_instance_keyboard->clear();
			m_instance_mouse->clear();
			m_window = NULL;
		}

		void 
		_craft_world::initialize(void)
		{

			if(m_initialized) {
				THROW_CRAFT_WORLD_EXCEPTION(CRAFT_WORLD_EXCEPTION_INITIALIZED);
			}

			m_initialized = true;
			m_window = craft_display::acquire()->window();
			setup();
		}

		bool 
		_craft_world::is_allocated(void)
		{
			return (craft_world::m_instance != NULL);
		}

		bool 
		_craft_world::is_initialized(void)
		{
			return m_initialized;
		}

		void 
		_craft_world::on_event(
			__in const SDL_KeyboardEvent &event
			)
		{

			if(!m_initialized) {
				THROW_CRAFT_WORLD_EXCEPTION(CRAFT_WORLD_EXCEPTION_UNINITIALIZED);
			}

			m_instance_keyboard->on_event(event);
		}

		void 
		_craft_world::on_event(
			__in const SDL_MouseButtonEvent &event
			)
		{

			if(!m_initialized) {
				THROW_CRAFT_WORLD_EXCEPTION(CRAFT_WORLD_EXCEPTION_UNINITIALIZED);
			}

			m_instance_mouse->on_event(event);
		}

		void 
		_craft_world::on_event(
			__in const SDL_MouseMotionEvent &event
			)
		{

			if(!m_initialized) {
				THROW_CRAFT_WORLD_EXCEPTION(CRAFT_WORLD_EXCEPTION_UNINITIALIZED);
			}

			m_instance_mouse->on_event(event);
		}

		void 
		_craft_world::on_event(
			__in const SDL_MouseWheelEvent &event
			)
		{

			if(!m_initialized) {
				THROW_CRAFT_WORLD_EXCEPTION(CRAFT_WORLD_EXCEPTION_UNINITIALIZED);
			}

			m_instance_mouse->on_event(event);
		}

		void 
		_craft_world::poll_input(void)
		{

			if(!m_initialized) {
				THROW_CRAFT_WORLD_EXCEPTION(CRAFT_WORLD_EXCEPTION_UNINITIALIZED);
			}

			m_instance_keyboard->update();
			m_instance_mouse->update();
		}

		void 
		_craft_world::render(void)
		{

			if(!m_initialized) {
				THROW_CRAFT_WORLD_EXCEPTION(CRAFT_WORLD_EXCEPTION_UNINITIALIZED);
			}

			glClearColor(0.f, 0.f, 0.f, 1.f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			// TODO: render world
			m_instance_test->render(m_mvp);
			// ---

			glFlush();
			SDL_GL_SwapWindow(m_window);
		}

		void 
		_craft_world::reset(void)
		{

			if(!m_initialized) {
				THROW_CRAFT_WORLD_EXCEPTION(CRAFT_WORLD_EXCEPTION_UNINITIALIZED);
			}

			m_instance_camera->reset();
			m_instance_keyboard->reset();
			m_instance_mouse->reset();
		}

		void 
		_craft_world::setup(void)
		{
			int height = 0, width = 0;

			if(!m_initialized) {
				THROW_CRAFT_WORLD_EXCEPTION(CRAFT_WORLD_EXCEPTION_UNINITIALIZED);
			}

			SDL_GetWindowSize(m_window, &width, &height);
			m_instance_keyboard->initialize(KEY_SET);
			m_instance_mouse->initialize(m_window, true);
			m_instance_camera->initialize({width, height});
			m_instance_test->initialize();
			reset();
		}

		void 
		_craft_world::teardown(void)
		{

			if(!m_initialized) {
				THROW_CRAFT_WORLD_EXCEPTION(CRAFT_WORLD_EXCEPTION_UNINITIALIZED);
			}

			clear();
			m_instance_test->uninitialize();
			m_instance_camera->uninitialize();
			m_instance_mouse->uninitialize();
			m_instance_keyboard->uninitialize();
		}

		std::string 
		_craft_world::to_string(
			__in_opt bool verbose
			)
		{
			std::stringstream result;

			if(!m_initialized) {
				THROW_CRAFT_WORLD_EXCEPTION(CRAFT_WORLD_EXCEPTION_UNINITIALIZED);
			}

			result << CRAFT_WORLD_HEADER << " (" << (m_initialized ? "INITIALIZED" : "UNINITIALIZED");

			if(verbose) {
				result << ", PTR. 0x" << SCALAR_AS_HEX(craft_world *, this);
			}

			result << ")";

			return result.str();
		}

		void 
		_craft_world::uninitialize(void)
		{

			if(!m_initialized) {
				THROW_CRAFT_WORLD_EXCEPTION(CRAFT_WORLD_EXCEPTION_UNINITIALIZED);
			}

			teardown();
			m_window = NULL;
			m_initialized = false;
		}

		void 
		_craft_world::update(
			__in GLfloat delta
			)
		{

			if(!m_initialized) {
				THROW_CRAFT_WORLD_EXCEPTION(CRAFT_WORLD_EXCEPTION_UNINITIALIZED);
			}
			
			update_world(delta);
			update_input(delta);
			m_mvp = m_instance_camera->update(m_instance_mouse->position());
		}

		void 
		_craft_world::update_input(
			__in GLfloat delta
			)
		{
			GLfloat speed;

			if(!m_initialized) {
				THROW_CRAFT_WORLD_EXCEPTION(CRAFT_WORLD_EXCEPTION_UNINITIALIZED);
			}

			if(m_instance_keyboard->is_pressed(KEY_CODE(KEY_QUIT))) {
				craft::acquire()->stop();
			}

			speed = CAMERA_SPEED * delta;

			if(m_instance_keyboard->is_pressed(KEY_CODE(KEY_FORWARD))) {
				m_instance_camera->position() += m_instance_camera->target() * speed;
			}

			if(m_instance_keyboard->is_pressed(KEY_CODE(KEY_LEFT))) {
				m_instance_camera->position() -= glm::normalize(glm::cross(
					m_instance_camera->target(), m_instance_camera->up())) * speed;
			}

			if(m_instance_keyboard->is_pressed(KEY_CODE(KEY_BACKWARD))) {
				m_instance_camera->position() -= m_instance_camera->target() * speed;
			}

			if(m_instance_keyboard->is_pressed(KEY_CODE(KEY_RIGHT))) {
				m_instance_camera->position() += glm::normalize(glm::cross(
					m_instance_camera->target(), m_instance_camera->up())) * speed;
			}
		}

		void 
		_craft_world::update_world(
			__in GLfloat delta
			)
		{

			if(!m_initialized) {
				THROW_CRAFT_WORLD_EXCEPTION(CRAFT_WORLD_EXCEPTION_UNINITIALIZED);
			}

			// TODO: update world logic
			m_instance_test->update(delta);
			// ---
		}
	}
}

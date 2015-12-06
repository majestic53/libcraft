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

		size_t 
		_craft_position_key::operator()(
			__in const glm::vec2 &position
			) const
		{
			return std::hash<double>()(position.x) ^ std::hash<double>()(position.y);
		}

		bool 
		_craft_position_key::operator()(
			__in const glm::vec2 &left,
			__in const glm::vec2 &right
			) const
		{
			bool result = (&left == &right);

			if(!result) {
				result = ((left.x == right.x) && (left.y == right.y));
			}

			return result;
		}

		_craft_world *_craft_world::m_instance = NULL;

		_craft_world::_craft_world(void) :
			m_initialized(false),
			m_instance_camera(craft_camera::acquire()),
			m_instance_keyboard(craft_keyboard::acquire()),
			m_instance_mouse(craft_mouse::acquire()),
			m_instance_random(craft_random::acquire()),
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
			m_height_list.clear();
			m_chunk_map.clear();
			m_window = NULL;
		}

		void 
		_craft_world::initialize(
			__in uint32_t seed,
			__in double dimension,
			__in uint32_t octaves,
			__in double amplitude,
			__in double persistence,
			__in_opt bool bicubic
			)
		{

			if(m_initialized) {
				THROW_CRAFT_WORLD_EXCEPTION(CRAFT_WORLD_EXCEPTION_INITIALIZED);
			}

			m_initialized = true;
			m_window = craft_display::acquire()->window();
			setup(seed, dimension, octaves, amplitude, persistence, bicubic);
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

			glClearColor(BACKGROUND_COLOR.x, BACKGROUND_COLOR.y, BACKGROUND_COLOR.z, 1.f);
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

			m_instance_random->reset();
			m_instance_camera->reset();
			m_instance_keyboard->reset();
			m_instance_mouse->reset();
		}

		void 
		_craft_world::setup(
			__in uint32_t seed,
			__in double dimension,
			__in uint32_t octaves,
			__in double amplitude,
			__in double persistence,
			__in_opt bool bicubic
			)
		{
			glm::vec3 volume;
			glm::uvec2 result;
			int height = 0, width = 0;
			uint32_t center, count = 1;
			std::vector<uint8_t> heights;
			glm::vec2 offset, position, position_offset;
			size_t iter_x, iter_y, iter_height_x, iter_height_y;

			if(!m_initialized) {
				THROW_CRAFT_WORLD_EXCEPTION(CRAFT_WORLD_EXCEPTION_UNINITIALIZED);
			}

			if(std::fmod(dimension, CHUNK_WIDTH)) {
				THROW_CRAFT_WORLD_EXCEPTION_FORMAT(CRAFT_WORLD_EXCEPTION_INVALID_DIMENSION,
					"%lu (must be divisible by %lu)", dimension, CHUNK_WIDTH);
			}

			m_instance_random->initialize(seed);
			SDL_GetWindowSize(m_window, &width, &height);
			m_instance_keyboard->initialize(KEY_SET);
			m_instance_mouse->initialize(m_window, true);
			m_instance_camera->initialize({width, height});
			m_instance_test->initialize();
			reset();

			// TODO: DEBUG
			std::stringstream path;
			// ---

			for(iter_x = PERLIN_POSITION.x; iter_x < (PERLIN_POSITION.x + count); ++iter_x) {

				for(iter_y = PERLIN_POSITION.y; iter_y < (PERLIN_POSITION.y + count); ++iter_y) {
					position = {iter_x * dimension, iter_y * dimension};
					offset = {position.x + dimension, position.y + dimension};
					m_height_list = craft_perlin_2d::acquire()->generate(result, position, 
						offset, octaves, amplitude, persistence, bicubic);

					// TODO: DEBUG
					/*path.clear();
					path.str(std::string());
					path << "./height_map_" << iter_x << "_" << iter_y;
					craft_perlin_2d::acquire()->to_file(path.str().c_str(), 
						m_height_list, result, true);*/
					// ---
				}
			}

			center = (dimension / 2);
			count = (dimension / CHUNK_WIDTH);
			volume = glm::vec3{CHUNK_WIDTH, CHUNK_HEIGHT, CHUNK_WIDTH};

			for(iter_x = 0; iter_x < count; ++iter_x) {

				for(iter_y = 0; iter_y < count; ++iter_y) {
					position = glm::vec2{iter_x, iter_y};
					position_offset = glm::vec2{(position.x * CHUNK_WIDTH) - center, (position.y * CHUNK_WIDTH) - center};
					heights.clear();
					heights.resize(CHUNK_WIDTH * CHUNK_WIDTH, 0);

					for(iter_height_x = 0; iter_height_x < CHUNK_WIDTH; ++iter_height_x) {

						for(iter_height_y = 0; iter_height_y < CHUNK_WIDTH; ++iter_height_y) {
								heights.at(SCALAR_INDEX_2D(iter_height_x, iter_height_y, CHUNK_WIDTH))
									= (m_height_list.at(SCALAR_INDEX_2D(iter_height_x + position.x, 
										iter_height_y + position.y, CHUNK_WIDTH)) * CHUNK_HEIGHT);
						}
					}

					m_chunk_map.insert(std::pair<glm::vec2, craft_chunk>(position_offset, 
						craft_chunk(position, volume, heights)));

					// TODO: DEBUG
					if((iter_x == 6) && (iter_y == 6)) {
						path.clear();
						path.str(std::string());
						path << "./chunk_" << iter_x << "_" << iter_y;
						craft_chunk::to_file(path.str().c_str(), m_chunk_map.at(glm::vec2(position_offset)), true);
					}
					// ---
				}
			}
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
			m_instance_random->uninitialize();
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

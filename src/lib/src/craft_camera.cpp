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
#include "../include/craft_camera_type.h"

namespace CRAFT {

	namespace COMPONENT {

		_craft_camera *_craft_camera::m_instance = NULL;

		_craft_camera::_craft_camera(void) :
			m_dimensions({0.f, 0.f}),
			m_fov(CAMERA_FOV),
			m_initialized(false),
			m_model(MAT_INITIAL),
			m_mvp(MAT_INITIAL),
			m_pitch(CAMERA_PITCH),
			m_position(CAMERA_POSITION),
			m_projection(MAT_INITIAL),
			m_sensitivity(CAMERA_SENSITIVITY),
			m_target(CAMERA_TARGET),
			m_up(CAMERA_UP),
			m_view(MAT_INITIAL),
			m_yaw(CAMERA_YAW)
		{
			std::atexit(craft_camera::_delete);
		}

		_craft_camera::~_craft_camera(void)
		{

			if(!m_initialized) {
				uninitialize();
			}
		}

		void 
		_craft_camera::_delete(void)
		{

			if(!craft_camera::m_instance) {
				delete craft_camera::m_instance;
				craft_camera::m_instance = NULL;
			}
		}

		_craft_camera *
		_craft_camera::acquire(void)
		{

			if(!craft_camera::m_instance) {

				craft_camera::m_instance = new craft_camera;
				if(!craft_camera::m_instance) {
					THROW_CRAFT_CAMERA_EXCEPTION(CRAFT_CAMERA_EXCEPTION_ALLOCATED);
				}
			}

			return craft_camera::m_instance;
		}

		void 
		_craft_camera::clear(void)
		{

			if(!m_initialized) {
				THROW_CRAFT_CAMERA_EXCEPTION(CRAFT_CAMERA_EXCEPTION_UNINITIALIZED);
			}

			m_fov = CAMERA_FOV;
			m_model = MAT_INITIAL;
			m_mvp = MAT_INITIAL;
			m_pitch = CAMERA_PITCH;
			m_position = CAMERA_POSITION;
			m_projection = MAT_INITIAL;
			m_sensitivity = CAMERA_SENSITIVITY;
			m_target = CAMERA_TARGET;
			m_up = CAMERA_UP;
			m_view = MAT_INITIAL;
			m_yaw = CAMERA_YAW;
		}

		glm::vec2 &
		_craft_camera::dimensions(void)
		{

			if(!m_initialized) {
				THROW_CRAFT_CAMERA_EXCEPTION(CRAFT_CAMERA_EXCEPTION_UNINITIALIZED);
			}

			return m_dimensions;
		}

		GLfloat &
		_craft_camera::fov(void)
		{

			if(!m_initialized) {
				THROW_CRAFT_CAMERA_EXCEPTION(CRAFT_CAMERA_EXCEPTION_UNINITIALIZED);
			}

			return m_fov;
		}

		void 
		_craft_camera::initialize(
			__in const glm::vec2 &dimensions,
			__in_opt GLfloat fov,
			__in_opt GLfloat sensitivity
			)
		{

			if(m_initialized) {
				THROW_CRAFT_CAMERA_EXCEPTION(CRAFT_CAMERA_EXCEPTION_INITIALIZED);
			}

			m_initialized = true;
			clear();
			m_dimensions = dimensions;
			m_fov = fov;
			m_sensitivity = sensitivity;
		}

		bool 
		_craft_camera::is_allocated(void)
		{
			return (craft_camera::m_instance != NULL);
		}

		bool 
		_craft_camera::is_initialized(void)
		{
			return m_initialized;
		}

		glm::vec3 &
		_craft_camera::position(void)
		{

			if(!m_initialized) {
				THROW_CRAFT_CAMERA_EXCEPTION(CRAFT_CAMERA_EXCEPTION_UNINITIALIZED);
			}

			return m_position;
		}

		void 
		_craft_camera::reset(void)
		{

			if(!m_initialized) {
				THROW_CRAFT_CAMERA_EXCEPTION(CRAFT_CAMERA_EXCEPTION_UNINITIALIZED);
			}

			m_pitch = CAMERA_PITCH;
			m_position = CAMERA_POSITION;
			m_target = CAMERA_TARGET;
			m_up = CAMERA_UP;
			m_yaw = CAMERA_YAW;
		}

		GLfloat &
		_craft_camera::sensitivity(void)
		{

			if(!m_initialized) {
				THROW_CRAFT_CAMERA_EXCEPTION(CRAFT_CAMERA_EXCEPTION_UNINITIALIZED);
			}

			return m_sensitivity;
		}

		glm::vec3 &
		_craft_camera::target(void)
		{

			if(!m_initialized) {
				THROW_CRAFT_CAMERA_EXCEPTION(CRAFT_CAMERA_EXCEPTION_UNINITIALIZED);
			}

			return m_target;
		}

		std::string 
		_craft_camera::to_string(
			__in_opt bool verbose
			)
		{
			std::stringstream result;

			result << CRAFT_CAMERA_HEADER << " (" << (m_initialized ? "INITIALIZED" : "UNINITIALIZED");

			if(verbose) {
				result << ", PTR. 0x" << SCALAR_AS_HEX(craft_camera *, this);
			}

			if(m_initialized) {
				result << ", POS. {" << m_position.x << ", " << m_position.y << ", " << m_position.z 
					<< "}, TARG. {" << m_target.x << ", " << m_target.y << ", " << m_target.z 
					<< "}, UP. {" << m_up.x << ", " << m_up.y << ", " << m_up.z  
					<< "}, PIT. " << m_pitch << ", YAW. " << m_yaw << ", FOV. " << m_fov
					<< ", SENS. " << m_sensitivity;
			}

			result << ")";

			return result.str();
		}

		void 
		_craft_camera::uninitialize(void)
		{

			if(!m_initialized) {
				THROW_CRAFT_CAMERA_EXCEPTION(CRAFT_CAMERA_EXCEPTION_UNINITIALIZED);
			}

			clear();
			m_dimensions = {0.f, 0.f};
			m_initialized = false;
		}

		glm::vec3 &
		_craft_camera::up(void)
		{

			if(!m_initialized) {
				THROW_CRAFT_CAMERA_EXCEPTION(CRAFT_CAMERA_EXCEPTION_UNINITIALIZED);
			}

			return m_up;
		}

		glm::mat4 
		_craft_camera::update(
			__in const glm::vec2 &motion
			)
		{

			if(!m_initialized) {
				THROW_CRAFT_CAMERA_EXCEPTION(CRAFT_CAMERA_EXCEPTION_UNINITIALIZED);
			}

			m_yaw += (motion.x * m_sensitivity);
			m_pitch -= (motion.y * m_sensitivity);

			if(m_pitch > CAMERA_PITCH_MAX) {
				m_pitch = CAMERA_PITCH_MAX;
			} else if(m_pitch < -CAMERA_PITCH_MAX) {
				m_pitch = -CAMERA_PITCH_MAX;
			}

			m_target.x = std::cos(glm::radians(m_pitch)) * std::cos(glm::radians(m_yaw));
			m_target.y = std::sin(glm::radians(m_pitch));
			m_target.z = std::cos(glm::radians(m_pitch)) * std::sin(glm::radians(m_yaw));
			m_target = glm::normalize(m_target);
			m_projection = glm::perspective(glm::radians(m_fov), (GLfloat) m_dimensions.x / (GLfloat) m_dimensions.y, 
				0.1f, 100.f);
			m_view = glm::lookAt(m_position, m_position + m_target, m_up);
			m_model = glm::mat4(MAT_UNIT);
			m_mvp = m_projection * m_view * m_model;

			return m_mvp;
		}
	}
}

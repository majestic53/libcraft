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

#ifndef CRAFT_CAMERA_H_
#define CRAFT_CAMERA_H_

namespace CRAFT {

	namespace COMPONENT {

		typedef class _craft_camera {

			public:

				~_craft_camera(void);

				static _craft_camera *acquire(void);

				void clear(void);

				GLfloat &fov(void);

				glm::vec2 &dimensions(void);

				void initialize(
					__in const glm::vec2 &dimensions,
					__in_opt GLfloat fov = CAMERA_FOV,
					__in_opt GLfloat sensitivity = CAMERA_SENSITIVITY
					);

				static bool is_allocated(void);

				bool is_initialized(void);

				glm::vec3 &position(void);

				void reset(void);

				GLfloat &sensitivity(void);

				glm::vec3 &target(void);

				std::string to_string(
					__in_opt bool verbose = false
					);

				void uninitialize(void);

				glm::vec3 &up(void);

				glm::mat4 update(
					__in const glm::vec2 &motion
					);

			protected:

				_craft_camera(void);

				_craft_camera(
					__in const _craft_camera &other
					);

				_craft_camera &operator=(
					__in const _craft_camera &other
					);

				static void _delete(void);

				glm::vec2 m_dimensions;

				GLfloat m_fov;

				bool m_initialized;

				static _craft_camera *m_instance;

				glm::mat4 m_model;

				glm::mat4 m_mvp;

				GLfloat m_pitch;

				glm::vec3 m_position;

				glm::mat4 m_projection;

				GLfloat m_sensitivity;

				glm::vec3 m_target;

				glm::vec3 m_up;

				glm::mat4 m_view;

				GLfloat m_yaw;

		} craft_camera;
	}
}

#endif // CRAFT_CAMERA_H_

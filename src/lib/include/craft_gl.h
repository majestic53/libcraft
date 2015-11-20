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

#ifndef CRAFT_GL_H_
#define CRAFT_GL_H_

namespace CRAFT {

	typedef class _craft_gl {

		public:

			~_craft_gl(void);

			static _craft_gl *acquire(void);

			void initialize(void);

			static void initialize_external(
				__in uint32_t accelerate,
				__in uint32_t depth,
				__in uint32_t double_buffer,
				__in uint32_t major,
				__in uint32_t minor,
				__in uint32_t swap_interval,
				__in uint32_t glew_version
				);

			static bool is_allocated(void);

			bool is_initialized(void);

			std::string to_string(
				__in_opt bool verbose = false
				);

			void uninitialize(void);

		protected:

			_craft_gl(void);

			_craft_gl(
				__in const _craft_gl &other
				);

			_craft_gl &operator=(
				__in const _craft_gl &other
				);

			static void _delete(void);

			bool m_initialized;

			static _craft_gl *m_instance;

	} craft_gl;
}

#endif // CRAFT_GL_H_

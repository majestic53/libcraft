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

#ifndef CRAFT_TEXT_H_
#define CRAFT_TEXT_H_

namespace CRAFT {

	namespace COMPONENT {

		typedef class _craft_text {

			public:

				~_craft_text(void);

				static _craft_text *acquire(void);

				void clear(void);

				void initialize(void);

				static bool is_allocated(void);

				bool is_initialized(void);

				void render(void);

				std::string to_string(
					__in_opt bool verbose = false
					);

				void uninitialize(void);

			protected:

				_craft_text(void);

				_craft_text(
					__in const _craft_text &other
					);

				_craft_text &operator=(
					__in const _craft_text &other
					);

				static void _delete(void);

				bool m_initialized;

				static _craft_text *m_instance;

				FT_Library m_library;

		} craft_text;
	}
}

#endif // CRAFT_TEXT_H_

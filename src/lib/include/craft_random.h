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

#ifndef CRAFT_RANDOM_H_
#define CRAFT_RANDOM_H_

#include <random>

namespace CRAFT {

	typedef class _craft_random {

		public:

			~_craft_random(void);

			static _craft_random *acquire(void);

			// TODO: generate_perlin_2d

			int32_t generate_signed(
				__in_opt int32_t min = INT32_MIN,
				__in_opt int32_t max = INT32_MAX
				);

			uint32_t generate_unsigned(
				__in_opt uint32_t min = 0,
				__in_opt uint32_t max = UINT32_MAX
				);

			void initialize(
				__in uint32_t seed
				);

			static bool is_allocated(void);

			bool is_initialized(void);

			void reset(void);

			std::string to_string(
				__in_opt bool verbose = false
				);

			void uninitialize(void);

		protected:

			_craft_random(void);

			_craft_random(
				__in const _craft_random &other
				);

			_craft_random &operator=(
				__in const _craft_random &other
				);

			static void _delete(void);

			std::mt19937 m_engine;

			bool m_initialized;

			static _craft_random *m_instance;

			uint32_t m_seed;

	} craft_random;
}

#endif // CRAFT_RANDOM_H_

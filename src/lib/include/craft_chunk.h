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

#ifndef CRAFT_CHUNK_H_
#define CRAFT_CHUNK_H_

namespace CRAFT {

	namespace COMPONENT {

		typedef enum {
			CRAFT_BLOCK_AIR = 0,
			CRAFT_BLOCK_BOUNDARY,
			CRAFT_BLOCK_WATER,
			CRAFT_BLOCK_STONE,
			CRAFT_BLOCK_SAND,
			CRAFT_BLOCK_DIRT,
			CRAFT_BLOCK_DIRT_SIDE,
			CRAFT_BLOCK_GRASS,
			CRAFT_BLOCK_WOOD,
			CRAFT_BLOCK_LEAVES,
		} craft_block;

		#define CRAFT_BLOCK_MAX CRAFT_BLOCK_LEAVES

		typedef class _craft_chunk {

			public:

				_craft_chunk(
					__in const glm::vec2 &position,
					__in const glm::vec3 &dimension,
					__in const std::vector<uint8_t> &height
					);

				_craft_chunk(
					__in const _craft_chunk &other
					);

				virtual ~_craft_chunk(void);

				_craft_chunk &operator=(
					__in const _craft_chunk &other
					);

				craft_block at(
					__in const glm::vec3 &position
					);

				glm::vec3 dimension(void);

				uint8_t height_at(
					__in const glm::vec2 &position
					);

				bool is_empty(
					__in const glm::vec3 &position
					);

				glm::vec2 position(void);

				void set(
					__in const glm::vec3 &position,
					__in craft_block type
					);

				static void to_file(
					__in const std::string &path,
					__in const _craft_chunk &chunk
					);

				virtual std::string to_string(
					__in_opt bool verbose = false
					);

			protected:

				std::vector<uint8_t>::iterator find_block(
					__in const glm::vec3 &position
					);

				std::vector<uint8_t>::iterator find_height(
					__in const glm::vec2 &position
					);

				void generate_blocks(void);

				void initialize(
					__in const glm::vec2 &position,
					__in const glm::vec3 &dimension,
					__in const std::vector<uint8_t> &height
					);

				bool is_valid_height(
					__in const glm::vec2 &position
					);

				bool is_valid_position(
					__in const glm::vec3 &position
					);

				std::vector<uint8_t> m_block;

				glm::vec3 m_dimension;

				std::vector<uint8_t> m_height;

				glm::vec2 m_position;
		} craft_chunk;
	}
}

#endif // CRAFT_CHUNK_H_

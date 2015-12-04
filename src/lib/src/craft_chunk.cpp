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

#include <fstream>
#include "../include/craft.h"
#include "../include/craft_chunk_type.h"

namespace CRAFT {

	namespace COMPONENT {

		_craft_chunk::_craft_chunk(
			__in const glm::vec2 &position,
			__in const glm::vec3 &dimension,
			__in const std::vector<uint8_t> &height
			)
		{
			initialize(position, dimension, height);
		}

		_craft_chunk::_craft_chunk(
			__in const _craft_chunk &other
			) :
				m_block(other.m_block),
				m_dimension(other.m_dimension),
				m_height(other.m_height),
				m_position(other.m_position)
		{
			return;
		}

		_craft_chunk::~_craft_chunk(void)
		{
			return;
		}

		_craft_chunk &
		_craft_chunk::operator=(
			__in const _craft_chunk &other
			)
		{

			if(this != &other) {
				m_block = other.m_block;
				m_dimension = other.m_dimension;
				m_height = other.m_height;
				m_position = other.m_position;
			}

			return *this;
		}

		craft_block 
		_craft_chunk::at(
			__in const glm::vec3 &position
			)
		{
			return (craft_block) *find_block(position);
		}

		glm::vec3 
		_craft_chunk::dimension(void)
		{
			return m_dimension;
		}

		std::vector<uint8_t>::iterator 
		_craft_chunk::find_block(
			__in const glm::vec3 &position
			)
		{

			if(!is_valid_position(position)) {
				THROW_CRAFT_CHUNK_EXCEPTION_FORMAT(CRAFT_CHUNK_EXCEPTION_INVALID_POSITION,
					"{%f, %f, %f}", position.x, position.y, position.z);
			}

			return (m_block.begin() + SCALAR_INDEX_3D(position.x, (m_dimension.y - 1.0) - position.y, 
				position.z, m_dimension.z, m_dimension.x));
		}

		std::vector<uint8_t>::iterator 
		_craft_chunk::find_height(
			__in const glm::vec2 &position
			)
		{

			if(!is_valid_height(position)) {
				THROW_CRAFT_CHUNK_EXCEPTION_FORMAT(CRAFT_CHUNK_EXCEPTION_INVALID_POSITION,
					"{%f, %f}", position.x, position.y);
			}

			return (m_height.begin() + SCALAR_INDEX_2D(position.x, position.y, m_dimension.x));
		}

		uint8_t 
		_craft_chunk::height_at(
			__in const glm::vec2 &position
			)
		{
			return *find_height(position);
		}

		void 
		_craft_chunk::generate_blocks(void)
		{
			glm::ivec3 iter = {0, 0, 0};

			m_block.resize(m_dimension.x * m_dimension.y * m_dimension.z, CRAFT_BLOCK_AIR);

			for(iter.y = (m_dimension.y - 1.0); iter.y >= 0; iter.y--) {

				for(iter.z = 0; iter.z < m_dimension.z; ++iter.z) {

					for(iter.x = 0; iter.x < m_dimension.x; ++iter.x) {

						if(*find_height({iter.x, iter.z}) < iter.y) {
							continue;
						}

						// TODO
						if(*find_height({iter.x, iter.z}) == iter.y) {
							set(iter, CRAFT_BLOCK_GRASS);
						} else {
							set(iter, CRAFT_BLOCK_DIRT);
						}

						std::cout << "{" << iter.x << ", " << iter.y << ", " << iter.z 
							<< "} HEIGHT: " << iter.y << ", TYPE: 0x" 
							<< SCALAR_AS_HEX(craft_block, at(iter)) << std::endl;
						// ---
					}
				}
			}
		}

		void 
		_craft_chunk::initialize(
			__in const glm::vec2 &position,
			__in const glm::vec3 &dimension,
			__in const std::vector<uint8_t> &height
			)
		{

			if((position.x < 0.0) || (position.y < 0.0)) {
				THROW_CRAFT_CHUNK_EXCEPTION_FORMAT(CRAFT_CHUNK_EXCEPTION_INVALID_POSITION,
					"{%f, %f}", position.x, position.y);
			}

			if((dimension.x <= 0.0) || (dimension.y <= 0.0) 
					|| (dimension.z <= 0.0)) {
				THROW_CRAFT_CHUNK_EXCEPTION_FORMAT(CRAFT_CHUNK_EXCEPTION_INVALID_DIMENSION,
					"{%f, %f, %f}", dimension.x, dimension.y, dimension.z);
			}

			if(height.size() != (dimension.x * dimension.z)) {
				THROW_CRAFT_CHUNK_EXCEPTION_FORMAT(CRAFT_CHUNK_EXCEPTION_INVALID_HEIGHT_MAP,
					"%lu (should contain %lu entries)", height.size(), dimension.x * dimension.z);
			}

			m_position = position;
			m_dimension = dimension;
			m_height = height;
			generate_blocks();
		}

		bool 
		_craft_chunk::is_empty(
			__in const glm::vec3 &position
			)
		{
			return (at(position) == CRAFT_BLOCK_AIR);
		}

		bool 
		_craft_chunk::is_valid_height(
			__in const glm::vec2 &position
			)
		{
			return ((position.x >= 0.0) 
				&& (position.y >= 0.0) 
				&& (position.x < m_dimension.x) 
				&& (position.y < m_dimension.z));
		}

		bool 
		_craft_chunk::is_valid_position(
			__in const glm::vec3 &position
			)
		{
			return ((position.x >= 0.0) 
				&& (position.y >= 0.0) 
				&& (position.z >= 0.0)
				&& (position.x < m_dimension.x) 
				&& (position.y < m_dimension.y) 
				&& (position.z < m_dimension.z));
		}

		glm::vec2 
		_craft_chunk::position(void)
		{
			return m_position;
		}

		void 
		_craft_chunk::set(
			__in const glm::vec3 &position,
			__in craft_block type
			)
		{
			glm::vec3 pos;
			std::vector<uint8_t>::iterator iter;

			if(type > CRAFT_BLOCK_MAX) {
				THROW_CRAFT_CHUNK_EXCEPTION_FORMAT(CRAFT_CHUNK_EXCEPTION_INVALID_TYPE,
					"0x%x", type);
			}

			iter = find_height({position.x, position.z});
			*find_block(position) = type;
			pos = position;

			while((type == CRAFT_BLOCK_AIR) && (pos.y == *iter)) {
				pos.y -= 1.0;
				*iter = (*iter - 1);
			}
		}

		void 
		_craft_chunk::to_file(
			__in const std::string &path,
			__in const _craft_chunk &chunk
			)
		{
			// TODO
		}

		std::string 
		_craft_chunk::to_string(
			__in_opt bool verbose
			)
		{
			std::stringstream result;

			result << CRAFT_CHUNK_HEADER << " (POS. {" << m_position.x << ", " << m_position.y 
				<< "}, DIM. {" << m_dimension.x << ", " << m_dimension.y 
				<< ", " << m_dimension.z << "}";

			if(verbose) {
				result << ", PTR. 0x" << SCALAR_AS_HEX(craft_chunk *, this);
			}

			result << ")";

			return result.str();
		}
	}
}

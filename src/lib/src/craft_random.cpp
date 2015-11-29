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
#include "../include/craft_random_type.h"

namespace CRAFT {

	_craft_random *_craft_random::m_instance = NULL;

	_craft_random::_craft_random(void) :
		m_initialized(false),
		m_seed(0)
	{
		std::atexit(craft_random::_delete);
	}

	_craft_random::~_craft_random(void)
	{

		if(m_initialized) {
			uninitialize();
		}
	}

	void 
	_craft_random::_delete(void)
	{

		if(craft_random::m_instance) {
			delete craft_random::m_instance;
			craft_random::m_instance = NULL;
		}
	}

	_craft_random *
	_craft_random::acquire(void)
	{

		if(!craft_random::m_instance) {

			craft_random::m_instance = new craft_random;
			if(!craft_random::m_instance) {
				THROW_CRAFT_RANDOM_EXCEPTION(CRAFT_RANDOM_EXCEPTION_ALLOCATED);
			}
		}

		return craft_random::m_instance;
	}

	int32_t 
	_craft_random::generate_signed(
		__in_opt int32_t min,
		__in_opt int32_t max
		)
	{

		if(!m_initialized) {
			THROW_CRAFT_RANDOM_EXCEPTION(CRAFT_RANDOM_EXCEPTION_UNINITIALIZED);
		}

		if(min >= max) {
			THROW_CRAFT_RANDOM_EXCEPTION_FORMAT(CRAFT_RANDOM_EXCEPTION_INVALID_RANGE,
				"{%ls - %ls} (min >= max)", min, max);
		}

		return std::uniform_int_distribution<int32_t>(min, max)(m_engine);
	}

	uint32_t 
	_craft_random::generate_unsigned(
		__in_opt uint32_t min,
		__in_opt uint32_t max
		)
	{

		if(!m_initialized) {
			THROW_CRAFT_RANDOM_EXCEPTION(CRAFT_RANDOM_EXCEPTION_UNINITIALIZED);
		}

		if(min >= max) {
			THROW_CRAFT_RANDOM_EXCEPTION_FORMAT(CRAFT_RANDOM_EXCEPTION_INVALID_RANGE,
				"{%lu - %lu} (min >= max)", min, max);
		}

		return std::uniform_int_distribution<uint32_t>(min, max)(m_engine);
	}

	void 
	_craft_random::initialize(
		__in uint32_t seed
		)
	{

		if(m_initialized) {
			THROW_CRAFT_RANDOM_EXCEPTION(CRAFT_RANDOM_EXCEPTION_INITIALIZED);
		}

		m_initialized = true;
		m_seed = seed;
		reset();
	}

	bool 
	_craft_random::is_allocated(void)
	{
		return (craft_random::m_instance != NULL);
	}

	bool 
	_craft_random::is_initialized(void)
	{
		return m_initialized;
	}

	void 
	_craft_random::reset(void)
	{

		if(!m_initialized) {
			THROW_CRAFT_RANDOM_EXCEPTION(CRAFT_RANDOM_EXCEPTION_UNINITIALIZED);
		}

		m_engine.seed(m_seed);
	}

	std::string 
	_craft_random::to_string(
		__in_opt bool verbose
		)
	{
		std::stringstream result;

		result << CRAFT_RANDOM_HEADER << " (" << (m_initialized ? "INITIALIZED" : "UNINITIALIZED");

		if(verbose) {
			result << ", PTR. 0x" << SCALAR_AS_HEX(craft_random *, this);
		}

		result << ", SEED. 0x" << SCALAR_AS_HEX(uint32_t, m_seed) << ")";

		return result.str();
	}

	void 
	_craft_random::uninitialize(void)
	{

		if(!m_initialized) {
			THROW_CRAFT_RANDOM_EXCEPTION(CRAFT_RANDOM_EXCEPTION_UNINITIALIZED);
		}

		m_seed = 0;
		reset();
		m_initialized = false;
	}
}

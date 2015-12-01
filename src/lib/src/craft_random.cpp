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
#include "../include/craft_random_type.h"

namespace CRAFT {

	_craft_perlin_2d::_craft_perlin_2d(
		__in double amplitude,
		__in double frequency,
		__in double persistence,
		__in uint32_t octaves,
		__in uint32_t seed
		) :
			m_amplitude(amplitude),
			m_frequency(frequency),
			m_octaves(octaves),
			m_persistence(persistence),
			m_seed(seed)
	{
		return;
	}

	_craft_perlin_2d::_craft_perlin_2d(
		__in const _craft_perlin_2d &other
		) :
			m_amplitude(other.m_amplitude),
			m_frequency(other.m_frequency),
			m_octaves(other.m_octaves),
			m_persistence(other.m_persistence),
			m_seed(other.m_seed)
	{
		return;
	}

	_craft_perlin_2d::~_craft_perlin_2d(void)
	{
		return;
	}

	_craft_perlin_2d &
	_craft_perlin_2d::operator=(
		__in const _craft_perlin_2d &other
		)
	{

		if(this != &other) {
			m_amplitude = other.m_amplitude;
			m_frequency = other.m_frequency;
			m_octaves = other.m_octaves;
			m_persistence = other.m_persistence;
			m_seed = other.m_seed;
		}

		return *this;
	}

	double &
	_craft_perlin_2d::amplitude(void)
	{
		return m_amplitude;
	}

	double &
	_craft_perlin_2d::frequency(void)
	{
		return m_frequency;
	}

	double 
	_craft_perlin_2d::generate(
		__in const glm::vec2 &position
		)
	{
		size_t iter = 0;
		double curr_freq = m_frequency, curr_amp = 1.0, result = 0.0;

		for(; iter < m_octaves; ++iter) {
			result += generate_value({position.x * curr_freq + m_seed, 
				position.y * curr_freq + m_seed}) * curr_amp;
			curr_amp *= m_persistence;
			curr_freq *= PERLIN_FREQ_SCALE;
		}

		return result * m_amplitude;
	}

	double 
	_craft_perlin_2d::generate_interpolation(
		__in const glm::vec3 &value
		)
	{
		double z = (value.z * value.z), z_prime = (1.0 - value.z);

		z_prime *= z_prime;

		return ((value.x * ((3.0 * z_prime) - (2.0 * (z_prime * z)))) 
			+ (value.y * ((3.0 * z) - (2.0 * (z * value.z)))));
	}

	double 
	_craft_perlin_2d::generate_noise(
		__in const glm::vec2 &value
		)
	{
		return glm::perlin(value);
	}

	double 
	_craft_perlin_2d::generate_value(
		__in const glm::vec2 &value
		)
	{		
		glm::vec2 delta, interpolate, origin;
		double sample[PERLIN_SAMPLE_COUNT] = {0.0};

		std::modf(value.x, &origin.x);
		std::modf(value.y, &origin.y);
		delta = {value.x - origin.x, value.y - origin.y};
		sample[0] = generate_noise({origin.x - 1.0, origin.y - 1.0});
		sample[1] = generate_noise({origin.x + 1.0, origin.y - 1.0});
		sample[2] = generate_noise({origin.x - 1.0, origin.y + 1.0});
		sample[3] = generate_noise({origin.x + 1.0, origin.y + 1.0});
		sample[4] = generate_noise({origin.x - 1.0, origin.y});
		sample[5] = generate_noise({origin.x + 1.0, origin.y});
		sample[6] = generate_noise({origin.x, origin.y - 1.0});
		sample[7] = generate_noise({origin.x, origin.y + 1.0});
		sample[8] = generate_noise({origin.x, origin.y});
		sample[9] = generate_noise({origin.x + 2.0, origin.y - 1.0});
		sample[10] = generate_noise({origin.x + 2.0, origin.y + 1.0});
		sample[11] = generate_noise({origin.x + 2.0, origin.y});
		sample[12] = generate_noise({origin.x - 1.0, origin.y + 2.0});
		sample[13] = generate_noise({origin.x + 1.0, origin.y + 2.0});
		sample[14] = generate_noise({origin.x, origin.y + 2.0});
		sample[15] = generate_noise({origin.x + 2.0, origin.y + 2.0});
		interpolate.x = generate_interpolation({
				(PERLIN_WEIGHT_0 * (sample[0] + sample[1] + sample[2] + sample[3])) 
				+ (PERLIN_WEIGHT_1 * (sample[4] + sample[5] + sample[6] + sample[7])) 
				+ (PERLIN_WEIGHT_2 * sample[8]), 
				(PERLIN_WEIGHT_0 * (sample[6] + sample[9] + sample[7] + sample[10])) 
				+ (PERLIN_WEIGHT_1 * (sample[8] + sample[11] + sample[1] + sample[3])) 
				+ (PERLIN_WEIGHT_2 * sample[5]), 
				delta.x});
		interpolate.y = generate_interpolation({
				(PERLIN_WEIGHT_0 * (sample[4] + sample[5] + sample[12] + sample[13])) 
				+ (PERLIN_WEIGHT_1 * (sample[2] + sample[3] + sample[8] + sample[14])) 
				+ (PERLIN_WEIGHT_2 * sample[7]), 
				(PERLIN_WEIGHT_0 * (sample[8] + sample[11] + sample[14] + sample[15])) 
				+ (PERLIN_WEIGHT_1 * (sample[7] + sample[10] + sample[5] + sample[13])) 
				+ (PERLIN_WEIGHT_2 * sample[3]), 
				delta.x});

		return generate_interpolation({interpolate.x, interpolate.y, delta.y});
	}

	uint32_t &
	_craft_perlin_2d::octaves(void)
	{
		return m_octaves;
	}

	double &
	_craft_perlin_2d::persistence(void)
	{
		return m_persistence;
	}

	uint32_t &
	_craft_perlin_2d::seed(void)
	{
		return m_seed;
	}

	void 
	_craft_perlin_2d::to_file(
		__in const std::string &path,
		__in const glm::vec2 &origin,
		__in const glm::uvec2 &dimension,
		__in_opt bool colorize
		)
	{
		std::stringstream stream;
		double iter_x, iter_y, normalized;

		std::ofstream file(path.c_str(), std::ios::out);
		if(!file) {
			THROW_CRAFT_RANDOM_EXCEPTION_FORMAT(CRAFT_RANDOM_EXCEPTION_FILE_NOT_FOUND,
				"%s", STRING_CHECK(path));
		}

		if(colorize) {
			stream << "P3" << std::endl;
		} else {
			stream << "P2" << std::endl;
		}	

		stream << dimension.x << " " << dimension.y << std::endl;

		if(colorize) {
			stream << 255 << std::endl;
		} else {
			stream << 128 << std::endl;
		}

		for(iter_y = 0.0; iter_y < dimension.y; ++iter_y) {

			for(iter_x = 0.0; iter_x < dimension.x; ++iter_x) {

				if(iter_x > 0) {
					stream << " ";
				}

				normalized = generate_value({iter_x, iter_y});

				if(colorize) {

					if((normalized >= 0.0) && (normalized < 0.3)) {
						stream << "0 0 " << (uint32_t) (255.0 * (normalized / 0.3));
					} else if((normalized >= 0.3) && (normalized < 0.4)) {
						stream << "0 " << (uint32_t) (255.0 * ((normalized - 0.3) / 0.1)) << " 255";
					} else if((normalized >= 0.4) && (normalized < 0.6)) {
						stream << "0 255 " << (255.0 - (uint32_t) (255.0 * ((normalized - 0.4) / 0.2)));
					} else if((normalized >= 0.6) && (normalized < 0.7)) {
						stream << (uint32_t) (255.0 * ((normalized - 0.6) / 0.1)) << " 255 0";
					} else {
						stream << " 255 " << (255.0 - (uint32_t) (255.0 * ((normalized - 0.7) / 0.3))) << " 0";
					}
				} else {
					stream << (uint32_t) (128.0 * normalized);
				}
			}

			stream << std::endl;
		}

		file.write(stream.str().c_str(), stream.str().size());
		file.close();
	}

	std::string 
	_craft_perlin_2d::to_string(
		__in_opt bool verbose
		)
	{
		std::stringstream result;

		result << CRAFT_PERLIN_HEADER << " (SEED. 0x" << SCALAR_AS_HEX(uint32_t, m_seed) 
			<< ", AMP. " << m_amplitude << ", FREQ. " << m_frequency 
			<< ", OCT. " << m_octaves << ", PER. " << m_persistence << ")";

		return result.str();
	}

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

	double 
	_craft_random::generate_float(
		__in_opt double min,
		__in_opt double max
		)
	{

		if(!m_initialized) {
			THROW_CRAFT_RANDOM_EXCEPTION(CRAFT_RANDOM_EXCEPTION_UNINITIALIZED);
		}

		if(min >= max) {
			THROW_CRAFT_RANDOM_EXCEPTION_FORMAT(CRAFT_RANDOM_EXCEPTION_INVALID_RANGE,
				"{%f - %f} (min >= max)", min, max);
		}

		return std::uniform_real_distribution<double>(min, max)(m_engine);
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

	uint32_t 
	_craft_random::seed(void)
	{

		if(!m_initialized) {
			THROW_CRAFT_RANDOM_EXCEPTION(CRAFT_RANDOM_EXCEPTION_UNINITIALIZED);
		}

		return m_seed;
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

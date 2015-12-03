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

	_craft_perlin_2d *_craft_perlin_2d::m_instance = NULL;

	_craft_perlin_2d::_craft_perlin_2d(void) :
		m_initialized(false)
	{
		std::atexit(craft_perlin_2d::_delete);
	}

	_craft_perlin_2d::~_craft_perlin_2d(void)
	{

		if(m_initialized) {
			uninitialize();
		}
	}

	void 
	_craft_perlin_2d::_delete(void)
	{

		if(craft_perlin_2d::m_instance) {
			delete craft_perlin_2d::m_instance;
			craft_perlin_2d::m_instance = NULL;
		}
	}

	_craft_perlin_2d *
	_craft_perlin_2d::acquire(void)
	{

		if(!craft_perlin_2d::m_instance) {

			craft_perlin_2d::m_instance = new craft_perlin_2d;
			if(!craft_perlin_2d::m_instance) {
				THROW_CRAFT_RANDOM_EXCEPTION(CRAFT_RANDOM_EXCEPTION_ALLOCATED);
			}
		}

		return craft_perlin_2d::m_instance;
	}

	std::vector<double> 
	_craft_perlin_2d::generate(
		__out glm::uvec2 &dimension,
		__in const glm::vec2 &position,
		__in const glm::vec2 &offset,
		__in uint32_t octaves,
		__in double amplitude,
		__in double persistence,
		__in_opt bool bicubic
		)
	{
		uint32_t iter_x, iter_y;
		std::vector<double> result;
		double amplitude_total = 0.0;
		std::vector<std::vector<double>> noise;
		std::vector<double>::iterator iter_result;
		std::vector<std::vector<double>>::reverse_iterator iter_noise;

		if(!m_initialized) {
			THROW_CRAFT_RANDOM_EXCEPTION(CRAFT_RANDOM_EXCEPTION_UNINITIALIZED);
		}

		if(position.x < offset.x) {
			dimension.x = std::abs(offset.x - position.x);
		} else {
			dimension.x = std::abs(position.x - offset.x);
		}

		if(position.y < offset.y) {
			dimension.y = std::abs(offset.y - position.y);
		} else {
			dimension.y = std::abs(position.y - offset.y);
		}

		result.resize(dimension.x * dimension.y, 0.0);
		generate_noise(noise, dimension, position, octaves, bicubic);

		if(!noise.empty()) {

			for(iter_noise = noise.rbegin(); iter_noise != noise.rend(); 
					++iter_noise) {
				amplitude *= persistence;
				amplitude_total += amplitude;

				for(iter_x = 0; iter_x < dimension.x; ++iter_x) {

					for(iter_y = 0; iter_y < dimension.y; ++iter_y) {
						result.at(SCALAR_INDEX_2D(iter_x, iter_y, dimension.x))
							+= std::abs((iter_noise->at(SCALAR_INDEX_2D(iter_x, iter_y, dimension.x)) 
							* amplitude));
					}
				}
			}

			for(iter_result = result.begin(); iter_result != result.end(); 
					++iter_result) {
				*iter_result /= amplitude_total;
			}
		}

		return result;
	}

	void 
	_craft_perlin_2d::generate_noise(
		__out std::vector<std::vector<double>> &noise,
		__in const glm::uvec2 &dimension,
		__in const glm::vec2 &position,
		__in uint32_t octaves,
		__in_opt bool bicubic
		)
	{
		craft_random *inst = NULL;
		std::vector<double> rough, smooth;
		double alpha_x, alpha_y, blend_bottom, blend_top, frequency;
		uint32_t iter_oct = 0, iter_x, iter_y, period, sample_x0, sample_x1, sample_y0, sample_y1;

		if(!m_initialized) {
			THROW_CRAFT_RANDOM_EXCEPTION(CRAFT_RANDOM_EXCEPTION_UNINITIALIZED);
		}

		noise.clear();
		inst = craft_random::acquire();
		rough.resize(dimension.x * dimension.y, 0.0);

		

		for(iter_x = 0; iter_x < dimension.x; ++iter_x) {

			for(iter_y = 0; iter_y < dimension.y; ++iter_y) {
				rough.at(SCALAR_INDEX_2D(iter_x, iter_y, dimension.x)) = inst->generate_float();
			}
		}

		for(; iter_oct < octaves; ++iter_oct) {
			smooth.resize(dimension.x * dimension.y, 0.0);
			period = (1 << iter_oct);
			frequency = (1.0 / period);

			for(iter_x = 0; iter_x < dimension.x; ++iter_x) {
				sample_x0 = (iter_x / period) * period;
				sample_x1 = (sample_x0 + period) % dimension.x;
				alpha_x = (iter_x - sample_x0) * frequency;

				for(iter_y = 0; iter_y < dimension.y; ++iter_y) {
					sample_y0 = (iter_y / period) * period;
					sample_y1 = (sample_y0 + period) % dimension.y;
					alpha_y = (iter_y - sample_y0) * frequency;
					blend_top = interpolate_noise({rough.at(SCALAR_INDEX_2D(sample_x0, sample_y0, dimension.x)),
						rough.at(SCALAR_INDEX_2D(sample_x1, sample_y0, dimension.x)), alpha_x}, bicubic);
					blend_bottom = interpolate_noise({rough.at(SCALAR_INDEX_2D(sample_x0, sample_y1, dimension.x)),
						rough.at(SCALAR_INDEX_2D(sample_x1, sample_y1, dimension.x)), alpha_x}, bicubic);
					smooth.at(SCALAR_INDEX_2D(iter_x, iter_y, dimension.x)) = interpolate_noise(
						{blend_top, blend_bottom, alpha_y}, bicubic);
				}
			}

			noise.push_back(smooth);
			smooth.clear();
		}
	}

	void 
	_craft_perlin_2d::initialize(void)
	{

		if(m_initialized) {
			THROW_CRAFT_RANDOM_EXCEPTION(CRAFT_RANDOM_EXCEPTION_INITIALIZED);
		}

		m_initialized = true;
	}

	double 
	_craft_perlin_2d::interpolate_noise(
		__in const glm::vec3 &value,
		__in_opt bool bicubic
		)
	{
		double part, result;

		if(!m_initialized) {
			THROW_CRAFT_RANDOM_EXCEPTION(CRAFT_RANDOM_EXCEPTION_UNINITIALIZED);
		}

		if(bicubic) {
			part = (1.0 - std::cos(value.z * M_PI)) * 0.5;
			result = value.x * (1.0 - part) + value.y * part;
		} else {
			result = value.x * (1.0 - value.z) + value.z * value.y;
		}

		return result;
	}

	bool 
	_craft_perlin_2d::is_allocated(void)
	{
		return (craft_perlin_2d::m_instance != NULL);
	}

	bool 
	_craft_perlin_2d::is_initialized(void)
	{
		return m_initialized;
	}

	void 
	_craft_perlin_2d::to_file(
		__in const std::string &path,
		__in const std::vector<double> &noise,
		__in const glm::uvec2 &dimension,
		__in_opt bool colorize
		)
	{
		double value;
		std::stringstream stream;
		uint32_t iter_x, iter_y = 0;

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
			stream << PERLIN_SCALE_COLOR << std::endl;
		} else {
			stream << PERLIN_SCALE_GREYSCALE << std::endl;
		}

		for(; iter_y < dimension.y; ++iter_y) {

			for(iter_x = 0; iter_x < dimension.x; ++iter_x) {

				if(iter_x > 0) {
					stream << " ";
				}

				value = noise.at(SCALAR_INDEX_2D(iter_x, iter_y, dimension.x));

				if(colorize) {

					/**
					 * Height-map gradient
					 * ------------------
					 * 0		{0, 0, 125}
					 * 0.25		{0, 0, 255}
					 * 0.4		{0, 125, 255}
					 * 0.4625	{225, 230, 75}
					 * 0.485	{32, 160, 0}
					 * 0.6		{220, 220, 0}
					 * 0.75		{130, 130, 130}
					 * 1		{255, 255, 255}
					 */

					if(value < 0.25) {
						stream << "0 0 " << (uint32_t) (((255.0 - 125.0) * (value / 0.25)) + 125.0);
					} else if(value < 0.4) {
						stream << "0 " << (uint32_t) (125.0 * ((value - 0.25) / 0.15)) << " 255";
					} else if(value < 0.4625) {
						stream << (uint32_t) (225.0 * ((value - 0.4) / 0.0625)) <<  " " 
							<< (uint32_t) (((230.0 - 125.0) * ((value - 0.4) / 0.0625)) + 125.0) << " "
							<< (uint32_t) (255.0 - ((255.0 - 75.0) * ((value - 0.4) / 0.0625)));
					} else if(value < 0.485) {
						stream << (uint32_t) (225.0 - ((225.0 - 32.0) * ((value - 0.4625) / 0.0225))) << " "
							<< (uint32_t) (230.0 - ((230.0 - 160.0) * ((value - 0.4625) / 0.0225))) << " "
							<< (uint32_t) (75.0 - (75.0 * ((value - 0.4625) / 0.0225)));
					} else if(value < 0.6) {
						stream << (uint32_t) (((255.0 - 220.0) * ((value - 0.485) / 0.115)) + 32.0) << " "
							<< (uint32_t) (((255.0 - 220.0) * ((value - 0.485) / 0.115)) + 160.0) << " 0";
					} else if(value < 0.75) {
						stream << (uint32_t) (220.0 - ((220.0 - 130.0) * ((value - 0.6) / 0.15))) << " "
							<< (uint32_t) (220.0 - ((220.0 - 130.0) * ((value - 0.6) / 0.15))) << " "
							<< (uint32_t) (130.0 * ((value - 0.6) / 0.15));
					} else {
						stream << (uint32_t) (((255.0 - 130.0) * ((value - 0.75) / 0.25)) + 130.0) << " "
							<< (uint32_t) (((255.0 - 130.0) * ((value - 0.75) / 0.25)) + 130.0) << " "
							<< (uint32_t) (((255.0 - 130.0) * ((value - 0.75) / 0.25)) + 130.0);
					}
				} else {
					stream << (uint32_t) (PERLIN_SCALE_GREYSCALE * value);
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

		result << CRAFT_PERLIN_2D_HEADER << " (" << (m_initialized ? "INITIALIZED" : "UNINITIALIZED");

		if(verbose) {
			result << ", PTR. 0x" << SCALAR_AS_HEX(craft_perlin_2d *, this);
		}

		result << ")";

		return result.str();
	}

	void 
	_craft_perlin_2d::uninitialize(void)
	{

		if(!m_initialized) {
			THROW_CRAFT_RANDOM_EXCEPTION(CRAFT_RANDOM_EXCEPTION_UNINITIALIZED);
		}

		m_initialized = false;
	}

	_craft_random *_craft_random::m_instance = NULL;

	_craft_random::_craft_random(void) :
		m_initialized(false),
		m_instance_perlin_2d(craft_perlin_2d::acquire()),
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

	std::vector<double> 
	_craft_random::generate_perlin_2d(
		__out glm::uvec2 &dimension,
		__in const glm::vec2 &position,
		__in const glm::vec2 &offset,
		__in uint32_t octaves,
		__in double amplitude,
		__in double persistence,
		__in_opt bool bicubic
		)
	{

		if(!m_initialized) {
			THROW_CRAFT_RANDOM_EXCEPTION(CRAFT_RANDOM_EXCEPTION_UNINITIALIZED);
		}

		return m_instance_perlin_2d->generate(dimension, position, offset, octaves, 
			amplitude, persistence, bicubic);
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
		m_instance_perlin_2d->initialize();
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

		m_instance_perlin_2d->uninitialize();
		m_seed = 0;
		reset();
		m_initialized = false;
	}
}

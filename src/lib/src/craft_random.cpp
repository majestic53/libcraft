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
		__in uint32_t width,
		__in uint32_t height,
		__in uint32_t samples
		) :
			m_height(height),
			m_sample_count({0, 0}),
			m_samples(samples),
			m_width(width)
	{

		if(!m_height || !m_samples || !m_width) {
			THROW_CRAFT_RANDOM_EXCEPTION_FORMAT(CRAFT_RANDOM_EXCEPTION_INVALID_DIMENSIONS,
				"{%lu, %lu}, %lu", width, height, samples);
		}
	}

	_craft_perlin_2d::_craft_perlin_2d(
		__in const _craft_perlin_2d &other
		) :
			m_gradient(other.m_gradient),
			m_height(other.m_height),
			m_sample_count(other.m_sample_count),
			m_sample_value(other.m_sample_value),
			m_samples(other.m_samples),
			m_width(other.m_width)
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
			m_gradient = other.m_gradient;
			m_height = other.m_height;
			m_sample_count = other.m_sample_count;
			m_sample_value = other.m_sample_value;
			m_samples = other.m_samples;
			m_width = other.m_width;
		}

		return *this;
	}

	void 
	_craft_perlin_2d::clear(void)
	{
		m_gradient.clear();
		m_sample_count = {0, 0};
		m_sample_value.clear();
	}

	_craft_perlin_2d 
	_craft_perlin_2d::generate(
		__in uint32_t width,
		__in uint32_t height,
		__in uint32_t samples
		)
	{
		craft_perlin_2d result(width, height, samples);

		result.run();

		return result;
	}

	uint32_t 
	_craft_perlin_2d::height(void)
	{
		return m_height;
	}

	void 
	_craft_perlin_2d::initialize_vectors(void)
	{
		size_t count = 0;		
		craft_random *inst = NULL;
		glm::vec2 current = {0.0, 0.0}, delta;
		std::vector<glm::vec3>::iterator sample_iter;
		std::vector<glm::vec2>::iterator gradient_iter;

		m_sample_count.x = m_width * m_samples;
		m_sample_count.y = m_height * m_samples;

		inst = craft_random::acquire();
		m_gradient.resize((m_width + 1) * (m_height + 1), {0.0, 0.0});

		for(gradient_iter = m_gradient.begin(); gradient_iter != m_gradient.end(); ++gradient_iter) {
			gradient_iter->x = inst->generate_float();
			gradient_iter->y = inst->generate_float();
		}

		m_sample_value.resize(m_sample_count.x * m_sample_count.y, {0.0, 0.0, 0.0});
		delta.x = m_width / (double) (m_sample_count.x - 1.0);
		delta.y = m_height / (double) (m_sample_count.y - 1.0);

		for(sample_iter = m_sample_value.begin(); sample_iter != m_sample_value.end(); 
				++sample_iter, ++count, ++current.x) {

			if(count == m_sample_count.x) {
				current.x = 0.0;
				++current.y;
				count = 0;
			}

			sample_iter->x = current.x * delta.x;
			sample_iter->y = current.y * delta.y;
		}
	}

	double 
	_craft_perlin_2d::interpolate(
		__in const glm::vec2 &position,
		__in const glm::uvec2 &lattice,
		__in double value11,
		__in double value12,
		__in double value21,
		__in double value22
		)
	{

		return (value11 * ((lattice.x + 1.0) - position.x) * (lattice.y - position.y) 
			+ value21 * (position.x - lattice.x) * (lattice.y - position.y) 
			+ value12 * ((lattice.x + 1.0) - position.x) * (position.y - (lattice.y + 1.0)) 
			+ value22 * (position.x - lattice.x) * (position.y - (lattice.y + 1.0)));
	}

	void 
	_craft_perlin_2d::run(void)
	{
		size_t count = 0;
		glm::uvec2 lattice;
		std::vector<glm::vec3>::iterator iter;
		glm::vec2 current = {0.0, 0.0}, delta;

		clear();
		initialize_vectors();

		for(iter = m_sample_value.begin(); iter != m_sample_value.end(); 
				++iter, ++count, ++current.x) {

			if(count == m_sample_count.x) {
				current.x = 0.0;
				++current.y;
				count = 0;
			}

			lattice.x = (uint32_t) iter->x;
			lattice.y = (uint32_t) iter->y;

			if(iter->x && !std::fmod(iter->x, 1.0)) {
				--lattice.x;
			}

			if(iter->y && !std::fmod(iter->y, 1.0)) {
				--lattice.y;
			}

			delta.x = iter->x - lattice.x;
			delta.y = iter->y - lattice.y;
			iter->z = interpolate({iter->x, iter->y}, lattice, 
				glm::dot({-delta.x, 1.0 - delta.y}, m_gradient.at(((lattice.y + 1) * m_width) + lattice.x)),
				glm::dot({-delta.x, -delta.y}, m_gradient.at((lattice.y * m_width) + lattice.x)),
				glm::dot({1.0 - delta.x, 1.0 - delta.y}, m_gradient.at(((lattice.y + 1) * m_width) + (lattice.x + 1))),
				glm::dot({1.0 - delta.x, -delta.y}, m_gradient.at((lattice.y * m_width) + (lattice.x + 1))));
		}
	}

	uint32_t 
	_craft_perlin_2d::samples(void)
	{
		return m_samples;
	}

	void 
	_craft_perlin_2d::to_file(
		__in const std::string &path,
		__in_opt bool colorize
		)
	{
		size_t count = 0;
		double normalized;
		std::stringstream stream;
		std::vector<glm::vec3>::iterator iter;
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

		stream << m_sample_count.x << " " << m_sample_count.y << std::endl;

		if(colorize) {
			stream << 255 << std::endl;
		} else {
			stream << 128 << std::endl;
		}

		for(iter = m_sample_value.begin(); iter != m_sample_value.end(); 
				++iter, ++count) {
			normalized = ((iter->z + 1.0) * 0.5);

			if(count == m_sample_count.x) {
				stream << std::endl;
				count = 0;
			}

			if(count > 0) {
				stream << " ";
			}

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

		file.write(stream.str().c_str(), stream.str().size());
		file.close();
	}

	std::string 
	_craft_perlin_2d::to_string(
		__in_opt bool verbose
		)
	{
		size_t count;
		std::stringstream result;
		std::vector<glm::vec3>::iterator sample_iter;
		std::vector<glm::vec2>::iterator gradient_iter;

		result << CRAFT_PERLIN_HEADER << " ({" << m_width << ", " << m_height << "}, SAMP. " 
			<< m_samples << ", GRAD. " << m_gradient.size() << ", SAMPOS. {" << m_sample_count.x 
			<< ", " << m_sample_count.y << "}, " << m_sample_value.size() << ")";

		if(verbose) {

			for(gradient_iter = m_gradient.begin(), count = 0; gradient_iter != m_gradient.end(); 
					++gradient_iter, ++count) {

				if(gradient_iter != m_gradient.begin()) {
					result << ", ";
				}

				if((gradient_iter == m_gradient.begin()) || (count == m_width)) {
					result << std::endl;
					count = 0;
				}

				result << "{" << gradient_iter->x << ", " << gradient_iter->y << "}";
			}

			result << std::endl;

			for(sample_iter = m_sample_value.begin(), count = 0; sample_iter != m_sample_value.end(); 
					++sample_iter, ++count) {

				if(sample_iter != m_sample_value.begin()) {
					result << ", ";
				}

				if((sample_iter == m_sample_value.begin()) || (count == m_sample_count.x)) {
					result << std::endl;
					count = 0;
				}

				result << "{" << sample_iter->x << ", " << sample_iter->y << "}";
			}

			result << std::endl;

			for(sample_iter = m_sample_value.begin(), count = 0; sample_iter != m_sample_value.end(); 
					++sample_iter, ++count) {

				if(sample_iter != m_sample_value.begin()) {
					result << ", ";
				}

				if((sample_iter == m_sample_value.begin()) || (count == m_sample_count.x)) {
					result << std::endl;
					count = 0;
				}

				result << sample_iter->z;
			}
		}

		return result.str();
	}

	uint32_t 
	_craft_perlin_2d::width(void)
	{
		return m_width;
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

	craft_perlin_2d 
	_craft_random::generate_perlin_2d(
		__in uint32_t width,
		__in uint32_t height,
		__in uint32_t samples
		)
	{

		if(!m_initialized) {
			THROW_CRAFT_RANDOM_EXCEPTION(CRAFT_RANDOM_EXCEPTION_UNINITIALIZED);
		}

		return craft_perlin_2d::generate(width, height, samples);
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

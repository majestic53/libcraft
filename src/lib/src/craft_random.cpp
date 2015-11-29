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
		std::vector<glm::vec2>::iterator gradient_iter;
		double current_x = 0, current_y = 0, delta_x, delta_y;
		std::vector<std::pair<glm::vec2, double>>::iterator sample_iter;

		m_sample_count.x = m_width * m_samples;
		m_sample_count.y = m_height * m_samples;

		inst = craft_random::acquire();
		m_gradient.resize(m_width * m_height, {0.0, 0.0});

		for(gradient_iter = m_gradient.begin(); gradient_iter != m_gradient.end(); ++gradient_iter) {
			gradient_iter->x = inst->generate_float();
			gradient_iter->y = inst->generate_float();
		}

		m_sample_value.resize(m_sample_count.x * m_sample_count.y, 
			std::pair<glm::vec2, double>({0.0, 0.0}, 0.0));
		delta_x = m_width / (double) (m_sample_count.x - 1);
		delta_y = m_height / (double) (m_sample_count.y - 1);

		for(sample_iter = m_sample_value.begin(); sample_iter != m_sample_value.end(); 
				++sample_iter, ++count, ++current_x) {

			if(count == m_sample_count.x) {
				current_x = 0.0;
				++current_y;
				count = 0;
			}

			sample_iter->first.x = current_x * delta_x;
			sample_iter->first.y = current_y * delta_y;
		}
	}

	void 
	_craft_perlin_2d::run(void)
	{
		size_t count = 0;
		double current_x = 0, current_y = 0;
		std::vector<std::pair<glm::vec2, double>>::iterator sample_iter;

		clear();
		initialize_vectors();

		for(sample_iter = m_sample_value.begin(); sample_iter != m_sample_value.end(); 
				++sample_iter, ++count, ++current_x) {

			if(count == m_sample_count.x) {
				current_x = 0.0;
				++current_y;
				count = 0;
			}

			// TODO: run calculation on [current_x][current_y] and place result under iter->second
		}
	}

	uint32_t 
	_craft_perlin_2d::samples(void)
	{
		return m_samples;
	}

	std::string 
	_craft_perlin_2d::to_string(
		__in_opt bool verbose
		)
	{
		size_t count;
		std::stringstream result;
		std::vector<glm::vec2>::iterator gradient_iter;
		std::vector<std::pair<glm::vec2, double>>::iterator sample_iter;

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

				result << "{" << sample_iter->first.x << ", " << sample_iter->first.y << "}";
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

				result << sample_iter->second;
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

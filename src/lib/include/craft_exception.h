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

#ifndef CRAFT_EXCEPTION_H_
#define CRAFT_EXCEPTION_H_

#include <stdexcept>

namespace CRAFT {

	#define EXCEPTION_HEADER "<EXCEPTION>"
	#define EXCEPTION_UNKNOWN "<EXCEPTION_UNKNOWN>"

	#define THROW_EXCEPTION(_EXCEPT_) \
		_craft_exception::generate(_EXCEPT_, __FUNCTION__, __FILE__, __LINE__, \
		NULL)
	#define THROW_EXCEPTION_FORMAT(_EXCEPT_, _FORMAT_, ...) \
		_craft_exception::generate(_EXCEPT_, __FUNCTION__, __FILE__, __LINE__, \
		_FORMAT_, __VA_ARGS__)

	typedef class _craft_exception : 
			public std::runtime_error {

		public:

			_craft_exception(
				__in const std::string &message,
				__in_opt const std::string &function = std::string(),
				__in_opt const std::string &file = std::string(),
				__in_opt size_t line = 0
				);

			_craft_exception(
				__in const _craft_exception &other
				);

			virtual ~_craft_exception(void);

			_craft_exception &operator=(
				__in const _craft_exception &other
				);

			std::string file(void);

			std::string function(void);

			static void generate(
				__in const std::string &message,
				__in const std::string &function,
				__in const std::string &file,
				__in size_t line,
				__in const char *format,
				...
				);

			size_t line(void);

			virtual std::string to_string(
				__in_opt bool verbose = false
				);

		protected:

			std::string m_file;

			std::string m_function;

			size_t m_line;

	} craft_exception;
}

#endif // CRAFT_EXCEPTION_H_

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

#include "../lib/include/craft.h"

int 
main(void) 
{
	int result = 0;
	craft *instance = NULL;

	std::cout << craft::version() << std::endl;

	try {
		instance = craft::acquire();
		instance->initialize();
		instance->start();
		instance->uninitialize();
	} catch(craft_exception &exc) {
		std::cerr << exc.to_string(true) << std::endl;
		result = SCALAR_INVALID(int);
		goto exit;
	} catch(std::runtime_error &exc) {
		std::cerr << exc.what() << std::endl;
		result = SCALAR_INVALID(int);
		goto exit;
	}

exit:
	return result;
}

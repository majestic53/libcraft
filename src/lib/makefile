# libcraft
# Copyright (C) 2015 David Jolly
# ----------------------
#
# libcraft is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# libcraft is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.

CC=clang++
CC_FLAGS=-march=native -pthread -std=gnu++11 -O3 -Wall -Werror
DIR_BIN=./../../bin/
DIR_BUILD=./../../build/
DIR_INC=./include/
DIR_SRC=./src/
LIB=libcraft.a

all: build archive

archive:
	@echo ''
	@echo '--- BUILDING LIBRARY -----------------------'
	ar rcs $(DIR_BUILD)$(LIB) $(DIR_BUILD)craft.o $(DIR_BUILD)craft_exception.o
	@echo '--- DONE -----------------------------------'
	@echo ''

build: craft.o craft_exception.o

craft.o: $(DIR_SRC)craft.cpp $(DIR_INC)craft.h
	$(CC) $(CC_FLAGS) -c $(DIR_SRC)craft.cpp -o $(DIR_BUILD)craft.o

craft_exception.o: $(DIR_SRC)craft_exception.cpp $(DIR_INC)craft_exception.h
	$(CC) $(CC_FLAGS) -c $(DIR_SRC)craft_exception.cpp -o $(DIR_BUILD)craft_exception.o

# COMPONENTS

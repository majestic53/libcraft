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
	ar rcs $(DIR_BIN)$(LIB) $(DIR_BUILD)craft.o $(DIR_BUILD)craft_camera.o $(DIR_BUILD)craft_display.o $(DIR_BUILD)craft_exception.o \
		$(DIR_BUILD)craft_gl.o $(DIR_BUILD)craft_keyboard.o $(DIR_BUILD)craft_mouse.o
	@echo '--- DONE -----------------------------------'
	@echo ''

build: craft.o craft_camera.o craft_display.o craft_exception.o craft_gl.o craft_keyboard.o craft_mouse.o

craft.o: $(DIR_SRC)craft.cpp $(DIR_INC)craft.h
	$(CC) $(CC_FLAGS) -c $(DIR_SRC)craft.cpp -o $(DIR_BUILD)craft.o

craft_exception.o: $(DIR_SRC)craft_exception.cpp $(DIR_INC)craft_exception.h
	$(CC) $(CC_FLAGS) -c $(DIR_SRC)craft_exception.cpp -o $(DIR_BUILD)craft_exception.o

craft_gl.o: $(DIR_SRC)craft_gl.cpp $(DIR_INC)craft_gl.h
	$(CC) $(CC_FLAGS) -c $(DIR_SRC)craft_gl.cpp -o $(DIR_BUILD)craft_gl.o

# COMPONENTS

craft_camera.o: $(DIR_SRC)craft_camera.cpp $(DIR_INC)craft_camera.h
	$(CC) $(CC_FLAGS) -c $(DIR_SRC)craft_camera.cpp -o $(DIR_BUILD)craft_camera.o

craft_display.o: $(DIR_SRC)craft_display.cpp $(DIR_INC)craft_display.h
	$(CC) $(CC_FLAGS) -c $(DIR_SRC)craft_display.cpp -o $(DIR_BUILD)craft_display.o

craft_keyboard.o: $(DIR_SRC)craft_keyboard.cpp $(DIR_INC)craft_keyboard.h
	$(CC) $(CC_FLAGS) -c $(DIR_SRC)craft_keyboard.cpp -o $(DIR_BUILD)craft_keyboard.o

craft_mouse.o: $(DIR_SRC)craft_mouse.cpp $(DIR_INC)craft_mouse.h
	$(CC) $(CC_FLAGS) -c $(DIR_SRC)craft_mouse.cpp -o $(DIR_BUILD)craft_mouse.o

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
CC_FLAGS_GL=-lGLEW -lGLU -lGL -lglfw -lSDL2 -lSDL2_mixer -lglut -lfreetype
DIR_BIN=./../../bin/
DIR_BUILD=./../../build/
DIR_INC=./include/
DIR_SRC=./src/
EXE=craft
LIB=libcraft.a

all: exe

exe:
	@echo ''
	@echo '--- BUILDING EXE ---------------------------' 
	$(CC) $(CC_FLAGS) $(CC_FLAGS_GL) main.cpp $(DIR_BIN)$(LIB) -o $(DIR_BIN)$(EXE)
	@echo '--- DONE -----------------------------------'
	@echo ''

#           todo - Light TODO list
#          Copyright (C)  2012  Ash Harley
#
#This library is free software; you can redistribute it and/or
#modify it under the terms of the GNU General Public
#License as published by the Free Software Foundation; either
#version 3.0 of the License, or (at your option) any later version.
#
#This library is distributed in the hope that it will be useful,
#but WITHOUT ANY WARRANTY; without even the implied warranty of
#MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
#General Public License for more details.
#
#You should have received a copy of the GNU General Public
#License along with this library; if not, write to the Free Software
#Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA

CC=gcc
CFLAGS=-lsqlite3 -O3
SRC=todo.c
CSRC=cctodo.c
CLIBS=ctodo.c
LIBS=todo.a
INCLUDES = -I .
INSTALL   ?= install
MKDIR     ?= $(INSTALL) -d
BINDIR    ?= $(PREFIX)/bin
DESTDIR   ?=

all: cctodo

todo:	$(SRC)
	$(CC) -o todo $^ $(CFLAGS)

cctodo:	$(CSRC) | ctodo
	$(CC) ${LIBS} -o todo $^ $(CFLAGS)

ctodo:	$(CLIBS)
	$(CC) ${INCLUDES} -c -o ${LIBS} $^ $(CFLAGS)

.PHONY: clean all

clean:
	rm -f todo.a
	rm -f todo
	find . -name '*~' -delete;

install:
	$(MKDIR) $(DESTDIR)$(BINDIR)
	$(INSTALL) todo$(EXE) $(DESTDIR)$(BINDIR)/

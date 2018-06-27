#           ctodo - Light TODO list
#       Copyright (C)  2012-2018 Cynede
#

CFLAGS=-lsqlite3 -O3
CSRC=cctodo.c
CLIBS=ctodo.c
LIBS=todo.o
INCLUDES = -I .
INSTALL   ?= install
MKDIR     ?= $(INSTALL) -d
BINDIR    ?= $(PREFIX)/bin
DESTDIR   ?=

all: cctodo

rebuild: clean | all

cctodo: $(CSRC) | ctodo
	$(CC) ${INCLUDES} -o todo $^ ${CFLAGS} ${LIBS}

ctodo: $(CLIBS)
	$(CC) ${INCLUDES} -c -o ${LIBS} $^ $(CFLAGS)

.PHONY: clean all rebuild inheritsqlite

clean:
	@echo "Cleaning binaries..."
	rm -f ${LIBS}
	rm -f todo
	@echo "Cleaning temp files..."
	find . -name '*~' -delete;
	find . -name '#*#' -delete;
	@echo "Cleaning complete."

install:
	$(MKDIR) $(DESTDIR)$(BINDIR)
	$(INSTALL) todo$(EXE) $(DESTDIR)$(BINDIR)/

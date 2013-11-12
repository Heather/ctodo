#           todo - Light TODO list
#       Copyright (C)  2012-2013 Heather
#

CC=gcc
CFLAGS=-lsqlite3 -O3
XCFLAGS=-lpthread -ldl -O3
CSRC=cctodo.c
CLIBS=ctodo.c
XCSRC=contrib/sqlite/sqlite3.c ctodo.c cctodo.c
LIBS=todo.o
INCLUDES = -I .
XINCLUDES = -I contrib/sqlite
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

inheritsqlite: $(XCSRC)
	$(CC) ${XINCLUDES} -o todo $^ ${XCFLAGS}

.PHONY: clean all rebuild inheritsqlite

clean:
	@echo " --- Clean binaries --- "
	rm -f ${LIBS}
	rm -f todo
	@echo " --- Clean temp files --- "
	find . -name '*~' -delete;
	find . -name '#*#' -delete;

install:
	$(MKDIR) $(DESTDIR)$(BINDIR)
	$(INSTALL) todo$(EXE) $(DESTDIR)$(BINDIR)/

CC=gcc
CFLAGS=-lsqlite3 -O3
SRC=todo.c

INSTALL   ?= install
MKDIR     ?= $(INSTALL) -d
BINDIR    ?= $(PREFIX)/bin
MODDIR    ?= $(PREFIX)/share/todo
DESTDIR   ?=

todo: 	$(SRC)
	$(CC) -o todo $^ $(CFLAGS)

.PHONY: clean

clean:
	rm -f todo

install:
	$(MKDIR) $(DESTDIR)$(BINDIR)
	$(INSTALL) todo$(EXE) $(DESTDIR)$(BINDIR)/
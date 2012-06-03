CC=gcc
CFLAGS=-lsqlite3 -O3
SRC=todo.c

INSTALL   ?= install
MKDIR     ?= $(INSTALL) -d
BINDIR    ?= $(PREFIX)/bin

todo: 	$(SRC)
	$(CC) -o todo $^ $(CFLAGS)

.PHONY: clean

clean:
	rm -f todo

install:
	$(MKDIR) $(BINDIR)
	$(INSTALL) todo $(BINDIR)/
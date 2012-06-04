CC=gcc
CFLAGS=-lsqlite3 -O3
SRC=todo.c

todo: 	$(SRC)
	$(CC) -o todo $^ $(CFLAGS)

.PHONY: clean

clean:
	rm -f todo

install:
	make clean
	make
	install todo${EXE} /bin/todo

CC=gcc
CFLAGS=-lpolarssl -lsqlite3
SRC=sholy.c

sholy: $(SRC)
	$(CC) -o sholy $^ $(CFLAGS)

.PHONY: clean

clean:
	rm -f sholy
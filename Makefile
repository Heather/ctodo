CC=gcc
CFLAGS=-lpolarssl -lsqlite3 -O3
SRC=sholy.c

sholy: $(SRC)
	$(CC) -o sholy $^ $(CFLAGS)

.PHONY: clean

clean:
	rm -f sholy
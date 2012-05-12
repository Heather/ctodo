CC=gcc
CFLAGS=-lpolarssl
SRC=sholy.c

sholy: $(SRC)
	$(CC) -o sholy $^ $(CFLAGS)

.PHONY: clean

clean:
	rm -f sholy
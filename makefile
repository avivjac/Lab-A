
.PHONY: all clean run

all: encoder

encoder: encoder.c
	gcc -g -m32 -Wall -o encoder encoder.c

clean:
	rm -f encoder
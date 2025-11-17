all: build

build: numbers.o add.o main.o encoder.o
	gcc -g -m32 -Wall -o build numbers.o add.o main.o encoder.o

numbers.o: numbers.c
	gcc -g -m32 -Wall -c -o numbers.o numbers.c

add.o: add.s
	nasm -g -f elf -w+all -o add.o add.s

main.o: main.c
	gcc -g -m32 -Wall -c -o main.o main.c

encoder: encoder.c
	gcc -g -m32 -Wall -c -o encoder encoder.c

.PHONY : clean

clean:
	rm -f *.o build
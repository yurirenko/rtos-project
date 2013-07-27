all: main

main: main.o array.o parser.o
	gcc main.o array.o parser.o -o main

main.o: main.c
	gcc -D_GNU_SOURCE -Wall -c main.c

array.o: array.c
	gcc -Wall -c array.c

parser.o: parser.c
	gcc -Wall -c parser.c

clean:
	rm -rf *o main

all: main

main: main.o array.o
	gcc main.o array.o -o main

main.o: main.c
	gcc -Wall -c main.c

array.o: array.c
	gcc -Wall -c array.c

clean:
	rm -rf *o main

all: main

main: main.o array.o parser.o cyclic.o utils.o threaded.o
	gcc main.o array.o parser.o cyclic.o utils.o threaded.o -o main  -lm 

main.o:
	gcc -D_GNU_SOURCE -Wall -c main.c

array.o:
	gcc -Wall -c lib/array.c

parser.o:
	gcc -Wall -c lib/parser.c

cyclic.o:
	gcc -Wall -c lib/cyclic.c

utils.o:
	gcc -Wall -c lib/utils.c

threaded.o:
	gcc -Wall -c lib/threaded.c

clean:
	rm -rf *o main

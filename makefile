all: flip clean

flip: img.o flip.o main.o
	gcc main.o flip.o img.o -o flippic

main.o: main.c
	gcc -Wall -std=c99 -c main.c

flip.o: flip.c flip.h
	gcc -Wall -std=c99 -c flip.c

img.o: img.c img.h
	gcc -Wall -std=c99 -c img.c

clean:
	rm -f *.o

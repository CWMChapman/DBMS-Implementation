CC=gcc
FLAGS=-Wall -g

all: tree

tree: tree.c tree.h pageManager.o
	$(CC) $(FLAGS) -o tree.e tree.c pageManager.o

pmtest: pageManagerTest.c pageManager.o
	$(CC) $(FLAGS) -o pmtest.e pageManagerTest.c pageManager.o

pageManager.o: pageManager.c pageManager.h
	$(CC) $(FLAGS) -c pageManager.c

clean:
	rm *.e
	rm *.o
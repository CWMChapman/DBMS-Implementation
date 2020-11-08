CC=gcc
FLAGS=-Wall

BPTNode: BPT.c BPT.h pageManager.o
	$(CC) $(FLAGS) -o BPT.e BPT.c pageManager.o

hash: linearHash.c linearHash.h pageManager.o
	$(CC) $(FLAGS) -o linearHash.e linearHash.c pageManager.o

pmtest: pageManagerTest.c pageManager.o
	$(CC) $(FLAGS) -o pmtest.e pageManagerTest.c pageManager.o

pageManager.o: pageManager.c pageManager.h
	$(CC) $(FLAGS) -c pageManager.c

clean:
	rm linearHash.e

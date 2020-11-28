CC=gcc
FLAGS=-Wall -g

all: tree

tree: tree.c tree.h treeHelper.o pageManager.o recVec.o
	$(CC) $(FLAGS) -o tree.e tree.c treeHelper.o pageManager.o recVec.o

hash: linearHash.c linearHash.h pageManager.o recVec.o
	$(CC) $(FLAGS) -o linearHash.e linearHash.c pageManager.o recVec.o

pmtest: pageManagerTest.c pageManager.o
	$(CC) $(FLAGS) -o pmtest.e pageManagerTest.c pageManager.o

treeHelper.o: treeHelper.c treeHelper.h pageManager.o
	$(CC) $(FLAGS) -c treeHelper.c

pageManager.o: pageManager.c pageManager.h
	$(CC) $(FLAGS) -c pageManager.c

recVec.o: recVec.c recVec.h pageManager.h
	$(CC) $(FLAGS) -c recVec.c

clean:
	rm *.e
	rm *.o
	rm -r *.dSYM

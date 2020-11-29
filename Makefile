CC=gcc
FLAGS=-Wall -g

all: treeBench

treeBench: treeBench.c tree.o treeHelper.o pageManager.o recVec.o
	$(CC) $(FLAGS) -o treeBench.e treeBench.c tree.o treeHelper.o pageManager.o recVec.o

tree.o: tree.c tree.h recVec.o
	$(CC) $(FLAGS) -c tree.c recVec.o

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

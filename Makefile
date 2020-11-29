CC=gcc
FLAGS=-Wall

all: benchmark

benchmark: benchmark.c tree.o treeHelper.o pageManager.o recVec.o linearHash.o
	$(CC) $(FLAGS) -o benchmark.e benchmark.c tree.o treeHelper.o linearHash.o pageManager.o recVec.o

tree.o: tree.c tree.h
	$(CC) $(FLAGS) -c tree.c

linearHash.o: linearHash.c linearHash.h 
	$(CC) $(FLAGS) -c linearHash.c

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

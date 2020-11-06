CC=gcc
FLAGS=-Wall

BPTNode: BPTNode.c BPTNode.h pageManager.o
	$(CC) $(FLAGS) -o BPTNode.e BPTNode.c pageManager.o

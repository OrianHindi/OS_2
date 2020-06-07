CC=gcc
FLAGS=-g -Wall


all: ipc 2.1.1 2.2.1 2.3.1


ipc: IPC.c
	$(CC) $(FLAGS) -o $@ $<

2.1.1: 2.1.1.c
	$(CC) $(FLAGS) -o $@ $< -pthread
 
2.2.1: 2.2.1.c
	$(CC) $(FLAGS) -o $@ $< -pthread

2.3.1: 2.3.1.c
	$(CC) $(FLAGS) -o $@ $< -pthread


.PHONY: clean all


clean:
	rm ipc 2.1.1 2.2.1 2.3.1










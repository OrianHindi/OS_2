CC=gcc
FLAGS=-g -Wall


all: ipc 2.1.1 2.1.2 2.2.1 2.2.2 2.3.1 2.3.2


ipc: IPC.c
	$(CC) $(FLAGS) -o $@ $<

2.1.1: 2.1.1.c
	$(CC) $(FLAGS) -o $@ $< -pthread

2.1.2: 2.1.2.c
	$(CC) $(FLAGS) -o $@ $< -pthread
 
2.2.1: 2.2.1.c
	$(CC) $(FLAGS) -o $@ $< -pthread

2.2.2: 2.2.2.c
	$(CC) $(FLAGS) -o $@ $< -pthread

2.3.1: 2.3.1.c
	$(CC) $(FLAGS) -o $@ $< -pthread

2.3.2: 2.3.2.c
	$(CC) $(FLAGS) -o $@ $< -pthread


.PHONY: clean all


clean:
	rm -f ipc 2.1.1 2.1.2 2.2.1 2.2.2 2.3.1 2.3.2










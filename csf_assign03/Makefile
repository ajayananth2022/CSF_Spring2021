# Homework 3
CC = g++
CONSERVATIVE_FLAGS = -std=c++11 -Wall -Wextra -pedantic
DEBUGGING_FLAGS = -g -O0
CFLAGS = $(CONSERVATIVE_FLAGS) $(DEBUGGING_FLAGS)

csim: main.o csim.o
	$(CC) -o csim main.o csim.o

main.o: main.cpp csim.h
	$(CC) $(CFLAGS) -c main.cpp

csim.o: csim.cpp csim.h
	$(CC) $(CFLAGS) -c csim.cpp

clean:
	rm -f *.o csim *~

# Homework 3
GCC = gcc
CONSERVATIVE_FLAGS = -std=c99 -Wall -Wextra -pedantic
DEBUGGING_FLAGS = -g -O0
CFLAGS = $(CONSERVATIVE_FLAGS) $(DEBUGGING_FLAGS)

csim: main.o csim.o
	$(GCC) -o csim main.o csim.o

test: test_csim.o csim.o
	$(GCC) -o test test_csim.o csim.o

main.o: main.c csim.h
	$(GCC) $(CFLAGS) -c main.c

test_csim.o: test_csim.c csim.h
	$(GCC) $(CFLAGS) -c test_csim.c

csim.o: csim.c csim.h
	$(GCC) $(CFLAGS) -c csim.c

clean:
	rm -f *.o csim *~

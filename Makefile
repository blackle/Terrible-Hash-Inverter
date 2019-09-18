CC = gcc
CFLAGS = -O3 -std=c11 -march=native
LINKER = gcc
LFLAGS = -fuse-ld=gold -pthread
OBJS = sha2.o isaac64.o basilisk.o

all : gen benchmark

gen : gen.o $(OBJS)
	$(LINKER) $(LFLAGS) -o $@ $^

benchmark : benchmark.o $(OBJS)
	$(LINKER) $(LFLAGS) -o $@ $^

%.o : %.c %.h Makefile
	$(CC) $(CFLAGS) -c $*.c

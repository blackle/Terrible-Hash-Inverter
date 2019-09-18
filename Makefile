CC = gcc
CFLAGS = -O3 -std=c11 -march=native
LINKER = gcc
LFLAGS = -fuse-ld=gold -pthread
OBJS = sha2.o basilisk.o sha2_sse4.o

all : gen benchmark

gen : gen.o $(OBJS)
	$(LINKER) $(LFLAGS) -o $@ $^

benchmark : benchmark.o $(OBJS)
	$(LINKER) $(LFLAGS) -o $@ $^

sha2_sse4.o : sha2_sse4.asm Makefile
	yasm -f x64 -f elf64 -X gnu -g dwarf2 -D LINUX -o sha2_sse4.o sha2_sse4.asm

%.o : %.c %.h Makefile
	$(CC) $(CFLAGS) -c $*.c


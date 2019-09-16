CC = gcc
CFLAGS = -g -O3 -std=c11
LINKER = gcc
LFLAGS = -fuse-ld=gold -pthread
OBJS = gen.o sha2.o isaac64.o

gen : $(OBJS)
	$(LINKER) $(LFLAGS) -o $@ $^

%.o : %.c %.h Makefile
	$(CC) $(CFLAGS) -c $*.c

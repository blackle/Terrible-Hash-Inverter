CC = gcc
CFLAGS = -O3 -std=c11 -march=native
LINKER = gcc
LFLAGS = -fuse-ld=gold -pthread
OBJS = gen.o sha2.o isaac64.o

gen : $(OBJS)
	$(LINKER) $(LFLAGS) -o $@ $^

%.o : %.c %.h Makefile
	$(CC) $(CFLAGS) -c $*.c

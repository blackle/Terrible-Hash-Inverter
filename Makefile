CC = gcc
CFLAGS = -O3 -std=c11 -march=native -Wall -Wextra -Werror -flto
LINKER = gcc
LFLAGS = -pthread -flto
OBJS = basilisk.o crypto/sha2.o crypto/sha256_sse4.o crypto/sha256_avx1.o crypto/sha256_avx2_rorx2.o crypto/sha256_ishaext.o

UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Darwin)
	OS = OSX
else
	OS = LINUX
endif

all : gen benchmark

gen : gen.o $(OBJS)
	$(LINKER) $(LFLAGS) -o $@ $^

benchmark : benchmark.o $(OBJS)
	$(LINKER) $(LFLAGS) -o $@ $^

sha256_%.o : sha256_%.asm
ifeq ($(OS),OSX)
	yasm -f x64 -f macho64 -D LINUX -o $@ $^
else
	yasm -f x64 -f elf64 -X gnu -g dwarf2 -D LINUX -o $@ $^
endif

%.o : %.c %.h Makefile
	$(CC) $(CFLAGS) -c -o $*.o $*.c

clean :
	rm gen benchmark *.o crypto/*.o

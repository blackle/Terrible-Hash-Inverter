CC = arm-linux-gnueabihf-gcc
CFLAGS = -O3 -std=c11 -mfpu=neon-vfpv4 -Wall -Wextra -Werror -flto
LINKER = arm-linux-gnueabihf-gcc
LFLAGS = -fuse-ld=gold -pthread -flto
OBJS = basilisk.o crypto/sha2.o crypto/sha256-armv4.o

all : gen benchmark

gen : gen.o $(OBJS)
	$(LINKER) $(LFLAGS) -o $@ $^

benchmark : benchmark.o $(OBJS)
	$(LINKER) $(LFLAGS) -o $@ $^

sha256_%.o : sha256_%.asm
	yasm -f x64 -f elf64 -X gnu -g dwarf2 -D LINUX -o $@ $^

crypto/sha256-armv4.o : crypto/sha256-armv4.S
	$(CC) $(CFLAGS) -c -o $*.o $*.S

%.o : %.c %.h Makefile
	$(CC) $(CFLAGS) -c -o $*.o $*.c

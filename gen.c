#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/prctl.h>
#include <fcntl.h>
#include <stdint.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include "basilisk.h"

static const char hex[] = "0123456789abcdef";

void find_basilisk(int pipe, int n) {
	basilisk_ctx basilisk;
	basilisk_init(&basilisk, n);

	while (1) {
		basilisk_step(&basilisk);

		if (basilisk.output[0] == 0 && basilisk.output[1] == 0 && basilisk.output[2] == 0 /*&& basilisk.output[3] == 0*/) {
			break;
		}
	}
	char *hexdump = basilisk.data + 85;
	for (int i = 0; i < 64/2; i++) {
		for (int j = 0; j < 2; j++) {
			int idx = i*2+j;
			int value = (j == 0) ? basilisk.output[i] / 16 : basilisk.output[i] % 16;
			hexdump[idx] = hex[value];
		}
	}
	write(pipe, basilisk.data, BASILISK_LENGTH);
}

#define POOLSIZE 8

int main(int argc, char** argv) {
	int pipes[2];
	pipe((int*)&pipes);
	if (argc != 2) {
		return -1;
	}
	int number = atoi(argv[1]);

	for (int i = 0; i < POOLSIZE; i++) {
		if (fork() == 0) {
			prctl(PR_SET_PDEATHSIG, SIGHUP);
			close(pipes[0]);
			find_basilisk(pipes[1], number);
			return 0;
		}
	}
	char winning_basilisk[BASILISK_LENGTH];
	read(pipes[0], winning_basilisk, BASILISK_LENGTH);
	printf(winning_basilisk);
	printf("\n");

	return 0;
}
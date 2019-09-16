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
#include "sha2.h"
#include "isaac64.h"

void seed() {
	int fd = open("/dev/urandom", O_RDONLY);
	read(fd, randrsl, sizeof(ub8)*RANDSIZ);
	close(fd);
	randinit(TRUE);
}

char letters[] = "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";

void randomize_nonce(char* start, int len) {
	for (int i = 0; i < len; i++) {
		start[i] = letters[rand() % 62];
	}
}

void find_basilisk(int pipe, int n) {

	unsigned char output[32];
	char basilisk[] = "basilisk|0000000000|################################################################ ################################################################";
	basilisk[18] = '0' + n;
	seed();
	randomize_nonce(basilisk+20, 64);

	sha256_ctx ctx_initial;
	sha256_init(&ctx_initial);
	sha256_update(&ctx_initial, basilisk, 64);

	while (1) {

		sha256_ctx ctx;
		memcpy(&ctx, &ctx_initial, sizeof(sha256_ctx));
		randomize_nonce(basilisk+64, 20);
		sha256_update(&ctx, basilisk+64, 20);
		sha256_final(&ctx, (unsigned char*)&output);

		if (output[0] == 0 && output[1] == 0 && output[2] == 0 && output[3] == 0) {
			break;
		}
	}
	char *hexdump = basilisk + 85;
	for (int i = 0; i < 64/2; i++) {
		for (int j = 0; j < 2; j++) {
			int idx = i*2+j;
			int value = (j == 0) ? output[i] / 16 : output[i] % 16;
			hexdump[idx] = letters[value];
		}
	}
	write(pipe, basilisk, 150);
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
	char winning_basilisk[150];
	read(pipes[0], winning_basilisk, 150);
	printf(winning_basilisk);
	printf("\n");

	return 0;
}
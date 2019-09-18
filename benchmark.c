#include <stdio.h>
#include <sys/prctl.h>
#include <sys/wait.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include "basilisk.h"

#define POOL_SIZE 8

void basilisk_stress(int count) {
	basilisk_ctx basilisk;
	basilisk_init(&basilisk, 0);

	for (int i = 0; i < count; i++) {
		basilisk_step(&basilisk);
	}
}

int main(int argc, char** argv) {
	if (argc != 2) {
		return -1;
	}
	int count = atoi(argv[1]);

	for (int i = 0; i < POOL_SIZE; i++) {
		if (fork() == 0) {
			prctl(PR_SET_PDEATHSIG, SIGHUP);
			basilisk_stress(count);
			return 0;
		}
	}

	int status = 0;
	while (wait(&status) > 0);

	return 0;
}
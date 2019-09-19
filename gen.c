#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdint.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include "basilisk.h"

void handle_sighup(int signal) {
	exit(signal);
}

void find_basilisk(int pipe, int n) {
	basilisk_ctx basilisk;
	basilisk_init(&basilisk, n);

	while (1) {
		basilisk_step(&basilisk);

		if (basilisk.ctx_final.s[0] == 0 && basilisk.ctx_final.s[1] < (1 << 24)) {
			break;
		}
	}
	basilisk_finalize(&basilisk);
	if ( write(pipe, basilisk.data, BASILISK_LENGTH) < 0 ) {
		fprintf( stderr, "error writing to pipe\n" );
	}
}

#define POOLSIZE 8

int main(int argc, char** argv) {
	int pipes[2];
	if ( pipe((int*)&pipes) < 0 ) {
		return -1;
	}
	if (argc != 2) {
		return -1;
	}
	int number = atoi(argv[1]);

	for (int i = 0; i < POOLSIZE; i++) {
		if (fork() == 0) {
			struct sigaction sa;
			sa.sa_handler = &handle_sighup;
			sa.sa_flags = SA_RESTART;
			if (sigaction(SIGHUP, &sa, NULL) < 0) {
				fprintf(stderr, "failed to setup SIGHUP handler\n");
				exit(1);
			}
			close(pipes[0]);
			find_basilisk(pipes[1], number);
			return 0;
		}
	}
	char winning_basilisk[BASILISK_LENGTH];
	if ( read(pipes[0], winning_basilisk, BASILISK_LENGTH) < 0 ) {
		fprintf( stderr, "failure reading result\n" );
		return -1;
	}
	printf("%s\n", winning_basilisk);

	return 0;
}

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

		if (basilisk.ctx_final.s[0] == 0) {
			break;
		}
	}
	basilisk_finalize(&basilisk);
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
	read(pipes[0], winning_basilisk, BASILISK_LENGTH);
	printf("%s\n", winning_basilisk);

	return 0;
}

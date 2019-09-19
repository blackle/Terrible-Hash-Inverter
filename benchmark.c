#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <sys/wait.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/time.h>
#include "basilisk.h"

#define POOL_SIZE 8

void handle_sighup(int signal) {
	exit(signal);
}

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

	struct timeval start, end;
	gettimeofday(&start, NULL);

	for (int i = 0; i < POOL_SIZE; i++) {
		if (fork() == 0) {
			struct sigaction sa;
			sa.sa_handler = &handle_sighup;
			sa.sa_flags = SA_RESTART;
			if (sigaction(SIGHUP, &sa, NULL) < 0) {
				fprintf(stderr, "failed to setup SIGHUP handler\n");
				exit(1);
			}
			basilisk_stress(count / POOL_SIZE);
			return 0;
		}
	}

	int status = 0;
	while (wait(&status) > 0);

	gettimeofday(&end, NULL);
	float time = (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec) / 1000000.0;
	int total = count;
	float hps = total / time;

	printf("%d hashes computed, %d threads\n", total, POOL_SIZE);
	printf("%.2f seconds taken\n", time);
	printf("%.2f million hashes per second\n", hps/1000000);

	return 0;
}

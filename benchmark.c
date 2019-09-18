#include <stdio.h>
#include <stdlib.h>
#include "basilisk.h"

int main(int argc, char** argv) {
	if (argc != 2) {
		return -1;
	}
	int count = atoi(argv[1]);

	basilisk_ctx basilisk;
	basilisk_init(&basilisk, 0);

	for (int i = 0; i < count; i++) {
		basilisk_step(&basilisk);
	}
	return 0;
}
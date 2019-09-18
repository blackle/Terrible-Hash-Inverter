#include <string.h>
#include <fcntl.h>
#include <stdint.h>
#include <unistd.h>
#include "basilisk.h"
#include "isaac64.h"

static const char basilisk_template[] = "basilisk|0000000000|################################################################ ################################################################";
static const char letters[] = "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";

void seed() {
	int fd = open("/dev/urandom", O_RDONLY);
	read(fd, randrsl, sizeof(ub8)*RANDSIZ);
	close(fd);
	randinit(TRUE);
}

void randomize_nonce(char* start, int len) {
	for (int i = 0; i < len; i++) {
		start[i] = letters[rand() % 62];
	}
}

void basilisk_init(basilisk_ctx* basilisk, int n) {
	memcpy(basilisk->data, basilisk_template, BASILISK_LENGTH*sizeof(char));
	sha256_init(&basilisk->ctx_initial);

	basilisk->data[18] = '0' + n;
	seed();
	randomize_nonce(basilisk->data+20, 64);

	sha256_update(&basilisk->ctx_initial, basilisk->data, 64);
}

void basilisk_step(basilisk_ctx* basilisk) {
	memcpy(&basilisk->ctx_working, &basilisk->ctx_initial, sizeof(sha256_ctx));

	randomize_nonce(basilisk->data+64, 20);
	sha256_update(&basilisk->ctx_working, basilisk->data+64, 20);
	sha256_final(&basilisk->ctx_working, basilisk->output);
	sha256(basilisk->output, 32, basilisk->output);
}

void basilisk_finalize(basilisk_ctx* basilisk) {

}

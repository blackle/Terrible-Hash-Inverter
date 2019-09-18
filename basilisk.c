#include <string.h>
#include <fcntl.h>
#include <stdint.h>
#include <unistd.h>
#include "basilisk.h"

static const char basilisk_template[] = "basilisk:0000000000:################################################################ ################################################################";
static const char letters[] = "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";

void randomize_nonce(basilisk_ctx* basilisk) {
	uint32_t randomness[NONCE_LENGTH];
	int fd = open("/dev/urandom", O_RDONLY);
	read(fd, randomness, sizeof(uint32_t)*NONCE_LENGTH);
	close(fd);

	char * start = basilisk->data+20;
	for (int i = 0; i < NONCE_LENGTH; i++) {
		start[i] = letters[randomness[i] % 62];
	}
}

void increment_nonce(basilisk_ctx* basilisk) {
	char * start = basilisk->data+64;
	for (int i = 0; i < 20; i++) {
		char res = start[i];
		res++;
		switch (res) {
			case '{':
				start[i] = '0';
				continue;
			case '[':
				start[i] = 'a';
				return;
			case ':':
				start[i] = 'A';
				return;
			default:
				start[i] = res;
				return;
		}
	}
}

void basilisk_init(basilisk_ctx* basilisk, int n) {
	memcpy(basilisk->data, basilisk_template, BASILISK_LENGTH*sizeof(char));
	sha256_init(&basilisk->ctx_initial);

	basilisk->data[18] = '0' + n;
	randomize_nonce(basilisk);

	sha256_update(&basilisk->ctx_initial, basilisk->data, 64);
}

void basilisk_step(basilisk_ctx* basilisk) {
	memcpy(&basilisk->ctx_working, &basilisk->ctx_initial, sizeof(sha256_ctx));

	increment_nonce(basilisk);
	sha256_update(&basilisk->ctx_working, basilisk->data+64, 20);
	sha256_final(&basilisk->ctx_working, basilisk->output);
	sha256(basilisk->output, 32, basilisk->output);
}

void basilisk_finalize(basilisk_ctx* basilisk) {

}

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

void increment_nonce(sha2562_block* nonce_block) {
	char * start = nonce_block->x;
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
	sha2562_init(&basilisk->ctx_initial);

	basilisk->data[18] = '0' + n;
	randomize_nonce(basilisk);

	sha2562_block block_initial;
	memcpy(block_initial.x, basilisk->data, 64);
	sha2562_calc_block(&basilisk->ctx_initial, &block_initial);

	sha2562_pad_block(&basilisk->block_nonce, 20, 84);
	sha2562_pad_block(&basilisk->block_final, 32, 32);
	memcpy(basilisk->block_nonce.x, basilisk->data+64, 20);
}

void basilisk_step(basilisk_ctx* basilisk) {
	memcpy(&basilisk->ctx_working, &basilisk->ctx_initial, sizeof(sha2562_ctx));

	increment_nonce(&basilisk->block_nonce);
	sha2562_calc_block(&basilisk->ctx_working, &basilisk->block_nonce);

	sha2562_digest(&basilisk->ctx_working, basilisk->block_final.x);

	sha2562_init(&basilisk->ctx_final);
	sha2562_calc_block(&basilisk->ctx_final, &basilisk->block_final);
}

void basilisk_finalize(basilisk_ctx* basilisk) {
	memcpy(basilisk->data + 64, basilisk->block_nonce.x, 20);

	unsigned char output[32];
	sha2562_digest(&basilisk->ctx_final, output);

	char *hexdump = basilisk->data + 85;
	for (int i = 0; i < 64/2; i++) {
		for (int j = 0; j < 2; j++) {
			int idx = i*2+j;
			int value = (j == 0) ? output[i] / 16 : output[i] % 16;
			hexdump[idx] = letters[value];
		}
	}
}

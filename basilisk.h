#pragma once

#include "crypto/sha2.h"

#define BASILISK_LENGTH 150
#define NONCE_LENGTH 64

typedef struct {
	char data[BASILISK_LENGTH];
	sha256_ctx ctx_initial;
	sha256_ctx ctx_working;
	sha256_ctx ctx_final;

	sha256_block block_nonce;
	sha256_block block_final;
} basilisk_ctx;

void basilisk_init(basilisk_ctx* basilisk, int n);

void basilisk_step(basilisk_ctx* basilisk);

void basilisk_finalize(basilisk_ctx* basilisk);

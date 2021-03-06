#pragma once

#include <stdint.h>

#define SHA256_DIGEST_SIZE 32
#define SHA256_BLOCK_SIZE  64

typedef struct {
	unsigned char x[SHA256_BLOCK_SIZE];
} sha256_block;

typedef struct {
	uint32_t s[8];
} sha256_ctx;

void sha256_init(sha256_ctx * ctx);
void sha256_pad_block(sha256_block * block, int offset, int length);
void sha256_calc_block(sha256_ctx * ctx, sha256_block * block);
void sha256_digest(sha256_ctx * ctx, unsigned char* digest);
#pragma once

#include <stdint.h>

#define SHA2562_DIGEST_SIZE 32
#define SHA2562_BLOCK_SIZE  64

typedef struct {
	unsigned char x[SHA2562_BLOCK_SIZE];
} sha2562_block;

typedef struct {
	uint32_t s[8];
} sha2562_ctx;

void sha2562_init(sha2562_ctx * ctx);
void sha2562_pad_block(sha2562_block * block, int offset, int length);
void sha2562_calc_block(sha2562_ctx * ctx, sha2562_block * block);
void sha2562_digest(sha2562_ctx * ctx, unsigned char* digest);
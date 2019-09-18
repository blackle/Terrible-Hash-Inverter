#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include <byteswap.h>
#include <assert.h>

#include "sha2.h"
#include "sha256_sse4.h"
#include "sha256_avx1.h"

uint32_t sha256_h0[8] = {0x6a09e667, 0xbb67ae85, 0x3c6ef372, 0xa54ff53a, 0x510e527f, 0x9b05688c, 0x1f83d9ab, 0x5be0cd19};

void sha256_init(sha256_ctx *ctx) {
	memcpy(ctx->s, sha256_h0, sizeof(sha256_h0));
}

void sha256_pad_block(sha256_block * block, int offset, int length) {
	assert((SHA256_BLOCK_SIZE - 9) >= offset);

	int len_b = length << 3;
	int pm_len = 1 << 6;

	memset(block->x + offset, 0, pm_len - offset);
	block->x[offset] = 0x80;
	uint32_t* encoded_len = (uint32_t*)(block->x + pm_len - 4);
	*encoded_len = __bswap_32(len_b);
}

void sha256_calc_block(sha256_ctx * ctx, sha256_block * block) {
	sha256_avx(block->x, ctx->s, 1);
}

void sha256_digest(sha256_ctx * ctx, unsigned char* digest) {
	uint32_t* digest_32 = (uint32_t*)digest;
	for (int i = 0 ; i < 8; i++) {
		digest_32[i] = __bswap_32(ctx->s[i]);
	}
}

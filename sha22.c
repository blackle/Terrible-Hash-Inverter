#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include <byteswap.h>
#include <assert.h>

#include "sha22.h"
#include "sha256_sse4.h"
#include "sha256_avx1.h"

uint32_t sha2562_h0[8] = {0x6a09e667, 0xbb67ae85, 0x3c6ef372, 0xa54ff53a, 0x510e527f, 0x9b05688c, 0x1f83d9ab, 0x5be0cd19};

void sha2562_init(sha2562_ctx *ctx) {
	memcpy(ctx->s, sha2562_h0, sizeof(sha2562_h0));
}

void sha2562_pad_block(sha2562_block * block, int offset, int length) {
	assert((SHA2562_BLOCK_SIZE - 9) >= offset);

	int len_b = length << 3;
	int pm_len = 1 << 6;

	memset(block->x + offset, 0, pm_len - offset);
	block->x[offset] = 0x80;
	uint32_t* encoded_len = (uint32_t*)(block->x + pm_len - 4);
	*encoded_len = __bswap_32(len_b);
}

void sha2562_calc_block(sha2562_ctx * ctx, sha2562_block * block) {
	sha256_avx(block->x, ctx->s, 1);
	return;
}

void sha2562_digest(sha2562_ctx * ctx, unsigned char* digest) {
	uint32_t* digest_32 = (uint32_t*)digest;
	for (int i = 0 ; i < 8; i++) {
		digest_32[i] = __bswap_32(ctx->s[i]);
	}
}

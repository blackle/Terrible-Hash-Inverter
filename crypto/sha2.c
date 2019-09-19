#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include <assert.h>

#if __has_include("byteswap.h")
#include <byteswap.h>
#elif __has_include("machine/bswap.h")
#include <machine/bswap.h>
#define __bswap_32 __byte_swap_u32_variable
#elif __has_include("libkern/OSByteOrder.h")
#include <libkern/OSByteOrder.h>
#define __bswap_32 OSSwapInt32
#else
#pragma message "WARNING: using unaccelerated bswap32"
inline uint32_t bswap32(uint32_t x) {
	return (((x & 0xFF000000) >> 24) | ((x & 0x00FF0000) >> 8) | ((x & 0x0000FF00) << 8) | ((x & 0x000000FF) << 24));
}
#define __bswap_32 bswap32
#endif

#include "sha2.h"
#include "sha256_sse4.h"
#include "sha256_avx1.h"
#include "sha256_avx2_rorx2.h"
#include "sha256_ishaext.h"
#include "sha256_slow.h"

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
#if defined(__SHA__)
#pragma message "Using Intel SHA extensions accelerated SHA256 transformer"
	sha256_ishaext(ctx->s, block->x, 1);
#elif defined(__AVX2__)
#pragma message "Using AVX2 accelerated SHA256 transformer"
	sha256_rorx(block->x, ctx->s, 1);
#elif defined(__AVX__)
#pragma message "Using AVX accelerated SHA256 transformer"
	sha256_avx(block->x, ctx->s, 1);
#elif defined(__SSE4_1__)
#pragma message "Using SSE4 accelerated SHA256 transformer"
	sha256_sse4(block->x, ctx->s, 1);
#else
#pragma message "No accelerated SHA256 transformer available for this platform"
	sha256_slow(block->x, ctx->s);
#endif
}

void sha256_digest(sha256_ctx * ctx, unsigned char* digest) {
	uint32_t* digest_32 = (uint32_t*)digest;
	for (int i = 0 ; i < 8; i++) {
		digest_32[i] = __bswap_32(ctx->s[i]);
	}
}

#pragma once

void sha256_block_data_order(void *ctx, const void *inp, size_t len);
void sha256_block_data_order_neon(void *ctx, const void *inp, size_t len);

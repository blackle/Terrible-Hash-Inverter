#pragma once
#include <stdint.h>

void sha256_rorx(const void *input_data, uint32_t digest[8], uint64_t num_blks);

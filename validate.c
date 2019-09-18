#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sha2.h"
#include "sha22.h"

int main(int argc, char** argv) {
	char test[] = "basilisk|0000000000|################################################################";

	sha256_ctx sha2ctx;
	sha256_init(&sha2ctx);
	sha256_update(&sha2ctx, test, 64);
	sha256_update(&sha2ctx, test+64, 20);
	unsigned char output1[32];
	sha256_final(&sha2ctx, output1);
	sha256(output1, 32, output1);

	sha2562_ctx sha22ctx;
	sha2562_block sha22block1;
	sha2562_block sha22block2;
	sha2562_block sha22block3;
	sha2562_pad_block(&sha22block2, 20, 84);
	sha2562_pad_block(&sha22block3, 32, 32);
	sha2562_init(&sha22ctx);
	memcpy(sha22block1.x, test, 64);
	sha2562_calc_block(&sha22ctx, &sha22block1);
	memcpy(sha22block2.x, test+64, 20);
	sha2562_calc_block(&sha22ctx, &sha22block2);
	unsigned char output2[32];
	sha2562_digest(&sha22ctx, sha22block3.x);
	sha2562_ctx sha22ctx_2;
	sha2562_init(&sha22ctx_2);
	sha2562_calc_block(&sha22ctx_2, &sha22block3);
	sha2562_digest(&sha22ctx_2, output2);

	for (int i = 0; i < 32; i++) {
		printf("%02x", output1[i]);
	}
	printf("\n");
	for (int i = 0; i < 32; i++) {
		printf("%02x", output2[i]);
	}
	printf("\n");

	return 0;
}
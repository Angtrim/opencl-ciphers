#include "des_expansion.h"


/*
 * 32-bit integer manipulation macros (big endian)
 */
#ifndef GET_UINT32_BE
#define GET_UINT32_BE(n, b, i)                                                 \
 {                                                                            \
 	(n) = ((uint32_t)(b)[(i)] << 24) | ((uint32_t)(b)[(i) + 1] << 16) |        \
 	((uint32_t)(b)[(i) + 2] << 8) | ((uint32_t)(b)[(i) + 3]);            \
 }
#endif

#ifndef PUT_UINT32_BE
#define PUT_UINT32_BE(n, b, i)                                                 \
 {                                                                            \
 	(b)[(i)] = (uint8_t)((n) >> 24);                                           \
 	(b)[(i) + 1] = (uint8_t)((n) >> 16);                                       \
 	(b)[(i) + 2] = (uint8_t)((n) >> 8);                                        \
 	(b)[(i) + 3] = (uint8_t)((n));                                             \
 }
#endif


  /*
 * PC1: left and right halves bit-swap
 */
 static const uint32_t LHs[16] = {
 	0x00000000, 0x00000001, 0x00000100, 0x00000101, 0x00010000, 0x00010001,
 	0x00010100, 0x00010101, 0x01000000, 0x01000001, 0x01000100, 0x01000101,
 	0x01010000, 0x01010001, 0x01010100, 0x01010101};

 	static const uint32_t RHs[16] = {
 		0x00000000, 0x01000000, 0x00010000, 0x01010000, 0x00000100, 0x01000100,
 		0x00010100, 0x01010100, 0x00000001, 0x01000001, 0x00010001, 0x01010001,
 		0x00000101, 0x01000101, 0x00010101, 0x01010101,
 	};



 	void des_main_ks(uint32_t SK[32], uint8_t key[8]) {
 		int i;
 		uint32_t X, Y, T;

 		GET_UINT32_BE(X, key, 0);
 		GET_UINT32_BE(Y, key, 4);

  /*
   * Permuted Choice 1
   */
   T = ((Y >> 4) ^ X) & 0x0F0F0F0F;
   X ^= T;
   Y ^= (T << 4);
   T = ((Y) ^ X) & 0x10101010;
   X ^= T;
   Y ^= (T);

   X = (LHs[(X      ) & 0xF] << 3) | (LHs[(X >>  8) & 0xF] << 2) |
   (LHs[(X >> 16) & 0xF] << 1) | (LHs[(X >> 24) & 0xF]) |
   (LHs[(X >>  5) & 0xF] << 7) | (LHs[(X >> 13) & 0xF] << 6) |
   (LHs[(X >> 21) & 0xF] << 5) | (LHs[(X >> 29) & 0xF] << 4);

   Y = (RHs[(Y >>  1) & 0xF] << 3) | (RHs[(Y >>  9) & 0xF] << 2) |
   (RHs[(Y >> 17) & 0xF] << 1) | (RHs[(Y >> 25) & 0xF]) |
   (RHs[(Y >>  4) & 0xF] << 7) | (RHs[(Y >> 12) & 0xF] << 6) |
   (RHs[(Y >> 20) & 0xF] << 5) | (RHs[(Y >> 28) & 0xF] << 4);

   X &= 0x0FFFFFFF;
   Y &= 0x0FFFFFFF;

/*
 * calculate subkeys
 */
  #pragma unroll
 for (i = 0; i < 16; i++) {
 	if (i < 2 || i == 8 || i == 15) {
 		X = ((X << 1) | (X >> 27)) & 0x0FFFFFFF;
 		Y = ((Y << 1) | (Y >> 27)) & 0x0FFFFFFF;
 	} else {
 		X = ((X << 2) | (X >> 26)) & 0x0FFFFFFF;
 		Y = ((Y << 2) | (Y >> 26)) & 0x0FFFFFFF;
 	}

 	*SK++ = ((X <<  4) & 0x24000000) | ((X << 28) & 0x10000000) |
 	((X << 14) & 0x08000000) | ((X << 18) & 0x02080000) |
 	((X <<  6) & 0x01000000) | ((X <<  9) & 0x00200000) |
 	((X >>  1) & 0x00100000) | ((X << 10) & 0x00040000) |
 	((X <<  2) & 0x00020000) | ((X >> 10) & 0x00010000) |
 	((Y >> 13) & 0x00002000) | ((Y >>  4) & 0x00001000) |
 	((Y <<  6) & 0x00000800) | ((Y >>  1) & 0x00000400) |
 	((Y >> 14) & 0x00000200) | ((Y      ) & 0x00000100) |
 	((Y >>  5) & 0x00000020) | ((Y >> 10) & 0x00000010) |
 	((Y >>  3) & 0x00000008) | ((Y >> 18) & 0x00000004) |
 	((Y >> 26) & 0x00000002) | ((Y >> 24) & 0x00000001);

 	*SK++ = ((X << 15) & 0x20000000) | ((X << 17) & 0x10000000) |
 	((X << 10) & 0x08000000) | ((X << 22) & 0x04000000) |
 	((X >>  2) & 0x02000000) | ((X <<  1) & 0x01000000) |
 	((X << 16) & 0x00200000) | ((X << 11) & 0x00100000) |
 	((X <<  3) & 0x00080000) | ((X >>  6) & 0x00040000) |
 	((X << 15) & 0x00020000) | ((X >>  4) & 0x00010000) |
 	((Y >>  2) & 0x00002000) | ((Y <<  8) & 0x00001000) |
 	((Y >> 14) & 0x00000808) | ((Y >>  9) & 0x00000400) |
 	((Y      ) & 0x00000200) | ((Y <<  7) & 0x00000100) |
 	((Y >>  7) & 0x00000020) | ((Y >>  3) & 0x00000011) |
 	((Y <<  2) & 0x00000004) | ((Y >> 21) & 0x00000002);
 }
}

/*
 * DES key schedule (56-bit)
 */
 void des_set_key(des_context *ctx, uint8_t key[8]) {
 	int i;

 	des_main_ks(ctx->esk, key);

  #pragma unroll
 	for (i = 0; i < 32; i += 2) {
 		ctx->dsk[i] = ctx->esk[30 - i];
 		ctx->dsk[i + 1] = ctx->esk[31 - i];
 	}
 }


 void des3_set_2keys(des3_context *ctx, uint8_t key[16]) {
 	int i;

 	des_main_ks(ctx->esk, key);
 	des_main_ks(ctx->dsk + 32, key + 8);

#pragma unroll
 	for (i = 0; i < 32; i += 2) {
 		ctx->dsk[i] = ctx->esk[30 - i];
 		ctx->dsk[i + 1] = ctx->esk[31 - i];

 		ctx->esk[i + 32] = ctx->dsk[62 - i];
 		ctx->esk[i + 33] = ctx->dsk[63 - i];

 		ctx->esk[i + 64] = ctx->esk[i];
 		ctx->esk[i + 65] = ctx->esk[1 + i];

 		ctx->dsk[i + 64] = ctx->dsk[i];
 		ctx->dsk[i + 65] = ctx->dsk[1 + i];
 	}
 }


/*
 * Triple-DES key schedule (168-bit)
 */
 void des3_set_3keys(des3_context *ctx, uint8_t key[24]) {
 	int i;

 	des_main_ks(ctx->esk, key);
 	des_main_ks(ctx->dsk + 32, key + 8);
 	des_main_ks(ctx->esk + 64, key + 16);

  #pragma unroll
 	for (i = 0; i < 32; i += 2) {
 		ctx->dsk[i] = ctx->esk[94 - i];
 		ctx->dsk[i + 1] = ctx->esk[95 - i];

 		ctx->esk[i + 32] = ctx->dsk[62 - i];
 		ctx->esk[i + 33] = ctx->dsk[63 - i];

 		ctx->dsk[i + 64] = ctx->esk[30 - i];
 		ctx->dsk[i + 65] = ctx->esk[31 - i];
 	}
 }

 void des_expandkey(des_context* K, uint8_t *key) {
 	des_set_key(K, key);
 }

 void tdes2_expandkey(des3_context* K, uint8_t *key) {
 	des3_set_2keys(K, key);
 }

 void tdes3_expandkey(des3_context* K, uint8_t *key) {
 	des3_set_3keys(K, key);
 }
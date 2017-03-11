#include <stdint.h>
#include <stdbool.h>
#include "hight_expansion.h"

#define rol8(x, n) (((x) << (n)) | ((x) >> (8 - n)))

#define concat64(a7, a6, a5, a4, a3, a2, a1, a0) \
( ((uint64_t)(a7) << (8*7)) \
| ((uint64_t)(a6) << (8*6)) \
| ((uint64_t)(a5) << (8*5)) \
| ((uint64_t)(a4) << (8*4)) \
| ((uint64_t)(a3) << (8*3)) \
| ((uint64_t)(a2) << (8*2)) \
| ((uint64_t)(a1) << (8*1)) \
| ((uint64_t)(a0) << (8*0)) \
)

#define concat8bit(a7, a6, a5, a4, a3, a2, a1, a0) \
( ((uint8_t)(a7) << 7) \
| ((uint8_t)(a6) << 6) \
| ((uint8_t)(a5) << 5) \
| ((uint8_t)(a4) << 4) \
| ((uint8_t)(a3) << 3) \
| ((uint8_t)(a2) << 2) \
| ((uint8_t)(a1) << 1) \
| ((uint8_t)(a0) << 0) \
)

#define byte(a, n) ((a >> (8*n)) & 0xff)

// ===---- 3.1.  Round functions ----===

inline __attribute__((always_inline))
uint8_t F0(uint8_t x) {
	return rol8(x, 1) ^ rol8(x, 2) ^ rol8(x, 7);
}

inline __attribute__((always_inline))
uint8_t F1(uint8_t x) {
	return rol8(x, 3) ^ rol8(x, 4) ^ rol8(x, 6);
}

// ===---- 3.2.  Key schedule ----===

#ifdef DUMP_ROUNDS
#  include <stdio.h>
#endif

// The key schedule for HIGHT describes the procedure to make whitening
// key bytes WKi and 128 subkey bytes SKi from a 128-bit master key K =
// K15 || K14 || ... || K0, as shown below.
//
inline __attribute__((always_inline))
void hight_expandkey(uint8_t *K, uint8_t *WK, uint8_t *SK) {
  // Whitening keys
	for (int i = 0; i < 4; ++i) {
		WK[i] = K[i + 12];
	}

	for (int i = 4; i < 8; ++i) {
		WK[i] = K[i - 4];
	}

  // Subkeys
  bool s[128 + 6]; // as 1-bit cells
  uint8_t d[128];

  s[0] = 0;
  s[1] = 1;
  s[2] = 0;
  s[3] = 1;
  s[4] = 1;
  s[5] = 0;
  s[6] = 1;

  d[0] = concat8bit(0, s[6], s[5], s[4], s[3], s[2], s[1], s[0]);

  #pragma unroll
  for (int i = 1; i < 128; ++i) {
  	s[i + 6] = s[i + 2] ^ s[i - 1];
  	d[i] = concat8bit(0, s[i + 6], s[i + 5], s[i + 4],
  	s[i + 3], s[i + 2], s[i + 1], s[i]);
  }

  #pragma unroll
  for (int i = 0; i < 8; ++i) {
    #pragma unroll
  	for (int j = 0; j < 8; ++j) {
  		SK[16*i + j] = K[(8 + j - i) % 8] + d[16*i + j];
  	}
    #pragma unroll
  	for (int j = 0; j < 8; ++j) {
  		SK[16*i + j + 8] = K[(8 + j - i)%8 + 8] + d[16*i + j + 8];
  	}
  }

#ifdef DUMP_ROUNDS
  uint32_t *SKw = (uint32_t*)SK;
  printf("Subkey values\n");
  for (int i = 0; i < 32; i += 2) {
  	printf("%08x %08x\n", SKw[i], SKw[i + 1]);
  }
#endif
 }
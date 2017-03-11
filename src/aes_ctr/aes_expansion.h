#ifndef AES_EXPANSION_H
#define AES_EXPANSION_H

#include <stdint.h>

typedef uint32_t word;
typedef uint8_t byte;

void KeyExpansion(word* key, word* w, int Nk, int Nb, int Nr);

#endif

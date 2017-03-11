#ifndef AES_EXPANSION_H
#define AES_EXPANSION_H
#include "../cipher_utils.h"
#include <stdint.h>



void KeyExpansion(word* key, word* w, int Nk, int Nb, int Nr);

#endif

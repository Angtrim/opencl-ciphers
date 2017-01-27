#ifndef AES_CIPHER_H
#define AES_CIPHER_H
#include "aes_expansion.h"

#define NUM_BLOCKS 1000
#define BLOCK_SIZE (128 / 8)

void aesCtrEncrypt(byte inputText[NUM_BLOCKS][BLOCK_SIZE], word key[Nk],  byte output[NUM_BLOCKS][BLOCK_SIZE]);
void aesEncrypt(byte inputText[NUM_BLOCKS][BLOCK_SIZE], word key[Nk],  byte output[NUM_BLOCKS][BLOCK_SIZE]);

#endif

#ifndef AES_CIPHER_H
#define AES_CIPHER_H
#include "aes_expansion.h"
#include <stdio.h>
#define Nk128 4
#define Nb128 4
#define Nr128 10

#define Nk192 6
#define Nb192 4
#define Nr192 12

#define Nk256 8
#define Nb256 4
#define Nr256 14
#define BLOCK_SIZE (128 / 8)

byte* aesCtr128Encrypt(char* fileName, word* key, char* output,size_t local_item_size);
byte* aesCtr192Encrypt(char* fileName, word* key, char* output,size_t local_item_size);
byte* aesCtr256Encrypt(char* fileName, word* key, char* output,size_t local_item_size);
byte* aes128Encrypt(char* fileName, word* key, char* output,size_t local_item_size);
byte* aes192Encrypt(char* fileName, word* key, char* output,size_t local_item_size);
byte* aes256Encrypt(char* fileName, word* key, char* output,size_t local_item_size);

#endif

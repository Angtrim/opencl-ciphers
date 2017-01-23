#ifndef AES_EXPANSION_H
# define AES_EXPANSION_H

#define AES128

#if defined(AES128)
#  define KEYSIZE 128
#  define NAME "AES 128"
#elif defined(AES192)
#  define KEYSIZE 192
#  define NAME "AES 192"
#elif defined(AES256)
#  define KEYSIZE 256
#  define NAME "AES 256"
#else
#  error "Define AES128, AES192 or AES256!"
#endif

#if KEYSIZE == 128
#  define Nk 4
#  define Nb 4
#  define Nr 10
#  define AES_128
#elif KEYSIZE == 192
#  define Nk 6
#  define Nb 4
#  define Nr 12
#  define AES_192
#elif KEYSIZE == 256
#  define Nk 8
#  define Nb 4
#  define Nr 14
#  define AES_256
#else
#  error "Wrong keysize, must be 128, 192 or 256"
#endif

typedef uint32_t word;
typedef uint8_t byte;

void aes_expand(word key[Nk] __attribute__((key)), byte in[4*Nb] __attribute__((plain)),byte out[4*Nb]);

#endif
#include <stdio.h>
#include <inttypes.h>
#include <stdlib.h>

#include "cipher_utils.h"

#define CPU_DEVICE "CPU"
#define GPU_DEVICE "GPU"

#define KEYLEN 128

#define CLEFIA_128

#define DES3


void main1(){
/*
	#if KEYLEN == 128
  	uint64_t Plaintext[2] = {0x0123456789abcdef, 0xfedcba9876543210};
  	uint64_t Ciphertext[16] = {0x6767313854966973, 0x0857065648eabe43, 0x6767313854966973, 0x0857065648eabe43, 0x6767313854966973, 0x0857065648eabe43, 0x6767313854966973, 0x0857065648eabe43, 0x6767313854966973, 0x0857065648eabe43, 0x6767313854966973, 0x0857065648eabe43, 0x6767313854966973, 0x0857065648eabe43, 0x6767313854966973, 0x0857065648eabe43};
  	uint64_t Key[2] = {0x0123456789abcdef, 0xfedcba9876543210};
	#elif KEYLEN == 192
  	uint64_t Plaintext[2] = {0x0123456789abcdef, 0xfedcba9876543210};
  	uint64_t Ciphertext[16] = {0xb4993401b3e996f8, 0x4ee5cee7d79b09b9, 0xb4993401b3e996f8, 0x4ee5cee7d79b09b9, 0xb4993401b3e996f8, 0x4ee5cee7d79b09b9, 0xb4993401b3e996f8, 0x4ee5cee7d79b09b9,
0xb4993401b3e996f8, 0x4ee5cee7d79b09b9, 0xb4993401b3e996f8, 0x4ee5cee7d79b09b9, 0xb4993401b3e996f8, 0x4ee5cee7d79b09b9, 0xb4993401b3e996f8, 0x4ee5cee7d79b09b9};
  	uint64_t Key[3] = {0x0123456789abcdef, 0xfedcba9876543210,
	             0x0011223344556677};
	#elif KEYLEN == 256
  	uint64_t Plaintext[2] = {0x0123456789abcdef, 0xfedcba9876543210};
  	uint64_t Ciphertext[16] = {0x9acc237dff16d76c, 0x20ef7c919e3a7509, 0x9acc237dff16d76c, 0x20ef7c919e3a7509, 0x9acc237dff16d76c, 0x20ef7c919e3a7509, 0x9acc237dff16d76c, 0x20ef7c919e3a7509, 0x9acc237dff16d76c, 0x20ef7c919e3a7509, 0x9acc237dff16d76c, 0x20ef7c919e3a7509, 0x9acc237dff16d76c, 0x20ef7c919e3a7509, 0x9acc237dff16d76c, 0x20ef7c919e3a7509};
  	uint64_t Key[4] = {0x0123456789abcdef, 0xfedcba9876543210,
	             0x0011223344556677, 0x8899aabbccddeeff};

	#endif


	 uint64_t* camelliaPlaintext = (uint64_t*)malloc(16*sizeof(uint64_t));
	 
	 camelliaCtr128Encrypt("camellia_plaintext", Key, camelliaPlaintext, 8, CPU_DEVICE);
	 for(int i=0; i < 16; i++){
	 	printf("%016llx\n", camelliaPlaintext[i]);
	 }

	for(int k = 0; k < 8; k++){
		if (camelliaPlaintext[2*k] != Ciphertext[2*k] || camelliaPlaintext[2*k+1] != Ciphertext[2*k+1]) {
			printf("no\n");
		}
	}
        printf("ok\n");

	free(camelliaPlaintext);*/

/*	uint8_t* clefiaPlaintext = (uint8_t*)malloc(128*sizeof(uint8_t));
	const unsigned char skey[32] = {
	    0xffU,0xeeU,0xddU,0xccU,0xbbU,0xaaU,0x99U,0x88U,
	    0x77U,0x66U,0x55U,0x44U,0x33U,0x22U,0x11U,0x00U,
	    0xf0U,0xe0U,0xd0U,0xc0U,0xb0U,0xa0U,0x90U,0x80U,
	    0x70U,0x60U,0x50U,0x40U,0x30U,0x20U,0x10U,0x00U
  	};
	#if defined (CLEFIA_128)
	  unsigned char ct[128] = {0xde, 0x2b, 0xf2, 0xfd, 0x9b, 0x74, 0xaa, 0xcd, 0xf1, 0x29, 0x85, 0x55, 0x45, 0x94, 0x94, 0xfd, 0xde, 0x2b, 0xf2, 0xfd, 0x9b, 0x74, 0xaa, 0xcd, 0xf1, 0x29, 0x85, 0x55, 0x45, 0x94, 0x94, 0xfd, 0xde, 0x2b, 0xf2, 0xfd, 0x9b, 0x74, 0xaa, 0xcd, 0xf1, 0x29, 0x85, 0x55, 0x45, 0x94, 0x94, 0xfd, 0xde, 0x2b, 0xf2, 0xfd, 0x9b, 0x74, 0xaa, 0xcd, 0xf1, 0x29, 0x85, 0x55, 0x45, 0x94, 0x94, 0xfd, 0xde, 0x2b, 0xf2, 0xfd, 0x9b, 0x74, 0xaa, 0xcd, 0xf1, 0x29, 0x85, 0x55, 0x45, 0x94, 0x94, 0xfd, 0xde, 0x2b, 0xf2, 0xfd, 0x9b, 0x74, 0xaa, 0xcd, 0xf1, 0x29, 0x85, 0x55, 0x45, 0x94, 0x94, 0xfd, 0xde, 0x2b, 0xf2, 0xfd, 0x9b, 0x74, 0xaa, 0xcd, 0xf1, 0x29, 0x85, 0x55, 0x45, 0x94, 0x94, 0xfd, 0xde, 0x2b, 0xf2, 0xfd, 0x9b, 0x74, 0xaa, 0xcd, 0xf1, 0x29, 0x85, 0x55, 0x45, 0x94, 0x94, 0xfd};
	#elif defined (CLEFIA_192)
	  unsigned char ct[128] = {0xe2, 0x48, 0x2f, 0x64, 0x9f, 0x02, 0x8d, 0xc4, 0x80, 0xdd, 0xa1, 0x84, 0xfd, 0xe1, 0x81, 0xad, 0xe2, 0x48, 0x2f, 0x64, 0x9f, 0x02, 0x8d, 0xc4, 0x80, 0xdd, 0xa1, 0x84, 0xfd, 0xe1, 0x81, 0xad, 0xe2, 0x48, 0x2f, 0x64, 0x9f, 0x02, 0x8d, 0xc4, 0x80, 0xdd, 0xa1, 0x84, 0xfd, 0xe1, 0x81, 0xad, 0xe2, 0x48, 0x2f, 0x64, 0x9f, 0x02, 0x8d, 0xc4, 0x80, 0xdd, 0xa1, 0x84, 0xfd, 0xe1, 0x81, 0xad, 0xe2, 0x48, 0x2f, 0x64, 0x9f, 0x02, 0x8d, 0xc4, 0x80, 0xdd, 0xa1, 0x84, 0xfd, 0xe1, 0x81, 0xad, 0xe2, 0x48, 0x2f, 0x64, 0x9f, 0x02, 0x8d, 0xc4, 0x80, 0xdd, 0xa1, 0x84, 0xfd, 0xe1, 0x81, 0xad, 0xe2, 0x48, 0x2f, 0x64, 0x9f, 0x02, 0x8d, 0xc4, 0x80, 0xdd, 0xa1, 0x84, 0xfd, 0xe1, 0x81, 0xad, 0xe2, 0x48, 0x2f, 0x64, 0x9f, 0x02, 0x8d, 0xc4, 0x80, 0xdd, 0xa1, 0x84, 0xfd, 0xe1, 0x81, 0xad};
	#elif defined (CLEFIA_256)
	  unsigned char ct[128] = {0xa1, 0x39, 0x78, 0x14, 0x28, 0x9d, 0xe8, 0x0c, 0x10, 0xda, 0x46, 0xd1, 0xfa, 0x48, 0xb3, 0x8a, 0xa1, 0x39, 0x78, 0x14, 0x28, 0x9d, 0xe8, 0x0c, 0x10, 0xda, 0x46, 0xd1, 0xfa, 0x48, 0xb3, 0x8a, 0xa1, 0x39, 0x78, 0x14, 0x28, 0x9d, 0xe8, 0x0c, 0x10, 0xda, 0x46, 0xd1, 0xfa, 0x48, 0xb3, 0x8a, 0xa1, 0x39, 0x78, 0x14, 0x28, 0x9d, 0xe8, 0x0c, 0x10, 0xda, 0x46, 0xd1, 0xfa, 0x48, 0xb3, 0x8a, 0xa1, 0x39, 0x78, 0x14, 0x28, 0x9d, 0xe8, 0x0c, 0x10, 0xda, 0x46, 0xd1, 0xfa, 0x48, 0xb3, 0x8a, 0xa1, 0x39, 0x78, 0x14, 0x28, 0x9d, 0xe8, 0x0c, 0x10, 0xda, 0x46, 0xd1, 0xfa, 0x48, 0xb3, 0x8a, 0xa1, 0x39, 0x78, 0x14, 0x28, 0x9d, 0xe8, 0x0c, 0x10, 0xda, 0x46, 0xd1, 0xfa, 0x48, 0xb3, 0x8a, 0xa1, 0x39, 0x78, 0x14, 0x28, 0x9d, 0xe8, 0x0c, 0x10, 0xda, 0x46, 0xd1, 0xfa, 0x48, 0xb3, 0x8a};
	#endif
	clefia_128_Encrypt("clefia_plaintext", skey, clefiaPlaintext, 1, CPU_DEVICE);
	for(int i=0; i < 128; i++){
	 	printf("%x", clefiaPlaintext[i]);
	 }
        if (memcmp(ct, clefiaPlaintext, 128) == 0) {
 	printf("\n--->[Test CORRECT]\n");
 	} else {
 	printf("\n--->[Test +WRONG!]\n");
 	}
	free(clefiaPlaintext);
	clefiaPlaintext = NULL;*/

uint8_t* desPlaintext = (uint8_t*)malloc(8*sizeof(uint8_t));

	uint8_t DES3_keys[24] = {0x2B, 0xD6, 0x45, 0x9F, 0x82, 0xC5, 0xB3, 0x00, 0x95, 0x2C, 0x49, 0x10, 0x48, 0x81, 0xFF, 0x48, 0x2B, 0xD6, 0x45, 0x9F, 0x82, 0xC5, 0xB3, 0x00};

uint8_t DES3_init[8] = {0xEA, 0x02, 0x47, 0x14, 0xAD, 0x5C, 0x4D, 0x84};
#if defined (DES1)
uint8_t DES3_enc_test[8] = {0x12, 0x6E, 0xFE, 0x8E, 0xD3, 0x12, 0x19, 0x0A};
#elif defined (DES2)
uint8_t DES3_enc_test[8] = {0xC6, 0x16, 0xAC, 0xE8, 0x43, 0x95, 0x82, 0x47};
#elif defined (DES3)
uint8_t DES3_enc_test[8] = {0xC6, 0x16, 0xAC, 0xE8, 0x43, 0x95, 0x82, 0x47};
#endif
des3CtrEncrypt("des_plaintext", DES3_keys, desPlaintext, 1, CPU_DEVICE);
	for(int i=0; i < 8; i++){
	 	printf("%x", desPlaintext[i]);
	 }
        if (memcmp(DES3_enc_test, desPlaintext, 8) == 0) {
 	printf("\n--->[Test CORRECT]\n");
 	} else {
 	printf("\n--->[Test +WRONG!]\n");
 	}
	free(desPlaintext);
	desPlaintext = NULL;



}

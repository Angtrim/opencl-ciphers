#include <stdio.h>
#include <inttypes.h>
#include <stdlib.h>

#include "cipher_utils.h"

#define CPU_DEVICE "CPU"
#define GPU_DEVICE "GPU"


void main22(){

	 uint64_t Ciphertext2[2] = {0x6767313854966973, 0x0857065648eabe43};
	 uint64_t Key2[2] = {0x0123456789abcdef, 0xfedcba9876543210};

	 uint64_t* camelliaPlaintext = (uint64_t*)malloc(2*sizeof(uint64_t));
	 
	 camellia128Encrypt("camellia_plaintext", Key2, camelliaPlaintext, 2, GPU_DEVICE);
	 for(int i=0; i < 2; i++){
	 	printf("%016llx\n", camelliaPlaintext[i]);
	 }

	 if (camelliaPlaintext[0] == Ciphertext2[0] && camelliaPlaintext[1] == Ciphertext2[1]) {
	   printf("ok\n");
	 } else {
	   printf("no\n");
	 }

	 free(camelliaPlaintext);
}

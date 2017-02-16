#define DEBUG_LOG 1
#include <stdio.h>
#include <inttypes.h>
#include <stdlib.h>

#include "test_utils.h"

#include "test_misty1.h"

#include "../cipher_utils.h"

//includes for testing normal encryption
#include "../aes_test_vect.h"

#define MAX_LOCAL_SIZE 64

#define CPU_DEVICE "CPU"
#define GPU_DEVICE "GPU"

uint8_t Misty1Key[16] = {
	0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77,
	0x88, 0x99, 0xaa, 0xbb, 0xcc, 0xdd, 0xee, 0xff
};

uint8_t Misty1P[16] = {
	0xef, 0xcd, 0xab, 0x89, 0x67, 0x45, 0x23, 0x01,
	0x10, 0x32, 0x54, 0x76, 0x98, 0xba, 0xdc, 0xfe
};

uint64_t Misty1C[2] = {
	0x8b1da5f56ab3d07cUL,
	0x04b68240b13be95dUL
};


// to save ciphertext to file (uint8_t)
static void writeOutputToFileUint64(char* outFileName, uint8_t* output, long lenght){
	FILE* fp = fopen(outFileName, "wb");
	if (!fp) {
		fprintf(stderr, "Failed to load file.\n");
		exit(1);
	}
	printf("\nwriting %d uint64_t..\n", lenght);
	for(int i = 0; i <lenght; i++){
		printf("%x", output[i]);	
	}
	fwrite(output, sizeof(uint8_t), lenght, fp);
	fclose(fp);
}

// TEST FOR 128

int testMisty1CPU(){


	int success = 1;
	uint64_t* misty1Ciphertext = (uint8_t*)malloc((2)*sizeof(uint64_t));
	writeOutputToFileUint64("misty1_plaintext", Misty1P, 16);
	misty1Encrypt("misty1_plaintext", Misty1Key, misty1Ciphertext, 2, CPU_DEVICE);
	if (misty1Ciphertext[0] != Misty1C[0] || misty1Ciphertext[1] != Misty1C[1]) {
		success = 0;
	}
	free(misty1Ciphertext);
	return success;
}

int testMisty1All(){

	int tMisty1 = testMisty1CPU();
	int result = tMisty1;
	if(result){
		log("--- --- --- ALL MISTY1 TEST PASSED");
	}else{
		log("--- --- --- TEST MISTY1 FAILED");
	}
	return result;
}
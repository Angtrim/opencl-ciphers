#define DEBUG_LOG 1
#include <stdio.h>
#include <inttypes.h>
#include <stdlib.h>

#include "test_utils.h"

#include "test_misty1.h"

#include "../cipher_utils.h"

#define MAX_LOCAL_SIZE 64

#define CPU_DEVICE "CPU"
#define GPU_DEVICE "GPU"

static uint8_t Misty1Key[16] = {
	0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77,
	0x88, 0x99, 0xaa, 0xbb, 0xcc, 0xdd, 0xee, 0xff
};

uint64_t Misty1P[2] = {
	0x0123456789abcdefUL,
	0xfedcba9876543210UL
};

uint64_t Misty1C[2] = {
	0x8b1da5f56ab3d07cUL,
	0x04b68240b13be95dUL
};


// to save ciphertext to file (uint8_t)
static void writeOutputToFileUint64(char* outFileName, uint64_t* output, long lenght){
	FILE* fp = fopen(outFileName, "wb");
	if (!fp) {
		fprintf(stderr, "Failed to load file.\n");
		exit(1);
	}
	fwrite(output, sizeof(uint64_t), lenght, fp);
	fclose(fp);
}

// TEST FOR MISTY1

int testMisty1(cl_device_id* device_id){
	int success = 1;
	uint64_t* misty1Ciphertext = (uint64_t*)malloc((2)*sizeof(uint64_t));
	writeOutputToFileUint64("misty1_plaintext", Misty1P, 2);
	misty1Encrypt("misty1_plaintext", Misty1Key, misty1Ciphertext, 2,  device_id);
	if (misty1Ciphertext[0] != Misty1C[0] || misty1Ciphertext[1] != Misty1C[1]) {
		success = 0;
	}
	free(misty1Ciphertext);
	remove("misty1_plaintext");
	return success;
}



int testMisty1Ctr(cl_device_id* device_id){
	int success = 1;
	struct FileInfo64 fileInfo = getFileUint64("ctr_test");
	long dim = fileInfo.lenght;
	uint64_t* misty1Ciphertext = (uint64_t*)malloc(dim*sizeof(uint64_t));
	uint64_t* misty1Plaintext = (uint64_t*)malloc(dim*sizeof(uint64_t));
	misty1CtrEncrypt("ctr_test", Misty1Key, misty1Ciphertext, 2,  device_id);
	writeOutputToFileUint64("misty1_ciphertext", misty1Ciphertext, dim);
	misty1CtrDecrypt("misty1_ciphertext", Misty1Key, misty1Plaintext, 2,  device_id);

	if(memcmp(misty1Plaintext, fileInfo.filePointer, fileInfo.lenght) != 0){
		success = 0;
	}

	free(misty1Ciphertext);
	free(misty1Plaintext);
	free(fileInfo.filePointer);
	remove("misty1_ciphertext");
	return success;
}



int testMisty1ALL(cl_device_id* device_id){
	int result = 1;
	log("--- --- Starting MISTY1 tests");
	
	log("--- Test MISTY1  starting");
	if(testMisty1( device_id) == 1){
		log("--- Test MISTY1  passed");
	}else{
		log("--- Test MISTY1  FAILED!");
		result = 0;
	}


	log("--- Test MISTY1 CTR  starting");
	if(testMisty1Ctr(device_id) == 1){
		log("--- Test MISTY1 CTR  passed");
	}else{
		log("--- Test MISTY1 CTR  FAILED!");
		result = 0;
	}



	if(result != 0){
		log("--- --- All MISTY1 test passed");
	}else{
		log("--- --- Some MISTY1 Test FAILED");
	}
	return result;

}

int testMisty1All(cl_device_id* device_id){

	int tMisty1 = testMisty1ALL(device_id);
	int result = tMisty1;
	if(result){
		log("--- --- --- ALL MISTY1 TEST PASSED");
	}else{
		log("--- --- --- TEST MISTY1 FAILED");
	}
	return result;
}
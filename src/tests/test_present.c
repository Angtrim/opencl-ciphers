#define DEBUG_LOG 1
#include <stdio.h>
#include <inttypes.h>
#include <stdlib.h>

#include "test_utils.h"

#include "test_present.h"
#include "../present_ctr/present_cipher.h"

#include "../cipher_utils.h"

#define MAX_LOCAL_SIZE 64


static uint64_t PresentKey[2] = {0, 0};
uint64_t PresentPlaintext[1] = {0x0000000000000000ul};
uint64_t PresentC = 0x5579c1387b228445ul;

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

// TEST FOR PRESENT MEMORY

int testPresentMemory(cl_device_id* device_id){
	int success = 1;
	uint64_t* presentCiphertext = (uint64_t*)malloc(sizeof(uint64_t));
	writeOutputToFileUint64("present_plaintext", PresentPlaintext, 1);
	present_memory_Encrypt("present_plaintext", PresentKey, presentCiphertext, 1,device_id);
	
	if (presentCiphertext[0] != PresentC) {
		success = 0;
	}
	free(presentCiphertext);
	remove("present_plaintext");
	return success;
}


// TEST FOR PRESENT SPEED

int testPresentSpeed(cl_device_id* device_id){
	int success = 1;
	uint64_t* presentCiphertext = (uint64_t*)malloc(sizeof(uint64_t));
	writeOutputToFileUint64("present_plaintext", PresentPlaintext, 1);
	present_speed_Encrypt("present_plaintext", PresentKey, presentCiphertext, 1, device_id);

	if (presentCiphertext[0] != PresentC) {
		success = 0;
	}
	free(presentCiphertext);
	remove("present_plaintext");
	return success;
}



// TEST FOR PRESENT MEMORY CTR

int testPresentMemoryCtr(cl_device_id* device_id){
	int success = 1;
	struct FileInfo64 fileInfo = getFileUint64("ctr_test");
	long dim = fileInfo.lenght;
	uint64_t* presentCiphertext = (uint64_t*)malloc(dim*sizeof(uint64_t));
	uint64_t* presentPlaintext = (uint64_t*)malloc(dim*sizeof(uint64_t));

	present_memory_CtrEncrypt("ctr_test", PresentKey, presentCiphertext, 2, device_id);
	writeOutputToFileUint64("present_ciphertext", presentCiphertext, dim);
	present_memory_CtrDecrypt("present_ciphertext", PresentKey, presentPlaintext, 2, device_id);

	if(memcmp(presentPlaintext, fileInfo.filePointer, fileInfo.lenght) != 0){
		success = 0;
	}

	free(presentCiphertext);
	free(presentPlaintext);
	free(fileInfo.filePointer);
	remove("present_ciphertext");
	return success;
}



// TEST FOR PRESENT SPEED CTR

int testPresentSpeedCtr(cl_device_id* device_id){
	int success = 1;
	struct FileInfo64 fileInfo = getFileUint64("ctr_test");
	long dim = fileInfo.lenght;
	uint64_t* presentCiphertext = (uint64_t*)malloc(dim*sizeof(uint64_t));
	uint64_t* presentPlaintext = (uint64_t*)malloc(dim*sizeof(uint64_t));

	present_speed_CtrEncrypt("ctr_test", PresentKey, presentCiphertext, 2,  device_id);
	writeOutputToFileUint64("present_ciphertext", presentCiphertext, dim);
	present_speed_CtrDecrypt("present_ciphertext", PresentKey, presentPlaintext, 2,  device_id);

	if(memcmp(presentPlaintext, fileInfo.filePointer, fileInfo.lenght) != 0){
		success = 0;
	}

	free(presentCiphertext);
	free(presentPlaintext);
	free(fileInfo.filePointer);
	remove("present_ciphertext");
	return success;
}


int testPresentMemoryALL(cl_device_id* device_id){
	int result = 1;
	logIt("--- --- Starting PRESENT MEMORY tests");
	
	logIt("--- Test PRESENT MEMORY  starting");
	if(testPresentMemory( device_id) == 1){
		logIt("--- Test PRESENT MEMORY  passed");
	}else{
		logIt("--- Test PRESENT MEMORY  FAILED!");
		result = 0;
	}



	logIt("--- Test PRESENT MEMORY CTR  starting");
	if(testPresentMemoryCtr(device_id) == 1){
		logIt("--- Test PRESENT MEMORY CTR  passed");
	}else{
		logIt("--- Test PRESENT MEMORY CTR  FAILED!");
		result = 0;
	}


	if(result != 0){
		logIt("--- --- All PRESENT MEMORY test passed");
	}else{
		logIt("--- --- Some PRESENT MEMORY Test FAILED");
	}
	return result;
}

int testPresentSpeedALL(cl_device_id* device_id){
	int result = 1;
	logIt("--- --- Starting PRESENT SPEED tests");
	
	logIt("--- Test PRESENT SPEED  starting");
	if(testPresentSpeed(device_id) == 1){
		logIt("--- Test PRESENT SPEED  passed");
	}else{
		logIt("--- Test PRESENT SPEED  FAILED!");
		result = 0;
	}


	logIt("--- Test PRESENT SPEED CTR  starting");
	if(testPresentSpeedCtr(device_id) == 1){
		logIt("--- Test PRESENT SPEED CTR  passed");
	}else{
		logIt("--- Test PRESENT SPEED CTR  FAILED!");
		result = 0;
	}



	if(result != 0){
		logIt("--- --- All PRESENT SPEED test passed");
	}else{
		logIt("--- --- Some PRESENT SPEED Test FAILED");
	}
	return result;
}

int testPresentAll(cl_device_id* device_id){

	int tPresentMemory = testPresentMemoryALL(device_id);
	int tPresentSpeed = testPresentSpeedALL(device_id);
	int result = tPresentMemory&&tPresentSpeed;
	if(result){
		logIt("--- --- --- ALL PRESENT TEST PASSED");
	}else{
		logIt("--- --- --- TEST PRESENT FAILED");
	}
	return result;
}
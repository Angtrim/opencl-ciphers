#include <stdio.h>
#include <inttypes.h>
#include <stdlib.h>

#include "test_utils.h"

#include "test_hight.h"

#include "../cipher_utils.h"

#define MAX_LOCAL_SIZE 64

#define CPU_DEVICE "CPU"
#define GPU_DEVICE "GPU"

static uint8_t HightKeys[2][16] = { 
    {0xff, 0xee, 0xdd, 0xcc, 0xbb, 0xaa, 0x99, 0x88,
     0x77, 0x66, 0x55, 0x44, 0x33, 0x22, 0x11, 0x00},
    {0xff, 0xee, 0xdd, 0xcc, 0xbb, 0xaa, 0x99, 0x88,
     0x77, 0x66, 0x55, 0x44, 0x33, 0x22, 0x11, 0x00}
	};

  	uint64_t HightPlaintexts[1] = { 0x0000000000000000ul };

  	uint64_t HightCiphertexts[1] = { 0x00f418aed94f03f2ul };

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

// TEST FOR HIGHT 

int testHight(cl_device_id* device_id){
	int success = 1;
	uint64_t* hightCiphertext = (uint64_t*)malloc(sizeof(uint64_t));
	writeOutputToFileUint64("hight_plaintext", HightPlaintexts, 1);
	hightEncrypt("hight_plaintext", HightKeys[0], hightCiphertext, 1, device_id);
	
	if (hightCiphertext[0] != HightCiphertexts[0]) {
		success = 0;
	}
	free(hightCiphertext);
	remove("hight_plaintext");
	return success;
}


// TEST FOR HIGHT CTR

int testHightCtr(cl_device_id* device_id){
	int success = 1;
	struct FileInfo64 fileInfo = getFileUint64("ctr_test");
	long dim = fileInfo.lenght;
	uint64_t* hightCiphertext = (uint64_t*)malloc(dim*sizeof(uint64_t));
	uint64_t* hightPlaintext = (uint64_t*)malloc(dim*sizeof(uint64_t));

	hightCtrEncrypt("ctr_test", HightKeys[0], hightCiphertext, 2,  device_id);
	writeOutputToFileUint64("hight_ciphertext", hightCiphertext, dim);
	hightCtrDecrypt("hight_ciphertext", HightKeys[0], hightPlaintext, 2,  device_id);

	if(memcmp(hightPlaintext, fileInfo.filePointer, fileInfo.lenght) != 0){
		success = 0;
	}

	free(hightCiphertext);
	free(hightPlaintext);
	free(fileInfo.filePointer);
	remove("hight_ciphertext");
	return success;
}


int testHightALL(cl_device_id* device_id){
	int result = 1;
	log("--- --- Starting HIGHT tests");
	
	log("--- Test HIGHT  starting");
	if(testHight( device_id) == 1){
		log("--- Test HIGHT  passed");
	}else{
		log("--- Test HIGHT  FAILED!");
		result = 0;
	}


	log("--- Test HIGHT CTR  starting");
	if(testHightCtr(device_id) == 1){
		log("--- Test HIGHT CTR  passed");
	}else{
		log("--- Test HIGHT CTR  FAILED!");
		result = 0;
	}

	return result;
}

int testHightAll(cl_device_id* device_id){

	int tHight = testHightALL(device_id);
	int result = tHight;
	if(result){
		log("--- --- --- ALL HIGHT TEST PASSED");
	}else{
		log("--- --- --- TEST HIGHT FAILED");
	}
	return result;
}
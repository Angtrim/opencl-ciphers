#include <stdio.h>
#include <inttypes.h>
#include <stdlib.h>
#include "test_utils.h"
#include "test_camellia.h"
#include "../camellia_ctr/camellia_cipher.h"

uint64_t Plaintext128[2] = {0x0123456789abcdef, 0xfedcba9876543210};
uint64_t Ciphertext128[2] = {0x6767313854966973, 0x0857065648eabe43};
static uint64_t Key128[2] = {0x0123456789abcdef, 0xfedcba9876543210};

uint64_t Plaintext192[2] = {0x0123456789abcdef, 0xfedcba9876543210};
uint64_t Ciphertext192[2] = {0xb4993401b3e996f8, 0x4ee5cee7d79b09b9};
static uint64_t Key192[3] = {0x0123456789abcdef, 0xfedcba9876543210, 0x0011223344556677};
uint64_t Plaintext256[2] = {0x0123456789abcdef, 0xfedcba9876543210};
uint64_t Ciphertext256[2] = {0x9acc237dff16d76c, 0x20ef7c919e3a7509};
static uint64_t Key256[4] = {0x0123456789abcdef, 0xfedcba9876543210,0x0011223344556677, 0x8899aabbccddeeff};



static void writeOutputToFileUint64(char* outFileName, uint64_t* output, long lenght){
	FILE* fp = fopen(outFileName, "wb");
	if (!fp) {
		fprintf(stderr, "Failed to load file.\n");
		exit(1);
	}
	fwrite(output, sizeof(uint64_t), lenght, fp);
	fclose(fp);
}

int testCamellia128(cl_device_id* device_id){
	int success = 1;
	char* fileName = "camTest";
	uint64_t out[2];
	writeOutputToFileUint64(fileName,Plaintext128,2);
	camellia128Encrypt(fileName, Key128, out, 1, device_id);
	if (out[0] != Ciphertext128[0] || out[1] != Ciphertext128[1]) {
		success = 0;
	}
	remove(fileName);
	return success;
}


int testCamellia192(cl_device_id* device_id){
	int success = 1;
	char* fileName = "camTest";
	uint64_t out[2];
	writeOutputToFileUint64(fileName,Plaintext192,2);
	camellia192Encrypt(fileName, Key192, out, 1,device_id);
	if (out[0] != Ciphertext192[0] || out[1] != Ciphertext192[1]) {
		success = 0;
	}
	remove(fileName);
	return success;
}



int testCamellia256(cl_device_id* device_id){
	int success = 1;
	char* fileName = "camTest";
	uint64_t out[2];
	writeOutputToFileUint64(fileName,Plaintext256,2);
	camellia256Encrypt(fileName, Key256, out, 1,device_id);
	if (out[0] != Ciphertext256[0] || out[1] != Ciphertext256[1]) {
		success = 0;
	}
	remove(fileName);
	return success;
}



int testCamelliaCTR128(cl_device_id* device_id){
	int success = 1;
	char* fileName = "camTest";
	char* fileNameOut = "camTestOut";
	uint64_t out[2];
	uint64_t outC[2];
	writeOutputToFileUint64(fileName,Plaintext128,2);
	camelliaCtr128Encrypt(fileName, Key128, out, 1, device_id);
	writeOutputToFileUint64(fileNameOut,out,2);
	camelliaCtr128Decrypt(fileNameOut, Key128, outC, 1, device_id);
	if (Plaintext128[0] != outC[0] || Plaintext128[1] != outC[1]) {
		success = 0;
	}
	remove(fileName);
	remove(fileNameOut);
	return success;
}





int testCamelliaCTR192(cl_device_id* device_id){
	int success = 1;
	char* fileName = "camTest";
	char* fileNameOut = "camTestOut";
	uint64_t out[2];
	uint64_t outC[2];
	writeOutputToFileUint64(fileName,Plaintext192,2);
	camelliaCtr192Encrypt(fileName, Key192, out, 1, device_id);
	writeOutputToFileUint64(fileNameOut,out,2);
	camelliaCtr192Decrypt(fileNameOut, Key192, outC, 1, device_id);
	if (Plaintext192[0] != outC[0] || Plaintext192[1] != outC[1]) {
		success = 0;
	}
	remove(fileName);
	remove(fileNameOut);
	return success;
}




int testCamelliaCTR256(cl_device_id* device_id){
	int success = 1;
	char* fileName = "camTest";
	char* fileNameOut = "camTestOut";
	uint64_t out[2];
	uint64_t outC[2];
	writeOutputToFileUint64(fileName,Plaintext256,2);
	camelliaCtr256Encrypt(fileName, Key256, out, 1,device_id);
	writeOutputToFileUint64(fileNameOut,out,2);
	camelliaCtr256Decrypt(fileNameOut, Key256, outC, 1, device_id);
	if (Plaintext256[0] != outC[0] || Plaintext256[1] != outC[1]) {
		success = 0;
	}
	remove(fileName);
	remove(fileNameOut);
	return success;
}




int testCamellia128All(cl_device_id* device_id){
	int result = 1;
	logIt("--- --- Starting CAMELLIA 128 tests");
	
	logIt("--- Test CAMELLIA 128  starting");
	if(testCamellia128( device_id) == 1){
		logIt("--- Test CAMELLIA 128  passed");
	}else{
		logIt("--- Test CAMELLIA 128  FAILED!");
		result = 0;
	}



	logIt("--- Test CAMELLIA CTR 128  starting");
	if(testCamelliaCTR128( device_id) == 1){
		logIt("--- Test CAMELLIA CTR 128  passed");
	}else{
		logIt("--- Test CAMELLIA CTR 128  FAILED!");
		result = 0;
	}


	if(result != 0){
        logIt("--- --- All CAMELLIA 128 test passed");
    }else{
        logIt("--- --- Some CAMELLIA 128 Test FAILED");
    }

	return result;
}

int testCamellia192All(cl_device_id* device_id){
	int result = 1;

	logIt("--- --- Starting CAMELLIA 192 tests");

	logIt("--- Test CAMELLIA 192  starting");
	if(testCamellia192( device_id) == 1){
		logIt("--- Test CAMELLIA 192  passed");
	}else{
		logIt("--- Test CAMELLIA 192  FAILED!");
		result = 0;
	}


	logIt("--- Test CAMELLIA CTR 192  starting");
	if(testCamelliaCTR192( device_id) == 1){
		logIt("--- Test CAMELLIA CTR 192  passed");
	}else{
		logIt("--- Test CAMELLIA CTR 192  FAILED!");
		result = 0;
	}


	if(result != 0){
        logIt("--- --- All CAMELLIA 192 test passed");
    }else{
        logIt("--- --- Some CAMELLIA 192 Test FAILED");
    }

    return result;
}

int testCamellia256All(cl_device_id* device_id){
	int result = 1;

	logIt("--- --- Starting CAMELLIA 256 tests");

	logIt("--- Test CAMELLIA 256  starting");
	if(testCamellia256(device_id) == 1){
		logIt("--- Test CAMELLIA 256  passed");
	}else{
		logIt("--- Test CAMELLIA 256  FAILED!");
		result = 0;
	}



	logIt("--- Test CAMELLIA CTR 256  starting");
	if(testCamelliaCTR256( device_id) == 1){
		logIt("--- Test CAMELLIA CTR 256  passed");
	}else{
		logIt("--- Test CAMELLIA CTR 256  FAILED!");
		result = 0;
	}


	if(result != 0){
        logIt("--- --- All CAMELLIA 256 test passed");
    }else{
        logIt("--- --- Some CAMELLIA 256 Test FAILED");
    }

	return result;
}


int testCamelliaAll(cl_device_id* device_id){
	
	int tCamellia128 = testCamellia128All( device_id);
	int tCamellia192 = testCamellia192All( device_id);
	int tCamellia256 = testCamellia192All( device_id);
	int result = tCamellia128&&tCamellia192&&tCamellia256;
	if(result){
		logIt("--- --- --- ALL CAMELLIA TEST PASSED");
	}else{
		logIt("--- --- --- TEST CAMELLIA FAILED");
	}
	return result;	
}



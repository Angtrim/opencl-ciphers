#include <stdio.h>
#include <inttypes.h>
#include <stdlib.h>
#include "test_utils.h"
#include "test_camellia.h"
#define CPU_DEVICE "CPU"
#define GPU_DEVICE "GPU"

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

int testCamellia128CPU(){
	int success = 1;
	char* fileName = "camTest";
	uint64_t out[2];
	writeOutputToFileUint64(fileName,Plaintext128,2);
	camellia128Encrypt(fileName, Key128, out, 1,CPU_DEVICE);
	if (out[0] != Ciphertext128[0] || out[1] != Ciphertext128[1]) {
		success = 0;
	}
	remove(fileName);
	return success;
}

int testCamellia128GPU(){
	int success = 1;
	char* fileName = "camTest";
	uint64_t out[2];
	writeOutputToFileUint64(fileName,Plaintext128,2);
	camellia128Encrypt(fileName, Key128, out, 1,GPU_DEVICE);
	if (out[0] != Ciphertext128[0] || out[1] != Ciphertext128[1]) {
		success = 0;
	}
	remove(fileName);
	return success;
}

int testCamellia192CPU(){
	int success = 1;
	char* fileName = "camTest";
	uint64_t out[2];
	writeOutputToFileUint64(fileName,Plaintext192,2);
	camellia192Encrypt(fileName, Key192, out, 1,CPU_DEVICE);
	if (out[0] != Ciphertext192[0] || out[1] != Ciphertext192[1]) {
		success = 0;
	}
	remove(fileName);
	return success;
}

int testCamellia192GPU(){
	int success = 1;
	char* fileName = "camTest";
	uint64_t out[2];
	writeOutputToFileUint64(fileName,Plaintext192,2);
	camellia192Encrypt(fileName, Key192, out, 1,GPU_DEVICE);
	if (out[0] != Ciphertext192[0] || out[1] != Ciphertext192[1]) {
		success = 0;
	}
	remove(fileName);
	return success;
}


int testCamellia256CPU(){
	int success = 1;
	char* fileName = "camTest";
	uint64_t out[2];
	writeOutputToFileUint64(fileName,Plaintext256,2);
	camellia256Encrypt(fileName, Key256, out, 1,CPU_DEVICE);
	if (out[0] != Ciphertext256[0] || out[1] != Ciphertext256[1]) {
		success = 0;
	}
	remove(fileName);
	return success;
}

int testCamellia256GPU(){
	int success = 1;
	char* fileName = "camTest";
	uint64_t out[2];
	writeOutputToFileUint64(fileName,Plaintext256,2);
	camellia256Encrypt(fileName, Key256, out, 1,GPU_DEVICE);
	if (out[0] != Ciphertext256[0] || out[1] != Ciphertext256[1]) {
		success = 0;
	}
	remove(fileName);
	return success;
}

int testCamelliaCTR128CPU(){
	int success = 1;
	char* fileName = "camTest";
	char* fileNameOut = "camTestOut";
	uint64_t out[2];
	uint64_t outC[2];
	writeOutputToFileUint64(fileName,Plaintext128,2);
	camelliaCtr128Encrypt(fileName, Key128, out, 1,CPU_DEVICE);
	writeOutputToFileUint64(fileNameOut,out,2);
	camelliaCtr128Decrypt(fileNameOut, Key128, outC, 1,CPU_DEVICE);
	if (Plaintext128[0] != outC[0] || Plaintext128[1] != outC[1]) {
		success = 0;
	}
	remove(fileName);
	remove(fileNameOut);
	return success;
}


int testCamelliaCTR128GPU(){
	int success = 1;
	char* fileName = "camTest";
	char* fileNameOut = "camTestOut";
	uint64_t out[2];
	uint64_t outC[2];
	writeOutputToFileUint64(fileName,Plaintext128,2);
	camelliaCtr128Encrypt(fileName, Key128, out, 1,GPU_DEVICE);
	writeOutputToFileUint64(fileNameOut,out,2);
	camelliaCtr128Decrypt(fileNameOut, Key128, outC, 1,GPU_DEVICE);
	if (Plaintext128[0] != outC[0] || Plaintext128[1] != outC[1]) {
		success = 0;
	}
	remove(fileName);
	remove(fileNameOut);
	return success;
}


int testCamelliaCTR192CPU(){
	int success = 1;
	char* fileName = "camTest";
	char* fileNameOut = "camTestOut";
	uint64_t out[2];
	uint64_t outC[2];
	writeOutputToFileUint64(fileName,Plaintext192,2);
	camelliaCtr192Encrypt(fileName, Key192, out, 1,CPU_DEVICE);
	writeOutputToFileUint64(fileNameOut,out,2);
	camelliaCtr192Decrypt(fileNameOut, Key192, outC, 1,CPU_DEVICE);
	if (Plaintext192[0] != outC[0] || Plaintext192[1] != outC[1]) {
		success = 0;
	}
	remove(fileName);
	remove(fileNameOut);
	return success;
}


int testCamelliaCTR192GPU(){
	int success = 1;
	char* fileName = "camTest";
	char* fileNameOut = "camTestOut";
	uint64_t out[2];
	uint64_t outC[2];
	writeOutputToFileUint64(fileName,Plaintext192,2);
	camelliaCtr192Encrypt(fileName, Key192, out, 1,GPU_DEVICE);
	writeOutputToFileUint64(fileNameOut,out,2);
	camelliaCtr192Decrypt(fileNameOut, Key192, outC, 1,GPU_DEVICE);
	if (Plaintext192[0] != outC[0] || Plaintext192[1] != outC[1]) {
		success = 0;
	}
	remove(fileName);
	remove(fileNameOut);
	return success;
}


int testCamelliaCTR256CPU(){
	int success = 1;
	char* fileName = "camTest";
	char* fileNameOut = "camTestOut";
	uint64_t out[2];
	uint64_t outC[2];
	writeOutputToFileUint64(fileName,Plaintext256,2);
	camelliaCtr256Encrypt(fileName, Key256, out, 1,CPU_DEVICE);
	writeOutputToFileUint64(fileNameOut,out,2);
	camelliaCtr256Decrypt(fileNameOut, Key256, outC, 1,CPU_DEVICE);
	if (Plaintext256[0] != outC[0] || Plaintext256[1] != outC[1]) {
		success = 0;
	}
	remove(fileName);
	remove(fileNameOut);
	return success;
}


int testCamelliaCTR256GPU(){
	int success = 1;
	char* fileName = "camTest";
	char* fileNameOut = "camTestOut";
	uint64_t out[2];
	uint64_t outC[2];
	writeOutputToFileUint64(fileName,Plaintext256,2);
	camelliaCtr256Encrypt(fileName, Key256, out, 1,GPU_DEVICE);
	writeOutputToFileUint64(fileNameOut,out,2);
	camelliaCtr256Decrypt(fileNameOut, Key256, outC, 1,GPU_DEVICE);
	if (Plaintext256[0] != outC[0] || Plaintext256[1] != outC[1]) {
		success = 0;
	}
	remove(fileName);
	remove(fileNameOut);
	return success;
}

int testCamellia128(){
	int result = 1;
	log("--- --- Starting CAMELLIA 128 tests");
	
	log("--- Test CAMELLIA 128 CPU starting");
	if(testCamellia128CPU() == 1){
		log("--- Test CAMELLIA 128 CPU passed");
	}else{
		log("--- Test CAMELLIA 128 CPU FAILED!");
		result = 0;
	}

	log("--- Test CAMELLIA 128 GPU starting");
	if(testCamellia128GPU() == 1){
		log("--- Test CAMELLIA 128 GPU passed");
	}else{
		log("--- Test CAMELLIA 128 GPU FAILED!");
		result = 0;
	}

	log("--- Test CAMELLIA CTR 128 CPU starting");
	if(testCamelliaCTR128CPU() == 1){
		log("--- Test CAMELLIA CTR 128 CPU passed");
	}else{
		log("--- Test CAMELLIA CTR 128 CPU FAILED!");
		result = 0;
	}

	log("--- Test CAMELLIA CTR 128 GPU starting");
	if(testCamelliaCTR128GPU() == 1){
		log("--- Test CAMELLIA CTR 128 GPU passed");
	}else{
		log("--- Test CAMELLIA CTR 128 GPU FAILED!");
		result = 0;
	}

	if(result != 0){
        log("--- --- All CAMELLIA 128 test passed");
    }else{
        log("--- --- Some CAMELLIA 128 Test FAILED");
    }

	return result;
}

int testCamellia192(){
	int result = 1;

	log("--- --- Starting CAMELLIA 192 tests");

	log("--- Test CAMELLIA 192 CPU starting");
	if(testCamellia192CPU() == 1){
		log("--- Test CAMELLIA 192 CPU passed");
	}else{
		log("--- Test CAMELLIA 192 CPU FAILED!");
		result = 0;
	}

	log("--- Test CAMELLIA 192 GPU starting");
	if(testCamellia192GPU() == 1){
		log("--- Test CAMELLIA 192 GPU passed");
	}else{
		log("--- Test CAMELLIA 192 GPU FAILED!");
		result = 0;
	}

	log("--- Test CAMELLIA CTR 192 CPU starting");
	if(testCamelliaCTR192CPU() == 1){
		log("--- Test CAMELLIA CTR 192 CPU passed");
	}else{
		log("--- Test CAMELLIA CTR 192 CPU FAILED!");
		result = 0;
	}

	log("--- Test CAMELLIA CTR 192 GPU starting");
	if(testCamelliaCTR192GPU() == 1){
		log("--- Test CAMELLIA CTR 192 GPU passed");
	}else{
		log("--- Test CAMELLIA CTR 192 GPU FAILED!");
		result = 0;
	}

	if(result != 0){
        log("--- --- All CAMELLIA 192 test passed");
    }else{
        log("--- --- Some CAMELLIA 192 Test FAILED");
    }

    return result;
}

int testCamellia256(){
	int result = 1;

	log("--- --- Starting CAMELLIA 256 tests");

	log("--- Test CAMELLIA 256 CPU starting");
	if(testCamellia256CPU() == 1){
		log("--- Test CAMELLIA 256 CPU passed");
	}else{
		log("--- Test CAMELLIA 256 CPU FAILED!");
		result = 0;
	}

	log("--- Test CAMELLIA 256 GPU starting");
	if(testCamellia256GPU() == 1){
		log("--- Test CAMELLIA 256 GPU passed");
	}else{
		log("--- Test CAMELLIA 256 GPU FAILED!");
		result = 0;
	}

	log("--- Test CAMELLIA CTR 256 CPU starting");
	if(testCamelliaCTR256CPU() == 1){
		log("--- Test CAMELLIA CTR 256 CPU passed");
	}else{
		log("--- Test CAMELLIA CTR 256 CPU FAILED!");
		result = 0;
	}

	log("--- Test CAMELLIA CTR 256 GPU starting");
	if(testCamelliaCTR256GPU() == 1){
		log("--- Test CAMELLIA CTR 256 GPU passed");
	}else{
		log("--- Test CAMELLIA CTR 256 GPU FAILED!");
		result = 0;
	}

	if(result != 0){
        log("--- --- All CAMELLIA 256 test passed");
    }else{
        log("--- --- Some CAMELLIA 256 Test FAILED");
    }

	return result;
}


int testCamelliaAll(){
	
	int tCamellia128 = testCamellia128();
	int tCamellia192 = testCamellia192();
	int tCamellia256 = testCamellia192();
	int result = tCamellia128&&tCamellia192&&tCamellia256;
	if(result){
		log("--- --- --- ALL CAMELLIA TEST PASSED");
	}else{
		log("--- --- --- TEST CAMELLIA FAILED");
	}
	return result;	
}



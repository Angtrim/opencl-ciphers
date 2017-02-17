#define DEBUG_LOG 1
#include <stdio.h>
#include <inttypes.h>
#include <stdlib.h>

#include "test_utils.h"

#include "test_present.h"
#include "../present_ctr/present_cipher.h"

#include "../cipher_utils.h"

#define MAX_LOCAL_SIZE 64

#define CPU_DEVICE "CPU"
#define GPU_DEVICE "GPU"

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
	printf("\nwriting %d uint64_t..\n", lenght);
	for(int i = 0; i < 1; i++){
		printf("%016llx", output[i]);	
	}
	fwrite(output, sizeof(uint64_t), lenght, fp);
	fclose(fp);
}

// TEST FOR PRESENT MEMORY

int testPresentMemoryCPU(){
	int success = 1;
	uint64_t* presentCiphertext = (uint64_t*)malloc(sizeof(uint64_t));
	writeOutputToFileUint64("tests/present_plaintext", PresentPlaintext, 1);
	present_memory_Encrypt("tests/present_plaintext", PresentKey, presentCiphertext, 1, CPU_DEVICE);
	
	if (presentCiphertext[0] != PresentC) {
		success = 0;
	}
	free(presentCiphertext);
	remove("tests/present_plaintext");
	return success;
}

int testPresentMemoryGPU(){
	int success = 1;
	uint64_t* presentCiphertext = (uint64_t*)malloc(sizeof(uint64_t));
	writeOutputToFileUint64("tests/present_plaintext", PresentPlaintext, 1);
	present_memory_Encrypt("tests/present_plaintext", PresentKey, presentCiphertext, 1, GPU_DEVICE);

	if (presentCiphertext[0] != PresentC) {
		success = 0;
	}
	free(presentCiphertext);
	remove("tests/present_plaintext");
	return success;
}

// TEST FOR PRESENT SPEED

int testPresentSpeedCPU(){
	int success = 1;
	uint64_t* presentCiphertext = (uint64_t*)malloc(sizeof(uint64_t));
	writeOutputToFileUint64("tests/present_plaintext", PresentPlaintext, 1);
	present_speed_Encrypt("tests/present_plaintext", PresentKey, presentCiphertext, 1, CPU_DEVICE);

	if (presentCiphertext[0] != PresentC) {
		success = 0;
	}
	free(presentCiphertext);
	remove("tests/present_plaintext");
	return success;
}

int testPresentSpeedGPU(){
	int success = 1;
	uint64_t* presentCiphertext = (uint64_t*)malloc(sizeof(uint64_t));
	writeOutputToFileUint64("tests/present_plaintext", PresentPlaintext, 1);
	present_speed_Encrypt("tests/present_plaintext", PresentKey, presentCiphertext, 1, GPU_DEVICE);

	if (presentCiphertext[0] != PresentC) {
		success = 0;
	}
	free(presentCiphertext);
	remove("tests/present_plaintext");
	return success;
}

// TEST FOR PRESENT MEMORY CTR

int testPresentMemoryCtrCPU(){
	int success = 1;
	struct FileInfo64 fileInfo = getFileUint64("tests/ctr_test");
	long dim = fileInfo.lenght;
	uint64_t* presentCiphertext = (uint64_t*)malloc(dim*sizeof(uint64_t));
	uint64_t* presentPlaintext = (uint64_t*)malloc(dim*sizeof(uint64_t));

	present_memory_CtrEncrypt("tests/ctr_test", PresentKey, presentCiphertext, 2, CPU_DEVICE);
	writeOutputToFileUint64("tests/present_ciphertext", presentCiphertext, dim);
	present_memory_CtrDecrypt("tests/present_ciphertext", PresentKey, presentPlaintext, 2, CPU_DEVICE);

	if(memcmp(presentPlaintext, fileInfo.filePointer, fileInfo.lenght) != 0){
		success = 0;
	}

	free(presentCiphertext);
	free(presentPlaintext);
	free(fileInfo.filePointer);
	remove("tests/present_ciphertext");
	return success;
}

int testPresentMemoryCtrGPU(){
	int success = 1;
	struct FileInfo64 fileInfo = getFileUint64("tests/ctr_test");
	long dim = fileInfo.lenght;
	uint64_t* presentCiphertext = (uint64_t*)malloc(dim*sizeof(uint64_t));
	uint64_t* presentPlaintext = (uint64_t*)malloc(dim*sizeof(uint64_t));

	present_memory_CtrEncrypt("tests/ctr_test", PresentKey, presentCiphertext, 2, GPU_DEVICE);
	writeOutputToFileUint64("tests/present_ciphertext", presentCiphertext, dim);
	present_memory_CtrDecrypt("tests/present_ciphertext", PresentKey, presentPlaintext, 2, GPU_DEVICE);

	if(memcmp(presentPlaintext, fileInfo.filePointer, fileInfo.lenght) != 0){
		success = 0;
	}

	free(presentCiphertext);
	free(presentPlaintext);
	free(fileInfo.filePointer);
	remove("tests/present_ciphertext");
	return success;
}

// TEST FOR PRESENT SPEED CTR

int testPresentSpeedCtrCPU(){
	int success = 1;
	struct FileInfo64 fileInfo = getFileUint64("tests/ctr_test");
	long dim = fileInfo.lenght;
	uint64_t* presentCiphertext = (uint64_t*)malloc(dim*sizeof(uint64_t));
	uint64_t* presentPlaintext = (uint64_t*)malloc(dim*sizeof(uint64_t));

	present_speed_CtrEncrypt("tests/ctr_test", PresentKey, presentCiphertext, 2, CPU_DEVICE);
	writeOutputToFileUint64("tests/present_ciphertext", presentCiphertext, dim);
	present_speed_CtrDecrypt("tests/present_ciphertext", PresentKey, presentPlaintext, 2, CPU_DEVICE);

	if(memcmp(presentPlaintext, fileInfo.filePointer, fileInfo.lenght) != 0){
		success = 0;
	}

	free(presentCiphertext);
	free(presentPlaintext);
	free(fileInfo.filePointer);
	remove("tests/present_ciphertext");
	return success;
}

int testPresentSpeedCtrGPU(){
	int success = 1;
	struct FileInfo64 fileInfo = getFileUint64("tests/ctr_test");
	long dim = fileInfo.lenght;
	uint64_t* presentCiphertext = (uint64_t*)malloc(dim*sizeof(uint64_t));
	uint64_t* presentPlaintext = (uint64_t*)malloc(dim*sizeof(uint64_t));

	present_speed_CtrEncrypt("tests/ctr_test", PresentKey, presentCiphertext, 2, GPU_DEVICE);
	writeOutputToFileUint64("tests/present_ciphertext", presentCiphertext, dim);
	present_speed_CtrDecrypt("tests/present_ciphertext", PresentKey, presentPlaintext, 2, GPU_DEVICE);

	if(memcmp(presentPlaintext, fileInfo.filePointer, fileInfo.lenght) != 0){
		success = 0;
	}

	free(presentCiphertext);
	free(presentPlaintext);
	free(fileInfo.filePointer);
	remove("tests/present_ciphertext");
	return success;
}

int testPresentMemory(){
	int result = 1;
	log("\n--- --- Starting PRESENT MEMORY tests");
	
	log("\n\n--- Test PRESENT MEMORY CPU starting");
	if(testPresentMemoryCPU() == 1){
		log("--- Test PRESENT MEMORY CPU passed");
	}else{
		log("--- Test PRESENT MEMORY CPU FAILED!");
		result = 0;
	}

	log("\n\n--- Test PRESENT MEMORY GPU starting");
	if(testPresentMemoryGPU() == 1){
		log("--- Test PRESENT MEMORY GPU passed");
	}else{
		log("--- Test PRESENT MEMORY GPU FAILED!");
		result = 0;
	}

	log("\n\n--- Test PRESENT MEMORY CTR CPU starting");
	if(testPresentMemoryCtrCPU() == 1){
		log("--- Test PRESENT MEMORY CTR CPU passed");
	}else{
		log("--- Test PRESENT MEMORY CTR CPU FAILED!");
		result = 0;
	}

	log("\n\n--- Test PRESENT MEMORY CTR GPU starting");
	if(testPresentMemoryCtrGPU() == 1){
		log("--- Test PRESENT MEMORY CTR GPU passed");
	}else{
		log("--- Test PRESENT MEMORY CTR GPU FAILED!");
		result = 0;
	}

	if(result != 0){
		log("\n--- --- All PRESENT MEMORY test passed");
	}else{
		log("\n--- --- Some PRESENT MEMORY Test FAILED");
	}
	return result;
}

int testPresentSpeed(){
	int result = 1;
	log("\n--- --- Starting PRESENT SPEED tests");
	
	log("\n\n--- Test PRESENT SPEED CPU starting");
	if(testPresentSpeedCPU() == 1){
		log("--- Test PRESENT SPEED CPU passed");
	}else{
		log("--- Test PRESENT SPEED CPU FAILED!");
		result = 0;
	}

	log("\n\n--- Test PRESENT SPEED GPU starting");
	if(testPresentSpeedGPU() == 1){
		log("--- Test PRESENT SPEED GPU passed");
	}else{
		log("--- Test PRESENT SPEED GPU FAILED!");
		result = 0;
	}

	log("\n\n--- Test PRESENT SPEED CTR CPU starting");
	if(testPresentSpeedCtrCPU() == 1){
		log("--- Test PRESENT SPEED CTR CPU passed");
	}else{
		log("--- Test PRESENT SPEED CTR CPU FAILED!");
		result = 0;
	}

	log("\n\n--- Test PRESENT SPEED CTR GPU starting");
	if(testPresentSpeedCtrGPU() == 1){
		log("--- Test PRESENT SPEED CTR GPU passed");
	}else{
		log("--- Test PRESENT SPEED CTR GPU FAILED!");
		result = 0;
	}

	if(result != 0){
		log("\n--- --- All PRESENT SPEED test passed");
	}else{
		log("\n--- --- Some PRESENT SPEED Test FAILED");
	}
	return result;
}

int testPresentAll(){

	int tPresentMemory = testPresentMemory();
	int tPresentSpeed = testPresentSpeed();
	int result = tPresentMemory&&tPresentSpeed;
	if(result){
		log("\n--- --- --- ALL PRESENT TEST PASSED");
	}else{
		log("\n--- --- --- TEST PRESENT FAILED");
	}
	return result;
}
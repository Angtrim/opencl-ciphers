
#include <stdio.h>
#include <inttypes.h>
#include <stdlib.h>

#include "test_utils.h"

#include "test_aes.h"

#include "../cipher_utils.h"

//includes for testing normal encryption
#include "../aes_test_vect.h"

#define MAX_LOCAL_SIZE 64

#define CPU_DEVICE "CPU"
#define GPU_DEVICE "GPU"


// to save ciphertext to file (uint8_t)
static void writeOutputToFile(char* outFileName,uint8_t* output, long lenght){
	FILE* fp = fopen(outFileName, "wb");
	if (!fp) {
		fprintf(stderr, "Failed to load file.\n");
		exit(1);
	}
	printf("\nwriting %d bytes..\n", lenght);
	for(int i = 0; i <16; i++){
		printf("%x", output[i]);	
	}
	fwrite(output, sizeof(char), lenght, fp);
	fclose(fp);
}

// TEST FOR 128

int testAes128CPU(){


	int success = 1;
	uint8_t* aesCiphertext = (uint8_t*)malloc((16000+1)*sizeof(uint8_t));
	aes128Encrypt("tests/aes_plaintext", aes128Key, aesCiphertext, 2, CPU_DEVICE);
	if (memcmp(aesCiphertext, aes128Ciphertext, 16000) != 0) {
		success = 0;
	}
	free(aesCiphertext);
	return success;
}

int testAes128GPU(){
	int success = 1;
	uint8_t* aesCiphertext = (uint8_t*)malloc((16000+1)*sizeof(uint8_t));
	uint8_t* aesPlaintext;
	aes128Encrypt("tests/aes_plaintext", aes128Key, aesCiphertext, 2, GPU_DEVICE);
	if (memcmp(aesCiphertext, aes128Ciphertext, 16000) != 0) {
		success = 0;
	}
	free(aesCiphertext);
	return success;
}

int testAes128CtrCPU(){
	int success = 1;
	struct FileInfo fileInfo = getFileBytes("tests/ctr_test");
	long dim = getByteLenght("tests/ctr_test");
	uint8_t* aesCiphertext = (uint8_t*)malloc(dim*sizeof(uint8_t));
	uint8_t* aesPlaintext = (uint8_t*)malloc(dim*sizeof(uint8_t));
	aes128CtrEncrypt("tests/ctr_test", aes128Key, aesCiphertext, 2, CPU_DEVICE);
	writeOutputToFile("tests/aes_ciphertext", aesCiphertext, fileInfo.lenght);
	aes128CtrDecrypt("tests/aes_ciphertext", aes128Key, aesPlaintext, 2, CPU_DEVICE);
	
	if (memcmp(aesPlaintext, fileInfo.filePointer, fileInfo.lenght) != 0) {
		success = 0;
	} 

	free(aesCiphertext);
	free(aesPlaintext);
	free(fileInfo.filePointer);
	return success;
}

int testAes128CtrGPU(){
	int success = 1;
	struct FileInfo fileInfo = getFileBytes("tests/ctr_test");
	long dim = getByteLenght("tests/ctr_test");
	uint8_t* aesCiphertext = (uint8_t*)malloc(dim*sizeof(uint8_t));
	uint8_t* aesPlaintext = (uint8_t*)malloc(dim*sizeof(uint8_t));
	
	aes128CtrEncrypt("tests/ctr_test", aes128Key, aesCiphertext, 2, GPU_DEVICE);
	writeOutputToFile("tests/aes_ciphertext", aesCiphertext, fileInfo.lenght);
	aes128CtrDecrypt("tests/aes_ciphertext", aes128Key, aesPlaintext, 2, GPU_DEVICE);
	
	if (memcmp(aesPlaintext, fileInfo.filePointer, fileInfo.lenght) != 0) {
		success = 0;
	}

	free(aesCiphertext);
	free(aesPlaintext);
	free(fileInfo.filePointer);
	return success;
}


// Test 192

int testAes192CPU(){
	int success = 1;
	uint8_t* aesCiphertext = (uint8_t*)malloc((16000+1)*sizeof(uint8_t));
	uint8_t* aesPlaintext;
	aes192Encrypt("tests/aes_plaintext", aes192Key, aesCiphertext, 1, CPU_DEVICE);
	if (memcmp(aesCiphertext, aes192Ciphertext, 16000) != 0) {
		success = 0;
	}
	free(aesCiphertext);
	return success;
}

int testAes192GPU(){
	int success = 1;
	uint8_t* aesCiphertext = (uint8_t*)malloc((16000+1)*sizeof(uint8_t));
	uint8_t* aesPlaintext;
	aes192Encrypt("tests/aes_plaintext", aes192Key, aesCiphertext, 2, GPU_DEVICE);
	if (memcmp(aesCiphertext, aes192Ciphertext, 16000) != 0) {
		success = 0;
	}
	free(aesCiphertext);
	return success;
}

int testAes192CtrCPU(){
	int success = 1;
	struct FileInfo fileInfo = getFileBytes("tests/ctr_test");
	long dim = getByteLenght("tests/ctr_test");
	uint8_t* aesCiphertext = (uint8_t*)malloc(dim*sizeof(uint8_t));
	uint8_t* aesPlaintext = (uint8_t*)malloc(dim*sizeof(uint8_t));
	aes192CtrEncrypt("tests/ctr_test", aes192Key, aesCiphertext, 2, CPU_DEVICE);
	writeOutputToFile("tests/aes_ciphertext", aesCiphertext, fileInfo.lenght);
	aes192CtrDecrypt("tests/aes_ciphertext", aes192Key, aesPlaintext, 2, CPU_DEVICE);
	
	if (memcmp(aesPlaintext, fileInfo.filePointer, fileInfo.lenght) != 0) {
		success = 0;
	} 

	free(aesCiphertext);
	free(aesPlaintext);
	free(fileInfo.filePointer);
	return success;
}

int testAes192CtrGPU(){
	int success = 1;
	struct FileInfo fileInfo = getFileBytes("tests/ctr_test");
	long dim = getByteLenght("tests/ctr_test");
	uint8_t* aesCiphertext = (uint8_t*)malloc(dim*sizeof(uint8_t));
	uint8_t* aesPlaintext = (uint8_t*)malloc(dim*sizeof(uint8_t));
	
	aes192CtrEncrypt("tests/ctr_test", aes192Key, aesCiphertext, 2, GPU_DEVICE);
	writeOutputToFile("tests/aes_ciphertext", aesCiphertext, fileInfo.lenght);
	aes192CtrDecrypt("tests/aes_ciphertext", aes192Key, aesPlaintext, 2, GPU_DEVICE);
	
	if (memcmp(aesPlaintext, fileInfo.filePointer, fileInfo.lenght) != 0) {
		success = 0;
	}

	free(aesCiphertext);
	free(aesPlaintext);
	free(fileInfo.filePointer);
	return success;
}


// Test 256
int testAes256CPU(){
	int success = 1;
	uint8_t* aesCiphertext = (uint8_t*)malloc((16000+1)*sizeof(uint8_t));
	uint8_t* aesPlaintext;
	aes256Encrypt("tests/aes_plaintext", aes256Key, aesCiphertext, 1, CPU_DEVICE);
	if (memcmp(aesCiphertext, aes256Ciphertext, 16000) != 0) {
		success = 0;
	}
	free(aesCiphertext);
	return success;
}

int testAes256GPU(){
	int success = 1;
	uint8_t* aesCiphertext = (uint8_t*)malloc((16000+1)*sizeof(uint8_t));
	uint8_t* aesPlaintext;
	aes256Encrypt("tests/aes_plaintext", aes256Key, aesCiphertext, 2, GPU_DEVICE);
	if (memcmp(aesCiphertext, aes256Ciphertext, 16000) != 0) {
		success = 0;
	}
	free(aesCiphertext);
	return success;
}

int testAes256CtrCPU(){
	int success = 1;
	struct FileInfo fileInfo = getFileBytes("tests/ctr_test");
	long dim = getByteLenght("tests/ctr_test");
	uint8_t* aesCiphertext = (uint8_t*)malloc(dim*sizeof(uint8_t));
	uint8_t* aesPlaintext = (uint8_t*)malloc(dim*sizeof(uint8_t));
	aes256CtrEncrypt("tests/ctr_test", aes256Key, aesCiphertext, 2, CPU_DEVICE);
	writeOutputToFile("tests/aes_ciphertext", aesCiphertext, fileInfo.lenght);
	aes256CtrDecrypt("tests/aes_ciphertext", aes256Key, aesPlaintext, 2, CPU_DEVICE);
	
	if (memcmp(aesPlaintext, fileInfo.filePointer, fileInfo.lenght) != 0) {
		success = 0;
	} 

	free(aesCiphertext);
	free(aesPlaintext);
	free(fileInfo.filePointer);
	return success;
}

int testAes256CtrGPU(){
	int success = 1;
	struct FileInfo fileInfo = getFileBytes("tests/ctr_test");
	long dim = getByteLenght("tests/ctr_test");
	uint8_t* aesCiphertext = (uint8_t*)malloc(dim*sizeof(uint8_t));
	uint8_t* aesPlaintext = (uint8_t*)malloc(dim*sizeof(uint8_t));
	
	aes256CtrEncrypt("tests/ctr_test", aes256Key, aesCiphertext, 2, GPU_DEVICE);
	writeOutputToFile("tests/aes_ciphertext", aesCiphertext, fileInfo.lenght);
	aes256CtrDecrypt("tests/aes_ciphertext", aes256Key, aesPlaintext, 2, GPU_DEVICE);
	
	if (memcmp(aesPlaintext, fileInfo.filePointer, fileInfo.lenght) != 0) {
		success = 0;
	}

	free(aesCiphertext);
	free(aesPlaintext);
	free(fileInfo.filePointer);
	return success;
}



int testAESAll(){

	int t128 = test128();
	int t192 = test192();
	int t256 = test256();	
	int result = t128&&t192&&t256;
	if(result){
		log("--- --- --- ALL AES TEST PASSED");
	}else{
		log("--- --- --- TEST AES FAILED");
	}
	return result;

}


int test128(){
	int result = 1;
	log("--- --- Starting AES128 tests");
	
	log("--- Test AES 128 CPU starting");
	if(testAes128CPU() == 1){
		log("--- Test AES 128 CPU passed");
	}else{
		log("--- Test AES 128 CPU FAILED!");
		result = 0;
	}

	log("--- Test AES 128 GPU starting");
	if(testAes128GPU() == 1){
		log("--- Test AES 128 GPU passed");
	}else{
		log("--- Test AES 128 GPU FAILED!");
		result = 0;
	}

	log("--- Test AES 128 CTR CPU starting");
	if(testAes128CtrCPU() == 1){
		log("--- Test AES 128 CTR CPU passed");
	}else{
		log("--- Test AES 128 CTR CPU FAILED!");
		result = 0;
	}

	log("--- Test AES 128 CTR GPU starting");
	if(testAes128CtrGPU() == 1){
		log("--- Test AES 128 CTR GPU passed");
	}else{
		log("--- Test AES 128 CTR GPU FAILED!");
		result = 0;
	}

	if(result != 0){
		log("--- --- All Aes 128 test passed");
	}else{
		log("--- --- Some AES 128 Test FAILED");
	}
	return result;

}

int test192(){
	int result = 1;
	log("--- --- Starting AES192 tests");
	
	log("--- Test AES 192 CPU starting");
	if(testAes192CPU() == 1){
		log("--- Test AES 192 CPU passed");
	}else{
		log("--- Test AES 192 CPU FAILED!");
		result = 0;
	}

	log("--- Test AES 192 GPU starting");
	if(testAes192GPU() == 1){
		log("--- Test AES 192 GPU passed");
	}else{
		log("--- Test AES 192 GPU FAILED!");
		result = 0;
	}

	log("--- Test AES 192 CTR CPU starting");
	if(testAes192CtrCPU() == 1){
		log("--- Test AES 192 CTR CPU passed");
	}else{
		log("--- Test AES 192 CTR CPU FAILED!");
		result = 0;
	}

	log("--- Test AES 192 CTR GPU starting");
	if(testAes192CtrGPU() == 1){
		log("--- Test AES 192 CTR GPU passed");
	}else{
		log("--- Test AES 192 CTR GPU FAILED!");
		result = 0;
	}

	if(result != 0){
		log("--- --- All Aes 192 test passed");
	}else{
		log("--- --- Some AES 192 Test FAILED");
	}

	return result;
	
}

int test256(){
	int result = 1;
	log("--- --- Starting AES256 tests");
	
	log("--- Test AES 256 CPU starting");
	if(testAes256CPU() == 1){
		log("--- Test AES 256 CPU passed");
	}else{
		log("--- Test AES 256 CPU FAILED!");
		result = 0;
	}

	log("--- Test AES 256 GPU starting");
	if(testAes256GPU() == 1){
		log("--- Test AES 256 GPU passed");
	}else{
		log("--- Test AES 256 GPU FAILED!");
		result = 0;
	}

	log("--- Test AES 256 CTR CPU starting");
	if(testAes256CtrCPU() == 1){
		log("--- Test AES 256 CTR CPU passed");
	}else{
		log("--- Test AES 256 CTR CPU FAILED!");
		result = 0;
	}

	log("--- Test AES 256 CTR GPU starting");
	if(testAes256CtrGPU() == 1){
		log("--- Test AES 256 CTR GPU passed");
	}else{
		log("--- Test AES 256 CTR GPU FAILED!");
		result = 0;
	}

	if(result != 0){
		log("--- --- All Aes 256 test passed");
	}else{
		log("--- --- Some AES 256 Test FAILED");
	}

	return result;
	
}









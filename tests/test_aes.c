
#include <stdio.h>
#include <inttypes.h>
#include <stdlib.h>

#include "test_utils.h"

#include "test_aes.h"

#include "../cipher_utils.h"

#define MAX_LOCAL_SIZE 64

#define CPU_DEVICE "CPU"
#define GPU_DEVICE "GPU"

static uint32_t aes128Key[16] = {0x6ede8bab, 0xdad63ffa, 0x796c767d, 0x92e52e38};
static uint32_t aes192Key[24] = {0xded642b8, 0x2daf73f0, 0x7a0f5caa, 0x554dc9c6, 0x21437628, 0x99c92ca2};
static uint32_t aes256Key[32] = {0x63375233, 0xca899f06, 0x2f868c91, 0x8eb2ccc1, 0x36eb5001, 0x58c75cbb, 0x2ee60020, 0x3286d86f};

uint8_t aesPlaintext[128]= {
	0x53, 0x9c, 0x7a, 0x6e, 0x4c, 0x11, 0x35, 0xba, 0xe1, 0xa4, 0x8e, 0x7e, 0xb1, 0xe7, 0x57, 0x15 ,
	0xd6, 0xea, 0x51, 0x91, 0x68, 0x66, 0x82, 0x03, 0x5c, 0xcc, 0x96, 0xf7, 0x87, 0xc9, 0x18, 0x03 ,
	0x4e, 0x02, 0x49, 0x79, 0xec, 0x3b, 0xb3, 0x7b, 0xfc, 0x89, 0xf6, 0x3b, 0xd0, 0x4b, 0x0c, 0x21 ,
	0xee, 0x69, 0xbd, 0xc0, 0x5a, 0x0f, 0x39, 0x06, 0x11, 0xf5, 0x0a, 0x42, 0x2b, 0x21, 0x7d, 0x0a ,
	0x00, 0x6c, 0x27, 0x4e, 0x82, 0xee, 0xbc, 0x33, 0x7b, 0x4d, 0x44, 0xc9, 0x9e, 0x1a, 0x7b, 0x16 ,
	0x2c, 0x8b, 0xa5, 0xb6, 0x9d, 0xca, 0x21, 0xf8, 0xcd, 0xeb, 0xbe, 0x05, 0x70, 0xeb, 0x5f, 0x66 ,
	0x44, 0xa6, 0xc9, 0x5e, 0x82, 0x63, 0xe2, 0x50, 0xa0, 0xf9, 0x83, 0x7f, 0xa9, 0x60, 0xce, 0x44 ,
	0x16, 0x03, 0xa0, 0x89, 0x9b, 0x99, 0x0a, 0x66, 0x07, 0x08, 0x18, 0xfb, 0x45, 0xd9, 0xad, 0x08 };
uint8_t aes128Ciphertext[128]= {
	0x32, 0x3f, 0x3d, 0x7b, 0x33, 0x30, 0xf5, 0x5f, 0x46, 0xb3, 0xce, 0x31, 0xf8, 0x92, 0xd9, 0x8c ,
	0x4a, 0xa0, 0x40, 0xc4, 0x72, 0x38, 0xad, 0xaf, 0xbc, 0x78, 0xe1, 0xf0, 0x0e, 0x4e, 0x16, 0x81 ,
	0xee, 0x09, 0xf8, 0xee, 0xe2, 0xcd, 0x52, 0x59, 0x74, 0xd4, 0x9c, 0xd0, 0xc7, 0x38, 0xba, 0xff ,
	0x74, 0x62, 0xb3, 0x31, 0x86, 0x2e, 0xb0, 0x11, 0xc0, 0x79, 0x7d, 0x1d, 0x89, 0xe6, 0x3c, 0xae ,
	0x6d, 0xae, 0x4c, 0x11, 0xed, 0x59, 0x74, 0xcf, 0xe0, 0xee, 0xd9, 0xca, 0xad, 0xda, 0x4a, 0x96 ,
	0x5c, 0xfa, 0xa7, 0xe1, 0x76, 0x64, 0x76, 0xd2, 0x0d, 0x06, 0x03, 0x0d, 0xeb, 0x57, 0x3c, 0x27 ,
	0xa8, 0x06, 0xf0, 0x56, 0x89, 0x6d, 0x59, 0x2d, 0xc8, 0xa2, 0x7a, 0xbb, 0x22, 0xab, 0x12, 0xd9 ,
	0xd9, 0x56, 0x64, 0x22, 0xec, 0x76, 0x53, 0x62, 0x7f, 0xcb, 0x1e, 0xf5, 0xdb, 0x80, 0xab, 0x36 };
uint8_t aes192Ciphertext[128] = {
	0x9a, 0x79, 0xc4, 0xae, 0xc0, 0xb7, 0x6a, 0x28, 0x91, 0xbd, 0xdc, 0xbc, 0x8d, 0xe4, 0x81, 0x89 ,
	0xe1, 0x1f, 0x75, 0xea, 0x2c, 0x8f, 0xd7, 0xde, 0x58, 0x01, 0x88, 0xdf, 0x7e, 0x62, 0x7a, 0x80 ,
	0xdb, 0x93, 0x20, 0x63, 0xc2, 0x2a, 0x13, 0x25, 0xfc, 0x3f, 0x1a, 0x80, 0xb6, 0xa0, 0xb2, 0x90 ,
	0xaa, 0x61, 0xa0, 0x02, 0x70, 0x11, 0xfe, 0x70, 0x47, 0x22, 0xda, 0xb5, 0x0f, 0x69, 0xfb, 0x7a ,
	0x23, 0x24, 0x0c, 0x5c, 0xff, 0x8c, 0xa4, 0xb4, 0x99, 0xcb, 0x66, 0x23, 0x4c, 0xa6, 0xd3, 0x1f ,
	0xa4, 0xdd, 0x89, 0xb4, 0x45, 0xac, 0x75, 0x0f, 0xb4, 0x13, 0x47, 0x04, 0xd2, 0x94, 0xf9, 0x88 ,
	0x6c, 0xe4, 0xb8, 0xff, 0xda, 0x02, 0x21, 0x20, 0x71, 0x8c, 0xe9, 0x26, 0xa7, 0x62, 0x0a, 0xba ,
	0xa9, 0x22, 0x3b, 0x96, 0xa7, 0xb7, 0x51, 0x09, 0x09, 0xc0, 0x30, 0x35, 0x4e, 0x94, 0x5c, 0xfb };
 uint8_t aes256Ciphertext[128] = { 
	0xbe, 0x5e, 0x3c, 0x05, 0x48, 0x44, 0xf6, 0x9d, 0x54, 0x1a, 0x5d, 0x60, 0x87, 0x0a, 0x69, 0xd1 ,
	0xd4, 0x57, 0x07, 0xc8, 0xef, 0xa2, 0xa4, 0x8a, 0x52, 0xbb, 0x0d, 0xe8, 0x59, 0xe5, 0x9e, 0x45 ,
	0x98, 0xa5, 0xc7, 0x69, 0x00, 0xb4, 0x93, 0xe3, 0x9e, 0x4b, 0x68, 0xef, 0x0a, 0xe4, 0xd3, 0x66 ,
	0xb8, 0x4d, 0x45, 0x36, 0xcc, 0xf9, 0x4e, 0x9d, 0xac, 0xef, 0x2a, 0x07, 0xab, 0xdf, 0x8f, 0x67 ,
	0xad, 0x40, 0x26, 0x1e, 0x5d, 0xf7, 0x22, 0x28, 0xae, 0xb8, 0xeb, 0xe2, 0x29, 0x2e, 0x89, 0xd3 ,
	0xac, 0x2d, 0x8d, 0xfb, 0x53, 0x9e, 0xd3, 0xe6, 0x49, 0x4d, 0xfd, 0x21, 0xb7, 0xab, 0x90, 0x85 ,
	0x86, 0xe7, 0x8c, 0x81, 0x3f, 0x07, 0x3a, 0xb9, 0x81, 0x3a, 0xdb, 0xad, 0x6a, 0x1d, 0x50, 0x09 ,
	0xda, 0xb1, 0x22, 0x0b, 0xe1, 0x7c, 0xdb, 0x61, 0x3d, 0xa9, 0xeb, 0x13, 0x70, 0x74, 0xae, 0xde };


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

	uint8_t* aesCiphertext = (uint8_t*)malloc(128*sizeof(uint8_t));
	writeOutputToFile("tests/aes_plaintext", aesPlaintext, 128);

	aes128Encrypt("tests/aes_plaintext", aes128Key, aesCiphertext, 1, CPU_DEVICE);
	if (memcmp(aesCiphertext, aes128Ciphertext, 128) != 0) {
		success = 0;
	}
	free(aesCiphertext);
	remove("tests/aes_plaintext");
	return success;
}

int testAes128GPU(){
	int success = 1;
	uint8_t* aesCiphertext = (uint8_t*)malloc(128*sizeof(uint8_t));
	writeOutputToFile("tests/aes_plaintext", aesPlaintext, 128);

	aes128Encrypt("tests/aes_plaintext", aes128Key, aesCiphertext, 2, GPU_DEVICE);
	if (memcmp(aesCiphertext, aes128Ciphertext, 128) != 0) {
		success = 0;
	}
	free(aesCiphertext);
	remove("tests/aes_plaintext");
	return success;
}

int testAes128CtrCPU(){
	int success = 1;
	struct FileInfo fileInfo = getFileBytes("tests/ctr_test");
	long dim = fileInfo.lenght;
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
	remove("tests/aes_ciphertext");
	return success;
}

int testAes128CtrGPU(){
	int success = 1;
	struct FileInfo fileInfo = getFileBytes("tests/ctr_test");
	long dim = fileInfo.lenght;
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
	remove("tests/aes_ciphertext");
	return success;
}


// Test 192

int testAes192CPU(){
	int success = 1;
	uint8_t* aesCiphertext = (uint8_t*)malloc(128*sizeof(uint8_t));
	writeOutputToFile("tests/aes_plaintext", aesPlaintext, 128);

	aes192Encrypt("tests/aes_plaintext", aes192Key, aesCiphertext, 1, CPU_DEVICE);
	if (memcmp(aesCiphertext, aes192Ciphertext, 128) != 0) {
		success = 0;
	}
	free(aesCiphertext);
	remove("tests/aes_plaintext");
	return success;
}

int testAes192GPU(){
	int success = 1;
	uint8_t* aesCiphertext = (uint8_t*)malloc(128*sizeof(uint8_t));
	writeOutputToFile("tests/aes_plaintext", aesPlaintext, 128);

	aes192Encrypt("tests/aes_plaintext", aes192Key, aesCiphertext, 2, GPU_DEVICE);
	if (memcmp(aesCiphertext, aes192Ciphertext, 128) != 0) {
		success = 0;
	}
	free(aesCiphertext);
	remove("tests/aes_plaintext");
	return success;
}

int testAes192CtrCPU(){
	int success = 1;
	struct FileInfo fileInfo = getFileBytes("tests/ctr_test");
	long dim = fileInfo.lenght;
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
	remove("tests/aes_ciphertext");
	return success;
}

int testAes192CtrGPU(){
	int success = 1;
	struct FileInfo fileInfo = getFileBytes("tests/ctr_test");
	long dim = fileInfo.lenght;
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
	remove("tests/aes_ciphertext");
	return success;
}


// Test 256
int testAes256CPU(){
	int success = 1;
	uint8_t* aesCiphertext = (uint8_t*)malloc(128*sizeof(uint8_t));
	writeOutputToFile("tests/aes_plaintext", aesPlaintext, 128);
	aes256Encrypt("tests/aes_plaintext", aes256Key, aesCiphertext, 1, CPU_DEVICE);

	if (memcmp(aesCiphertext, aes256Ciphertext, 128) != 0) {
		success = 0;
	}
	free(aesCiphertext);
	remove("tests/aes_plaintext");
	return success;
}

int testAes256GPU(){
	int success = 1;
	uint8_t* aesCiphertext = (uint8_t*)malloc(128*sizeof(uint8_t));
	writeOutputToFile("tests/aes_plaintext", aesPlaintext, 128);
	aes256Encrypt("tests/aes_plaintext", aes256Key, aesCiphertext, 2, GPU_DEVICE);

	if (memcmp(aesCiphertext, aes256Ciphertext, 128) != 0) {
		success = 0;
	}
	free(aesCiphertext);
	remove("tests/aes_plaintext");
	return success;
}

int testAes256CtrCPU(){
	int success = 1;
	struct FileInfo fileInfo = getFileBytes("tests/ctr_test");
	long dim = fileInfo.lenght;
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
	remove("tests/aes_ciphertext");
	return success;
}

int testAes256CtrGPU(){
	int success = 1;
	struct FileInfo fileInfo = getFileBytes("tests/ctr_test");
	long dim = fileInfo.lenght;
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
	remove("tests/aes_ciphertext");
	return success;
}





int test128(){
	int result = 1;
	log("\n--- --- Starting AES128 tests");
	
	log("\n\n--- Test AES 128 CPU starting");
	if(testAes128CPU() == 1){
		log("--- Test AES 128 CPU passed");
	}else{
		log("--- Test AES 128 CPU FAILED!");
		result = 0;
	}

	log("\n\n--- Test AES 128 GPU starting");
	if(testAes128GPU() == 1){
		log("--- Test AES 128 GPU passed");
	}else{
		log("--- Test AES 128 GPU FAILED!");
		result = 0;
	}

	log("\n\n--- Test AES 128 CTR CPU starting");
	if(testAes128CtrCPU() == 1){
		log("--- Test AES 128 CTR CPU passed");
	}else{
		log("--- Test AES 128 CTR CPU FAILED!");
		result = 0;
	}

	log("\n\n--- Test AES 128 CTR GPU starting");
	if(testAes128CtrGPU() == 1){
		log("--- Test AES 128 CTR GPU passed");
	}else{
		log("--- Test AES 128 CTR GPU FAILED!");
		result = 0;
	}

	if(result != 0){
		log("\n--- --- All Aes 128 test passed");
	}else{
		log("\n--- --- Some AES 128 Test FAILED");
	}
	return result;

}

int test192(){
	int result = 1;
	log("--- --- Starting AES192 tests");
	
	log("\n--- Test AES 192 CPU starting");
	if(testAes192CPU() == 1){
		log("--- Test AES 192 CPU passed");
	}else{
		log("--- Test AES 192 CPU FAILED!");
		result = 0;
	}

	log("\n--- Test AES 192 GPU starting");
	if(testAes192GPU() == 1){
		log("--- Test AES 192 GPU passed");
	}else{
		log("--- Test AES 192 GPU FAILED!");
		result = 0;
	}

	log("\n--- Test AES 192 CTR CPU starting");
	if(testAes192CtrCPU() == 1){
		log("--- Test AES 192 CTR CPU passed");
	}else{
		log("--- Test AES 192 CTR CPU FAILED!");
		result = 0;
	}

	log("\n--- Test AES 192 CTR GPU starting");
	if(testAes192CtrGPU() == 1){
		log("--- Test AES 192 CTR GPU passed");
	}else{
		log("--- Test AES 192 CTR GPU FAILED!");
		result = 0;
	}

	if(result != 0){
		log("\n--- --- All Aes 192 test passed");
	}else{
		log("\n--- --- Some AES 192 Test FAILED");
	}

	return result;
	
}

int test256(){
	int result = 1;
	log("\n--- --- Starting AES256 tests");
	
	log("\n--- Test AES 256 CPU starting");
	if(testAes256CPU() == 1){
		log("--- Test AES 256 CPU passed");
	}else{
		log("--- Test AES 256 CPU FAILED!");
		result = 0;
	}

	log("\n--- Test AES 256 GPU starting");
	if(testAes256GPU() == 1){
		log("--- Test AES 256 GPU passed");
	}else{
		log("--- Test AES 256 GPU FAILED!");
		result = 0;
	}

	log("\n--- Test AES 256 CTR CPU starting");
	if(testAes256CtrCPU() == 1){
		log("--- Test AES 256 CTR CPU passed");
	}else{
		log("--- Test AES 256 CTR CPU FAILED!");
		result = 0;
	}

	log("\n--- Test AES 256 CTR GPU starting");
	if(testAes256CtrGPU() == 1){
		log("--- Test AES 256 CTR GPU passed");
	}else{
		log("--- Test AES 256 CTR GPU FAILED!");
		result = 0;
	}

	if(result != 0){
		log("\n--- --- All Aes 256 test passed");
	}else{
		log("\n--- --- Some AES 256 Test FAILED");
	}

	return result;
	
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









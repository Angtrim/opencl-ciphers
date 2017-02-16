

#define DEBUG_LOG 1
#include <stdio.h>
#include <inttypes.h>
#include <stdlib.h>

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



void log(char* message){
	if(DEBUG_LOG){
		printf("%s\n", message);
	}
}


int testAes128CPU(){
	int success = 1;
	uint8_t* aesCiphertext = (uint8_t*)malloc((16000+1)*sizeof(uint8_t));
 uint8_t* aesPlaintext;
 aes128Encrypt("tests/aes_plaintext", aes128Key, aesCiphertext, 1, CPU_DEVICE);
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


int main(){

	int result = 1;
	log("--- --- Starting AES128 tests");

	if(testAes128CPU() == 1){
		log("--- Test AES 128 CPU passed");
	}else{
		log("--- Test AES 128 CPU FAILED!");
		result = 0;
	}

	if(testAes128GPU() == 1){
		log("--- Test AES 128 GPU passed");
	}else{
		log("--- Test AES 128 GPU FAILED!");
		result = 0;
	}

	if(testAes128CtrCPU() == 1){
		log("--- Test AES 128 CTR CPU passed");
	}else{
		log("--- Test AES 128 CTR CPU FAILED!");
		result = 0;
	}

	if(testAes128CtrGPU() == 1){
		log("--- Test AES 128 CTR GPU passed");
	}else{
		log("--- Test AES 128 CTR GPU FAILED!");
		result = 0;
	}

	if(result != 0){
		log("--- --- All Aes 128 test passed");
	}

}









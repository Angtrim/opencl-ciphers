#include <stdio.h>
#include <inttypes.h>
#include <stdlib.h>
#include "test_utils.h"
#include "test_camelia.h"
#define CPU_DEVICE "CPU"
#define GPU_DEVICE "GPU"

uint64_t Plaintext128[2] = {0x0123456789abcdef, 0xfedcba9876543210};
uint64_t Ciphertext128[2] = {0x6767313854966973, 0x0857065648eabe43};
uint64_t Key128[2] = {0x0123456789abcdef, 0xfedcba9876543210};

uint64_t Plaintext192[2] = {0x0123456789abcdef, 0xfedcba9876543210};
uint64_t Ciphertext192[2] = {0xb4993401b3e996f8, 0x4ee5cee7d79b09b9};
uint64_t Key192[3] = {0x0123456789abcdef, 0xfedcba9876543210, 0x0011223344556677};
uint64_t Plaintext256[2] = {0x0123456789abcdef, 0xfedcba9876543210};
uint64_t Ciphertext256[2] = {0x9acc237dff16d76c, 0x20ef7c919e3a7509};
uint64_t Key256[4] = {0x0123456789abcdef, 0xfedcba9876543210,0x0011223344556677, 0x8899aabbccddeeff};



static void writeOutputToFileUint64(char* outFileName, uint64_t* output, long lenght){
	FILE* fp = fopen(outFileName, "wb");
	if (!fp) {
		fprintf(stderr, "Failed to load file.\n");
		exit(1);
	}
	printf("\nwriting %d uint64_t..\n", lenght);
	for(int i = 0; i <lenght; i++){
		printf("%x", output[i]);	
	}
	fwrite(output, sizeof(uint64_t), lenght, fp);
	fclose(fp);
}

int testCamelia128CPU(){
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

int testCamelia128GPU(){
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

int testCamelia192CPU(){
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

int testCamelia192GPU(){
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


int testCamelia256CPU(){
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

int testCamelia256GPU(){
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

int testCameliaCTR128CPU(){
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


int testCameliaCTR128GPU(){
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


int testCameliaCTR192CPU(){
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


int testCameliaCTR192GPU(){
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


int testCameliaCTR256CPU(){
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


int testCameliaCTR256GPU(){
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





int testCameliaAll(){
	int result = 1;
	log("--- --- Starting Camelia tests");
	
	log("--- Test Camelia 128 CPU starting");
	if(testCamelia128CPU() == 1){
		log("--- Test Camelia 128 CPU passed");
	}else{
		log("--- Test Camelia 128 CPU FAILED!");
		result = 0;
	}

	log("--- Test Camelia 128 GPU starting");
	if(testCamelia128GPU() == 1){
		log("--- Test Camelia 128 GPU passed");
	}else{
		log("--- Test Camelia 128 GPU FAILED!");
		result = 0;
	}

	log("--- Test Camelia 192 CPU starting");
	if(testCamelia192CPU() == 1){
		log("--- Test Camelia 192 CPU passed");
	}else{
		log("--- Test Camelia 192 CPU FAILED!");
		result = 0;
	}

	log("--- Test Camelia 192 GPU starting");
	if(testCamelia192GPU() == 1){
		log("--- Test Camelia 192 GPU passed");
	}else{
		log("--- Test Camelia 192 GPU FAILED!");
		result = 0;
	}

	log("--- Test Camelia 256 CPU starting");
	if(testCamelia256CPU() == 1){
		log("--- Test Camelia 256 CPU passed");
	}else{
		log("--- Test Camelia 256 CPU FAILED!");
		result = 0;
	}

	log("--- Test Camelia 256 GPU starting");
	if(testCamelia256GPU() == 1){
		log("--- Test Camelia 256 GPU passed");
	}else{
		log("--- Test Camelia 256 GPU FAILED!");
		result = 0;
	}

	log("--- Test Camelia CTR 128 CPU starting");
	if(testCameliaCTR128CPU() == 1){
		log("--- Test Camelia CTR 128 CPU passed");
	}else{
		log("--- Test Camelia CTR 128 CPU FAILED!");
		result = 0;
	}

	log("--- Test Camelia CTR 128 GPU starting");
	if(testCameliaCTR128GPU() == 1){
		log("--- Test Camelia CTR 128 GPU passed");
	}else{
		log("--- Test Camelia CTR 128 GPU FAILED!");
		result = 0;
	}

	log("--- Test Camelia CTR 192 CPU starting");
	if(testCameliaCTR192CPU() == 1){
		log("--- Test Camelia CTR 192 CPU passed");
	}else{
		log("--- Test Camelia CTR 192 CPU FAILED!");
		result = 0;
	}

	log("--- Test Camelia CTR 192 GPU starting");
	if(testCameliaCTR192GPU() == 1){
		log("--- Test Camelia CTR 192 GPU passed");
	}else{
		log("--- Test Camelia CTR 192 GPU FAILED!");
		result = 0;
	}

	log("--- Test Camelia CTR 256 CPU starting");
	if(testCameliaCTR256CPU() == 1){
		log("--- Test Camelia CTR 256 CPU passed");
	}else{
		log("--- Test Camelia CTR 256 CPU FAILED!");
		result = 0;
	}

	log("--- Test Camelia CTR 256 GPU starting");
	if(testCameliaCTR256GPU() == 1){
		log("--- Test Camelia CTR 256 GPU passed");
	}else{
		log("--- Test Camelia CTR 256 GPU FAILED!");
		result = 0;
	}
	return result;
}



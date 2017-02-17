#include <stdio.h>
#include <inttypes.h>
#include <stdlib.h>
#include "test_utils.h"
#include "test_clefia.h"
#include "../clefia_ctr/clefia_cipher.h"
#define CPU_DEVICE "CPU"
#define GPU_DEVICE "GPU"

static const unsigned char skey[32] = {
	0xffU,0xeeU,0xddU,0xccU,0xbbU,0xaaU,0x99U,0x88U,
	0x77U,0x66U,0x55U,0x44U,0x33U,0x22U,0x11U,0x00U,
	0xf0U,0xe0U,0xd0U,0xc0U,0xb0U,0xa0U,0x90U,0x80U,
	0x70U,0x60U,0x50U,0x40U,0x30U,0x20U,0x10U,0x00U
};
static const unsigned char pt[16] = {
	0x00U,0x01U,0x02U,0x03U,0x04U,0x05U,0x06U,0x07U,
	0x08U,0x09U,0x0aU,0x0bU,0x0cU,0x0dU,0x0eU,0x0fU
};

static unsigned char ct128[16] = {0xde, 0x2b, 0xf2, 0xfd, 0x9b, 0x74, 0xaa, 0xcd, 0xf1, 0x29, 0x85, 0x55, 0x45, 0x94, 0x94, 0xfd};
static unsigned char ct192[16] = {0xe2, 0x48, 0x2f, 0x64, 0x9f, 0x02, 0x8d, 0xc4, 0x80, 0xdd, 0xa1, 0x84, 0xfd, 0xe1, 0x81, 0xad};
static unsigned char ct256[16] = {0xa1, 0x39, 0x78, 0x14, 0x28, 0x9d, 0xe8, 0x0c, 0x10, 0xda, 0x46, 0xd1, 0xfa, 0x48, 0xb3, 0x8a};

static void writeOutputToFileUint64(char* outFileName, uint8_t* output, long lenght){
	FILE* fp = fopen(outFileName, "wb");
	if (!fp) {
		fprintf(stderr, "Failed to load file.\n");
		exit(1);
	}
	printf("\nwriting %d uint8_t..\n", lenght);
	for(int i = 0; i <lenght; i++){
		printf("%x", output[i]);	
	}
	fwrite(output, sizeof(uint8_t), lenght, fp);
	fclose(fp);
}

int testClefia128CPU(){
	int success = 1;
	char* fileName = "clefTest";
	unsigned char dst[16];
	writeOutputToFileUint64(fileName,pt,16);
	clefia_128_Encrypt(fileName, skey, dst, 1,CPU_DEVICE);
	if (memcmp(ct128, dst, sizeof(ct128)) != 0) {
		success = 0;
	}
	remove(fileName);
	return success;
}

int testClefia128GPU(){
	int success = 1;
	char* fileName = "clefTest";
	unsigned char dst[16];
	writeOutputToFileUint64(fileName,pt,16);
	clefia_128_Encrypt(fileName, skey, dst, 1,GPU_DEVICE);
	if (memcmp(ct128, dst, sizeof(ct128)) != 0) {
		success = 0;
	}
	remove(fileName);
	return success;
}


int testClefia192CPU(){
	int success = 1;
	char* fileName = "clefTest";
	unsigned char dst[16];
	writeOutputToFileUint64(fileName,pt,16);
	clefia_192_Encrypt(fileName, skey, dst, 1,CPU_DEVICE);
	if (memcmp(ct192, dst, sizeof(ct192)) != 0) {
		success = 0;
	}
	remove(fileName);
	return success;
}

int testClefia192GPU(){
	int success = 1;
	char* fileName = "clefTest";
	unsigned char dst[16];
	writeOutputToFileUint64(fileName,pt,16);
	clefia_192_Encrypt(fileName, skey, dst, 1,GPU_DEVICE);
	if (memcmp(ct192, dst, sizeof(ct192)) != 0) {
		success = 0;
	}
	remove(fileName);
	return success;
}

int testClefia256CPU(){
	int success = 1;
	char* fileName = "clefTest";
	unsigned char dst[16];
	writeOutputToFileUint64(fileName,pt,16);
	clefia_256_Encrypt(fileName, skey, dst, 1,CPU_DEVICE);
	if (memcmp(ct256, dst, sizeof(ct256)) != 0) {
		success = 0;
	}
	remove(fileName);
	return success;
}

int testClefia256GPU(){
	int success = 1;
	char* fileName = "clefTest";
	unsigned char dst[16];
	writeOutputToFileUint64(fileName,pt,16);
	clefia_256_Encrypt(fileName, skey, dst, 1,GPU_DEVICE);
	if (memcmp(ct256, dst, sizeof(ct256)) != 0) {
		success = 0;
	}
	remove(fileName);
	return success;
}


int testClefia128CTRCPU(){
	int success = 1;
	char* fileName = "clefTest";
	char* fileNameOut = "clefTestOut";
	unsigned char dst[16];
	unsigned char dstC[16];
	writeOutputToFileUint64(fileName,pt,16);
	clefia_128_CtrEncrypt(fileName, skey, dst, 1,CPU_DEVICE);
	writeOutputToFileUint64(fileNameOut,dst,16);
	clefia_128_CtrDecrypt(fileNameOut, skey, dstC, 1,CPU_DEVICE);

	if (memcmp(pt, dstC, sizeof(pt)) != 0) {
		success = 0;
	}
	remove(fileName);
	remove(fileNameOut);
	return success;
}

int testClefia128CTRGPU(){
	int success = 1;
	char* fileName = "clefTest";
	char* fileNameOut = "clefTestOut";
	unsigned char dst[16];
	unsigned char dstC[16];
	writeOutputToFileUint64(fileName,pt,16);
	clefia_128_CtrEncrypt(fileName, skey, dst, 1,GPU_DEVICE);
	writeOutputToFileUint64(fileNameOut,dst,16);
	clefia_128_CtrDecrypt(fileNameOut, skey, dstC, 1,GPU_DEVICE);

	if (memcmp(pt, dstC, sizeof(pt)) != 0) {
		success = 0;
	}
	remove(fileName);
	remove(fileNameOut);
	return success;
}


int testClefiaAll(){
	int result = 1;
	log("--- --- Starting Clefia tests");
	
	// log("--- Test Clefia 128 CPU starting");
	// if(testClefia128CPU() == 1){
	// 	log("--- Test Clefia 128 CPU passed");
	// }else{
	// 	log("--- Test Clefia 128 CPU FAILED!");
	// 	result = 0;
	// }

	// log("--- Test Clefia 128 GPU starting");
	// if(testClefia128CPU() == 1){
	// 	log("--- Test Clefia 128 GPU passed");
	// }else{
	// 	log("--- Test Clefia 128 GPU FAILED!");
	// 	result = 0;
	// }

	// log("--- Test Clefia 192 CPU starting");
	// if(testClefia192CPU() == 1){
	// 	log("--- Test Clefia 192 CPU passed");
	// }else{
	// 	log("--- Test Clefia 192 CPU FAILED!");
	// 	result = 0;
	// }

	// log("--- Test Clefia 192 GPU starting");
	// if(testClefia192CPU() == 1){
	// 	log("--- Test Clefia 192 GPU passed");
	// }else{
	// 	log("--- Test Clefia 192 GPU FAILED!");
	// 	result = 0;
	// }

	// log("--- Test Clefia 256 CPU starting");
	// if(testClefia256CPU() == 1){
	// 	log("--- Test Clefia 256 CPU passed");
	// }else{
	// 	log("--- Test Clefia 256 CPU FAILED!");
	// 	result = 0;
	// }

	// log("--- Test Clefia 256 GPU starting");
	// if(testClefia256CPU() == 1){
	// 	log("--- Test Clefia 256 GPU passed");
	// }else{
	// 	log("--- Test Clefia 256 GPU FAILED!");
	// 	result = 0;
	// }

	// log("--- Test Clefia 128 CTR CPU starting");
	// if(testClefia128CTRCPU() == 1){
	// 	log("--- Test Clefia 128 CTR CPU passed");
	// }else{
	// 	log("--- Test Clefia 128 CTR CPU FAILED!");
	// 	result = 0;
	// }

	log("--- Test Clefia 128 CTR GPU starting");
	if(testClefia128CTRGPU() == 1){
		log("--- Test Clefia 128 CTR GPU passed");
	}else{
		log("--- Test Clefia 128 CTR GPU FAILED!");
		result = 0;
	}
}

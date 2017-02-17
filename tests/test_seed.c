#define DEBUG_LOG 1
#include <stdio.h>
#include <inttypes.h>
#include <stdlib.h>

#include "test_utils.h"

#include "test_seed.h"

#include "../cipher_utils.h"

#define MAX_LOCAL_SIZE 64

#define CPU_DEVICE "CPU"
#define GPU_DEVICE "GPU"

static uint32_t SeedKey[4][4] = {
  {0x00000000, 0x00000000, 0x00000000, 0x00000000},
  {0x00010203, 0x04050607, 0x08090A0B, 0x0C0D0E0F},
  {0x47064808, 0x51E61BE8, 0x5D74BFB3, 0xFD956185},
  {0x28DBC3BC, 0x49FFD87D, 0xCFA509B1, 0x1D422BE7}
};

uint64_t SeedPlaintext[4][2] = {
  {0x0001020304050607ul, 0x08090A0B0C0D0E0Ful},
  {0x0000000000000000ul, 0x0000000000000000ul},
  {0x83A2F8A288641FB9ul, 0xA4E9A5CC2F131C7Dul},
  {0xB41E6BE2EBA84A14ul, 0x8E2EED84593C5EC7ul}
};

uint64_t SeedCiphertext[4][2] = {
  {0x5EBAC6E0054E1668ul, 0x19AFF1CC6D346CDBul},
  {0xC11F22F201405050ul, 0x84483597E4370F43ul},
  {0xEE54D13EBCAE706Dul, 0x226BC3142CD40D4Aul},
  {0x9B9B7BFCD1813CB9ul, 0x5D0B3618F40F5122ul}
};

// to save ciphertext to file (uint64_t)
static void writeOutputToFileUint64(char* outFileName, uint64_t* output, long lenght){
	FILE* fp = fopen(outFileName, "wb");
	if (!fp) {
		fprintf(stderr, "Failed to load file.\n");
		exit(1);
	}
	printf("\nwriting %d uint64_t..\n", lenght);
	for(int i = 0; i < 2; i++){
		printf("%016llx", output[i]);	
	}
	fwrite(output, sizeof(uint64_t), lenght, fp);
	fclose(fp);
}

// TEST FOR SEED OLD

int testSeedOldCPU(){
	int success = 1;
	uint64_t* seedCiphertext = (uint64_t*)malloc(2*sizeof(uint64_t));
	writeOutputToFileUint64("tests/seed_plaintext", SeedPlaintext[3], 2);
	seed_old_Encrypt("tests/seed_plaintext", SeedKey[3], seedCiphertext, 1, CPU_DEVICE);
	for(int i = 0; i < 2; i++){
			printf("\n%016llx", seedCiphertext[i]);
	}
	if (seedCiphertext[0] != SeedCiphertext[3][0] || seedCiphertext[1] != SeedCiphertext[3][1]) {
		success = 0;
	}
	free(seedCiphertext);
	remove("tests/seed_plaintext");
	return success;
}

int testSeedOldGPU(){
	int success = 1;
	uint64_t* seedCiphertext = (uint64_t*)malloc(2*sizeof(uint64_t));
	writeOutputToFileUint64("tests/seed_plaintext", SeedPlaintext[3], 2);
	seed_old_Encrypt("tests/seed_plaintext", SeedKey[3], seedCiphertext, 1, GPU_DEVICE);
	for(int i = 0; i < 2; i++){
			printf("\n%016llx", seedCiphertext[i]);
	}
	if (seedCiphertext[0] != SeedCiphertext[3][0] || seedCiphertext[1] != SeedCiphertext[3][1]) {
		success = 0;
	}
	free(seedCiphertext);
	remove("tests/seed_plaintext");
	return success;
}

// TEST FOR SEED 

int testSeedCPU(){
	int success = 1;
	uint64_t* seedCiphertext = (uint64_t*)malloc(2*sizeof(uint64_t));
	writeOutputToFileUint64("tests/seed_plaintext", SeedPlaintext[3], 2);
	seed_Encrypt("tests/seed_plaintext", SeedKey[3], seedCiphertext, 1, CPU_DEVICE);
	for(int i = 0; i < 2; i++){
			printf("\n%016llx", seedCiphertext[i]);
	}
	if (seedCiphertext[0] != SeedCiphertext[3][0] || seedCiphertext[1] != SeedCiphertext[3][1]) {
		success = 0;
	}
	free(seedCiphertext);
	remove("tests/seed_plaintext");
	return success;
}

int testSeedGPU(){
	int success = 1;
	uint64_t* seedCiphertext = (uint64_t*)malloc(2*sizeof(uint64_t));
	writeOutputToFileUint64("tests/seed_plaintext", SeedPlaintext[3], 2);
	seed_Encrypt("tests/seed_plaintext", SeedKey[3], seedCiphertext, 1, GPU_DEVICE);
	for(int i = 0; i < 2; i++){
			printf("\n%016llx", seedCiphertext[i]);
	}
	if (seedCiphertext[0] != SeedCiphertext[3][0] || seedCiphertext[1] != SeedCiphertext[3][1]) {
		success = 0;
	}
	free(seedCiphertext);
	remove("tests/seed_plaintext");
	return success;
}

// TEST FOR SEED OLD CTR

int testSeedOldCtrCPU(){
	int success = 1;
	struct FileInfo64 fileInfo = getFileUint64("tests/ctr_test");
	long dim = fileInfo.lenght;
	uint64_t* seedCiphertext = (uint64_t*)malloc(dim*sizeof(uint64_t));
	uint64_t* seedPlaintext = (uint64_t*)malloc(dim*sizeof(uint64_t));

	seed_old_CtrEncrypt("tests/ctr_test", SeedKey[0], seedCiphertext, 1, CPU_DEVICE);
	writeOutputToFileUint64("tests/seed_ciphertext", seedCiphertext, dim);
	seed_old_CtrDecrypt("tests/seed_ciphertext", SeedKey[0], seedPlaintext, 1, CPU_DEVICE);

	if(memcmp(seedPlaintext, fileInfo.filePointer, fileInfo.lenght) != 0){
		success = 0;
	}

	free(seedCiphertext);
	free(seedPlaintext);
	free(fileInfo.filePointer);
	remove("tests/seed_ciphertext");
	return success;
}

int testSeedOldCtrGPU(){
	int success = 1;
	struct FileInfo64 fileInfo = getFileUint64("tests/ctr_test");
	long dim = fileInfo.lenght;
	uint64_t* seedCiphertext = (uint64_t*)malloc(dim*sizeof(uint64_t));
	uint64_t* seedPlaintext = (uint64_t*)malloc(dim*sizeof(uint64_t));

	seed_old_CtrEncrypt("tests/ctr_test", SeedKey[0], seedCiphertext, 1, GPU_DEVICE);
	writeOutputToFileUint64("tests/seed_ciphertext", seedCiphertext, dim);
	seed_old_CtrDecrypt("tests/seed_ciphertext", SeedKey[0], seedPlaintext, 1, GPU_DEVICE);

	if(memcmp(seedPlaintext, fileInfo.filePointer, fileInfo.lenght) != 0){
		success = 0;
	}

	free(seedCiphertext);
	free(seedPlaintext);
	free(fileInfo.filePointer);
	remove("tests/seed_ciphertext");
	return success;
}

// TEST FOR SEED CTR

int testSeedCtrCPU(){
	int success = 1;
	struct FileInfo64 fileInfo = getFileUint64("tests/ctr_test");
	long dim = fileInfo.lenght;
	uint64_t* seedCiphertext = (uint64_t*)malloc(dim*sizeof(uint64_t));
	uint64_t* seedPlaintext = (uint64_t*)malloc(dim*sizeof(uint64_t));

	seed_CtrEncrypt("tests/ctr_test", SeedKey[0], seedCiphertext, 1, CPU_DEVICE);
	writeOutputToFileUint64("tests/seed_ciphertext", seedCiphertext, dim);
	seed_CtrDecrypt("tests/seed_ciphertext", SeedKey[0], seedPlaintext, 1, CPU_DEVICE);

	if(memcmp(seedPlaintext, fileInfo.filePointer, fileInfo.lenght) != 0){
		success = 0;
	}

	free(seedCiphertext);
	free(seedPlaintext);
	free(fileInfo.filePointer);
	remove("tests/seed_ciphertext");
	return success;
}

int testSeedCtrGPU(){
	int success = 1;
	struct FileInfo64 fileInfo = getFileUint64("tests/ctr_test");
	long dim = fileInfo.lenght;
	uint64_t* seedCiphertext = (uint64_t*)malloc(dim*sizeof(uint64_t));
	uint64_t* seedPlaintext = (uint64_t*)malloc(dim*sizeof(uint64_t));

	seed_CtrEncrypt("tests/ctr_test", SeedKey[0], seedCiphertext, 1, GPU_DEVICE);
	writeOutputToFileUint64("tests/seed_ciphertext", seedCiphertext, dim);
	seed_CtrDecrypt("tests/seed_ciphertext", SeedKey[0], seedPlaintext, 1, GPU_DEVICE);

	if(memcmp(seedPlaintext, fileInfo.filePointer, fileInfo.lenght) != 0){
		success = 0;
	}

	free(seedCiphertext);
	free(seedPlaintext);
	free(fileInfo.filePointer);
	remove("tests/seed_ciphertext");
	return success;
}

int testSeedOld(){
	int result = 1;
	log("\n--- --- Starting SEED OLD tests");
	
	log("\n\n--- Test SEED OLD CPU starting");
	if(testSeedOldCPU() == 1){
		log("--- Test SEED OLD CPU passed");
	}else{
		log("--- Test SEED OLD CPU FAILED!");
		result = 0;
	}

	log("\n\n--- Test SEED OLD GPU starting");
	if(testSeedOldGPU() == 1){
		log("--- Test SEED OLD GPU passed");
	}else{
		log("--- Test SEED OLD GPU FAILED!");
		result = 0;
	}

	log("\n\n--- Test SEED OLD CTR CPU starting");
	if(testSeedOldCtrCPU() == 1){
		log("--- Test SEED OLD CTR CPU passed");
	}else{
		log("--- Test SEED OLD CTR CPU FAILED!");
		result = 0;
	}

	log("\n\n--- Test SEED OLD CTR GPU starting");
	if(testSeedOldCtrGPU() == 1){
		log("--- Test SEED OLD CTR GPU passed");
	}else{
		log("--- Test SEED OLD CTR GPU FAILED!");
		result = 0;
	}

	if(result != 0){
		log("\n--- --- All SEED OLD test passed");
	}else{
		log("\n--- --- Some SEED OLD Test FAILED");
	}
	return result;
}

int testSeed(){
	int result = 1;
	log("\n--- --- Starting SEED tests");
	
	log("\n\n--- Test SEED CPU starting");
	if(testSeedCPU() == 1){
		log("--- Test SEED CPU passed");
	}else{
		log("--- Test SEED CPU FAILED!");
		result = 0;
	}

	log("\n\n--- Test SEED GPU starting");
	if(testSeedGPU() == 1){
		log("--- Test SEED GPU passed");
	}else{
		log("--- Test SEED GPU FAILED!");
		result = 0;
	}

	log("\n\n--- Test SEED CTR CPU starting");
	if(testSeedCtrCPU() == 1){
		log("--- Test SEED CTR CPU passed");
	}else{
		log("--- Test SEED CTR CPU FAILED!");
		result = 0;
	}

	log("\n\n--- Test SEED CTR GPU starting");
	if(testSeedCtrGPU() == 1){
		log("--- Test SEED CTR GPU passed");
	}else{
		log("--- Test SEED CTR GPU FAILED!");
		result = 0;
	}

	if(result != 0){
		log("\n--- --- All SEED test passed");
	}else{
		log("\n--- --- Some SEED Test FAILED");
	}
	return result;
}

int testSeedAll(){

	int tSeedOld = testSeedOld();
	int tSeed = testSeed();	
	int result = tSeedOld&&tSeed;
	if(result){
		log("\n--- --- --- ALL SEED TEST PASSED");
	}else{
		log("\n--- --- --- TEST SEED FAILED");
	}
	return result;

}
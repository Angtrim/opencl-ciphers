
#include <stdio.h>
#include <inttypes.h>
#include <stdlib.h>

#include "test_utils.h"

#include "test_des.h"



uint8_t DES3_keys[24] = {0x2B, 0xD6, 0x45, 0x9F, 0x82, 0xC5, 0xB3, 0x00, 0x95, 0x2C, 0x49, 0x10, 0x48, 0x81, 0xFF, 0x48, 0x2B, 0xD6, 0x45, 0x9F, 0x82, 0xC5, 0xB3, 0x00};

uint8_t DES3_init[8] = {0xEA, 0x02, 0x47, 0x14, 0xAD, 0x5C, 0x4D, 0x84};

uint8_t DES3_enc_test[3][8] = {
    {0x12, 0x6E, 0xFE, 0x8E, 0xD3, 0x12, 0x19, 0x0A},
    {0xC6, 0x16, 0xAC, 0xE8, 0x43, 0x95, 0x82, 0x47},
    {0xC6, 0x16, 0xAC, 0xE8, 0x43, 0x95, 0x82, 0x47}};

#define CPU_DEVICE "CPU"
#define GPU_DEVICE "GPU"

// to save ciphertext to file (uint8_t)
static void writeOutputToFile(char* outFileName,uint8_t* output, long lenght){
	FILE* fp = fopen(outFileName, "wb");
	if (!fp) {
		fprintf(stderr, "Failed to load file.\n");
		exit(1);
	}
	fwrite(output, sizeof(char), lenght, fp);
	fclose(fp);
}

int testDesCPU(){
  	int success = 1;
    uint8_t DES3_out[8];
    char* fileName = "desTest";
    writeOutputToFile(fileName,DES3_init,8);
    desEncrypt(fileName, DES3_keys, DES3_out ,1,CPU_DEVICE);
    if (memcmp(DES3_out, DES3_enc_test[0], 8) != 0) {
      success = 0;
    }
    remove(fileName);
    return success;
}

int testDesGPU(){
  	int success = 1;
    uint8_t DES3_out[8];
    char* fileName = "desTest";
    writeOutputToFile(fileName,DES3_init,8);
    desEncrypt(fileName, DES3_keys, DES3_out ,1,GPU_DEVICE);
    if (memcmp(DES3_out, DES3_enc_test[0], 8) != 0) {
      success = 0;
    }
    remove(fileName);
    return success;
}

int testDes2CPU(){
  	int success = 1;
    uint8_t DES3_out[8];
    char* fileName = "desTest";
    writeOutputToFile(fileName,DES3_init,8);
    des2Encrypt(fileName, DES3_keys, DES3_out ,1,CPU_DEVICE);
    if (memcmp(DES3_out, DES3_enc_test[1], 8) != 0) {
      success = 0;
    }
    remove(fileName); 
    return success;
}

int testDes2GPU(){
  	int success = 1;
    uint8_t DES3_out[8];
    char* fileName = "desTest";
    writeOutputToFile(fileName,DES3_init,8);
    des2Encrypt(fileName, DES3_keys, DES3_out ,1,GPU_DEVICE);
    if (memcmp(DES3_out, DES3_enc_test[1], 8) != 0) {
      success = 0;
    }
    remove(fileName); 
    return success;
}

int testDes3CPU(){
  	int success = 1;
    uint8_t DES3_out[8];
    char* fileName = "desTest";
    writeOutputToFile(fileName,DES3_init,8);
    des3Encrypt(fileName, DES3_keys, DES3_out ,1,CPU_DEVICE);
    if (memcmp(DES3_out, DES3_enc_test[2], 8) != 0) {
      success = 0;
    }
    remove(fileName); 
    return success;
}

int testDes3GPU(){
  	int success = 1;
    uint8_t DES3_out[8];
    char* fileName = "desTest";
    writeOutputToFile(fileName,DES3_init,8);
    des3Encrypt(fileName, DES3_keys, DES3_out ,1,GPU_DEVICE);
    if (memcmp(DES3_out, DES3_enc_test[2], 8) != 0) {
      success = 0;
    }
    remove(fileName); 
    return success;
}


int testDesCTRCPU(){
  	int success = 1;
    uint8_t DES3_out[8];
    uint8_t DES3_out_dec[8];
    char* fileName = "desTest";
    char* fileNameOut = "desTestOut";
    writeOutputToFile(fileName,DES3_init,8);
    desCtrEncrypt(fileName, DES3_keys, DES3_out ,1,CPU_DEVICE);
    writeOutputToFile(fileNameOut,DES3_out,8);
    desCtrDecrypt(fileNameOut,DES3_keys,DES3_out_dec,1,CPU_DEVICE);
    if (memcmp(DES3_out_dec, DES3_init, 8) != 0) {
      success = 0;
    }
    remove(fileName);
    remove(fileNameOut);
    return success;
}

int testDesCTRGPU(){
  	int success = 1;
    uint8_t DES3_out[8];
    uint8_t DES3_out_dec[8];
    char* fileName = "desTest";
    char* fileNameOut = "desTestOut";
    writeOutputToFile(fileName,DES3_init,8);
    desCtrEncrypt(fileName, DES3_keys, DES3_out ,1,GPU_DEVICE);
    writeOutputToFile(fileNameOut,DES3_out,8);
    desCtrDecrypt(fileNameOut,DES3_keys,DES3_out_dec,1,GPU_DEVICE);
    if (memcmp(DES3_out_dec, DES3_init, 8) != 0) {
      success = 0;
    }
    remove(fileName);
    remove(fileNameOut);
    return success;
}

int testDes2CTRCPU(){
  	int success = 1;
    uint8_t DES3_out[8];
    uint8_t DES3_out_dec[8];
    char* fileName = "desTest";
    char* fileNameOut = "desTestOut";
    writeOutputToFile(fileName,DES3_init,8);
    des2CtrEncrypt(fileName, DES3_keys, DES3_out ,1,CPU_DEVICE);
    writeOutputToFile(fileNameOut,DES3_out,8);
    des2CtrDecrypt(fileNameOut,DES3_keys,DES3_out_dec,1,CPU_DEVICE);
    if (memcmp(DES3_out_dec, DES3_init, 8) != 0) {
      success = 0;
    }
    remove(fileName);
    remove(fileNameOut);
    return success;
}

int testDes2CTRGPU(){
  	int success = 1;
    uint8_t DES3_out[8];
    uint8_t DES3_out_dec[8];
    char* fileName = "desTest";
    char* fileNameOut = "desTestOut";
    writeOutputToFile(fileName,DES3_init,8);
    des2CtrEncrypt(fileName, DES3_keys, DES3_out ,1,GPU_DEVICE);
    writeOutputToFile(fileNameOut,DES3_out,8);
    des2CtrDecrypt(fileNameOut,DES3_keys,DES3_out_dec,1,GPU_DEVICE);
    if (memcmp(DES3_out_dec, DES3_init, 8) != 0) {
      success = 0;
    }
    remove(fileName);
    remove(fileNameOut);
    return success;
}

int testDes3CTRCPU(){
  	int success = 1;
    uint8_t DES3_out[8];
    uint8_t DES3_out_dec[8];
    char* fileName = "desTest";
    char* fileNameOut = "desTestOut";
    writeOutputToFile(fileName,DES3_init,8);
    des3CtrEncrypt(fileName, DES3_keys, DES3_out ,1,CPU_DEVICE);
    writeOutputToFile(fileNameOut,DES3_out,8);
    des3CtrDecrypt(fileNameOut,DES3_keys,DES3_out_dec,1,CPU_DEVICE);
    if (memcmp(DES3_out_dec, DES3_init, 8) != 0) {
      success = 0;
    }
    remove(fileName);
    remove(fileNameOut);
    return success;
}

int testDes3CTRGPU(){
  	int success = 1;
    uint8_t DES3_out[8];
    uint8_t DES3_out_dec[8];
    char* fileName = "desTest";
    char* fileNameOut = "desTestOut";
    writeOutputToFile(fileName,DES3_init,8);
    des3CtrEncrypt(fileName, DES3_keys, DES3_out ,1,GPU_DEVICE);
    writeOutputToFile(fileNameOut,DES3_out,8);
    des3CtrDecrypt(fileNameOut,DES3_keys,DES3_out_dec,1,GPU_DEVICE);
    if (memcmp(DES3_out_dec, DES3_init, 8) != 0) {
      success = 0;
    }
    remove(fileName);
    remove(fileNameOut);
    return success;
}

int testDes(){
    int result = 1;
    log("--- --- Starting DES tests");
    
    log("--- Test DES CPU starting");
    if(testDesCPU() == 1){
        log("--- Test DES CPU passed");
    }else{
        log("--- Test DES CPU FAILED!");
        result = 0;
    }

    log("--- Test DES GPU starting");
    if(testDesGPU() == 1){
        log("--- Test DES GPU passed");
    }else{
        log("--- Test DES GPU FAILED!");
        result = 0;
    }

    log("--- Test DES CTR CPU starting");
    if(testDesCTRCPU() == 1){
        log("--- Test DES CTR CPU passed");
    }else{
        log("--- Test DES CTR CPU FAILED!");
        result = 0;
    }

    log("--- Test DES CTR GPU starting");
    if(testDesCTRGPU() == 1){
        log("--- Test DES CTR GPU passed");
    }else{
        log("--- Test DES CTR GPU FAILED!");
        result = 0;
    }

    if(result != 0){
        log("--- --- All DES test passed");
    }else{
        log("--- --- Some DES Test FAILED");
    }

    return result;
}

int testDes2(){
    int result = 1;

    log("--- Test DES2 CPU starting");
    if(testDes2CPU() == 1){
        log("--- Test DES2 CPU passed");
    }else{
        log("--- Test DES2 CPU FAILED!");
        result = 0;
    }

    log("--- Test DES2 GPU starting");
    if(testDes2GPU() == 1){
        log("--- Test DES2 GPU passed");
    }else{
        log("--- Test DES2 GPU FAILED!");
        result = 0;
    }

    log("--- Test DES2 CTR CPU starting");
    if(testDes2CTRCPU() == 1){
        log("--- Test DES2 CTR CPU passed");
    }else{
        log("--- Test DES2 CTR CPU FAILED!");
        result = 0;
    }

    log("--- Test DES2 CTR GPU starting");
    if(testDes2CTRGPU() == 1){
        log("--- Test DES2 CTR GPU passed");
    }else{
        log("--- Test DES2 CTR GPU FAILED!");
        result = 0;
    }

    if(result != 0){
        log("--- --- All DES2 test passed");
    }else{
        log("--- --- Some DES2 Test FAILED");
    }

    return result;
}

int testDes3(){
    int result = 1;

    log("--- Test DES3 CPU starting");
    if(testDes3CPU() == 1){
        log("--- Test DES3 CPU passed");
    }else{
        log("--- Test DES3 CPU FAILED!");
        result = 0;
    }

    log("--- Test DES3 GPU starting");
    if(testDes3GPU() == 1){
        log("--- Test DES3 GPU passed");
    }else{
        log("--- Test DES3 GPU FAILED!");
        result = 0;
    }

    log("--- Test DES3 CTR CPU starting");
    if(testDes3CTRCPU() == 1){
        log("--- Test DES3 CTR CPU passed");
    }else{
        log("--- Test DES3 CTR CPU FAILED!");
        result = 0;
    }

    log("--- Test DES3 CTR GPU starting");
    if(testDes3CTRGPU() == 1){
        log("--- Test DES3 CTR GPU passed");
    }else{
        log("--- Test DES3 CTR GPU FAILED!");
        result = 0;
    }

    if(result != 0){
        log("--- --- All DES3 test passed");
    }else{
        log("--- --- Some DES3 Test FAILED");
    }

    return result;
}


int testDESAll(){

	int tDES = testDes();
    int tDES2 = testDes2();
    int tDES3 = testDes3();   
    int result = tDES&&tDES2&&tDES3;
    if(result){
        log("--- --- --- ALL DES TEST PASSED");
    }else{
        log("--- --- --- TEST DES FAILED");
    }
    return result;
	 
}


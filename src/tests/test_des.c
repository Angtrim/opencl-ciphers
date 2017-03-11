
#include <stdio.h>
#include <inttypes.h>
#include <stdlib.h>

#include "test_utils.h"

#include "test_des.h"
#include "../des_ctr/des_cipher.h"


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

int testDes(cl_device_id* device_id){
  	int success = 1;
    uint8_t DES3_out[8];
    char* fileName = "desTest";
    writeOutputToFile(fileName,DES3_init,8);
    desEncrypt(fileName, DES3_keys, DES3_out ,1,device_id);
    if (memcmp(DES3_out, DES3_enc_test[0], 8) != 0) {
      success = 0;
    }
    remove(fileName);
    return success;
}


int testDes2(cl_device_id* device_id){
  	int success = 1;
    uint8_t DES3_out[8];
    char* fileName = "desTest";
    writeOutputToFile(fileName,DES3_init,8);
    des2Encrypt(fileName, DES3_keys, DES3_out ,1, device_id);
    if (memcmp(DES3_out, DES3_enc_test[1], 8) != 0) {
      success = 0;
    }
    remove(fileName); 
    return success;
}


int testDes3(cl_device_id* device_id){
  	int success = 1;
    uint8_t DES3_out[8];
    char* fileName = "desTest";
    writeOutputToFile(fileName,DES3_init,8);
    des3Encrypt(fileName, DES3_keys, DES3_out ,1, device_id);
    if (memcmp(DES3_out, DES3_enc_test[2], 8) != 0) {
      success = 0;
    }
    remove(fileName); 
    return success;
}



int testDesCTR(cl_device_id* device_id){
  	int success = 1;
    uint8_t DES3_out[8];
    uint8_t DES3_out_dec[8];
    char* fileName = "desTest";
    char* fileNameOut = "desTestOut";
    writeOutputToFile(fileName,DES3_init,8);
    desCtrEncrypt(fileName, DES3_keys, DES3_out ,1, device_id);
    writeOutputToFile(fileNameOut,DES3_out,8);
    desCtrDecrypt(fileNameOut,DES3_keys,DES3_out_dec,1, device_id);
    if (memcmp(DES3_out_dec, DES3_init, 8) != 0) {
      success = 0;
    }
    remove(fileName);
    remove(fileNameOut);
    return success;
}


int testDes2CTR(cl_device_id* device_id){
  	int success = 1;
    uint8_t DES3_out[8];
    uint8_t DES3_out_dec[8];
    char* fileName = "desTest";
    char* fileNameOut = "desTestOut";
    writeOutputToFile(fileName,DES3_init,8);
    des2CtrEncrypt(fileName, DES3_keys, DES3_out ,1,device_id);
    writeOutputToFile(fileNameOut,DES3_out,8);
    des2CtrDecrypt(fileNameOut,DES3_keys,DES3_out_dec,1, device_id);
    if (memcmp(DES3_out_dec, DES3_init, 8) != 0) {
      success = 0;
    }
    remove(fileName);
    remove(fileNameOut);
    return success;
}



int testDes3CTR(cl_device_id* device_id){
  	int success = 1;
    uint8_t DES3_out[8];
    uint8_t DES3_out_dec[8];
    char* fileName = "desTest";
    char* fileNameOut = "desTestOut";
    writeOutputToFile(fileName,DES3_init,8);
    des3CtrEncrypt(fileName, DES3_keys, DES3_out ,1, device_id);
    writeOutputToFile(fileNameOut,DES3_out,8);
    des3CtrDecrypt(fileNameOut,DES3_keys,DES3_out_dec,1, device_id);
    if (memcmp(DES3_out_dec, DES3_init, 8) != 0) {
      success = 0;
    }
    remove(fileName);
    remove(fileNameOut);
    return success;
}



int testDesAll(cl_device_id* device_id){
    int result = 1;
    logIt("--- --- Starting DES tests");
    
    logIt("--- Test DES CPU starting");
    if(testDes( device_id) == 1){
        logIt("--- Test DES  passed");
    }else{
        logIt("--- Test DES  FAILED!");
        result = 0;
    }



    logIt("--- Test DES CTR  starting");
    if(testDesCTR( device_id) == 1){
        logIt("--- Test DES CTR  passed");
    }else{
        logIt("--- Test DES CTR  FAILED!");
        result = 0;
    }


    if(result != 0){
        logIt("--- --- All DES test passed");
    }else{
        logIt("--- --- Some DES Test FAILED");
    }

    return result;
}

int testDes2All(cl_device_id* device_id){
    int result = 1;

    logIt("--- Test DES2  starting");
    if(testDes2( device_id) == 1){
        logIt("--- Test DES2  passed");
    }else{
        logIt("--- Test DES2  FAILED!");
        result = 0;
    }



    logIt("--- Test DES2 CTR  starting");
    if(testDes2CTR( device_id) == 1){
        logIt("--- Test DES2 CTR  passed");
    }else{
        logIt("--- Test DES2 CTR  FAILED!");
        result = 0;
    }



    if(result != 0){
        logIt("--- --- All DES2 test passed");
    }else{
        logIt("--- --- Some DES2 Test FAILED");
    }

    return result;
}

int testDes3All(cl_device_id* device_id){
    int result = 1;

    logIt("--- Test DES3  starting");
    if(testDes3(device_id) == 1){
        logIt("--- Test DES3  passed");
    }else{
        logIt("--- Test DES3  FAILED!");
        result = 0;
    }



    logIt("--- Test DES3 CTR  starting");
    if(testDes3CTR(device_id) == 1){
        logIt("--- Test DES3 CTR  passed");
    }else{
        logIt("--- Test DES3 CTR  FAILED!");
        result = 0;
    }


    if(result != 0){
        logIt("--- --- All DES3 test passed");
    }else{
        logIt("--- --- Some DES3 Test FAILED");
    }

    return result;
}


int testDESAll(cl_device_id* device_id){

	int tDES = testDesAll(device_id);
    int tDES2 = testDes2All( device_id);
    int tDES3 = testDes3All( device_id);   
    int result = tDES&&tDES2&&tDES3;
    if(result){
        logIt("--- --- --- ALL DES TEST PASSED");
    }else{
        logIt("--- --- --- TEST DES FAILED");
    }
    return result;
	 
}


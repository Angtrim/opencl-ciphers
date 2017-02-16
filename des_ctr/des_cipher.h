
#ifndef DES_CIPHER_H
#define DES_CIPHER_H

#include "../cipher_utils.h"

#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "des_expansion.h"

#ifdef __APPLE__
#include <OpenCL/opencl.h>
#else
#include <CL/cl.h>
#endif
#define BLOCK_DIMEN 8

/** -- opencl parameters initialization to run the kernel -- **/
static cl_device_id device_id = NULL;
static cl_device_type device_type;
static cl_context context = NULL;
static cl_command_queue command_queue = NULL;
static cl_event event = NULL;
static cl_ulong time_start, time_end;

static cl_mem out = NULL;
static cl_mem in = NULL;
static cl_mem _esk = NULL;

static cl_program program = NULL;
static cl_kernel kernel = NULL;
static cl_platform_id platform_id = NULL;
static cl_uint ret_num_devices;
static cl_uint ret_num_platforms;
static cl_int ret;

//cl file parameters
static FILE *fp;
static char clFileName[] = "des_ctr/des_ctr.cl";

static char* source_str;

cl_event desCtrEncrypt(char* fileName, uint8_t* key, uint8_t* output,size_t local_item_size, char* deviceType);
cl_event des2CtrEncrypt(char* fileName, uint8_t* key, uint8_t* output,size_t local_item_size, char* deviceType);
cl_event des3CtrEncrypt(char* fileName, uint8_t* key, uint8_t* output,size_t local_item_size, char* deviceType);
cl_event desEncrypt(char* fileName, uint8_t* key, uint8_t* output,size_t local_item_size, char* deviceType);
cl_event des2Encrypt(char* fileName, uint8_t* key, uint8_t* output,size_t local_item_size, char* deviceType);
cl_event des3Encrypt(char* fileName, uint8_t* key, uint8_t* output,size_t local_item_size, char* deviceType);
cl_event desDecrypt(char* fileName, uint8_t* key, uint8_t* output,size_t local_item_size, char* deviceType);
cl_event des2Decrypt(char* fileName, uint8_t* key, uint8_t* output,size_t local_item_size, char* deviceType);
cl_event des3Decrypt(char* fileName, uint8_t* key, uint8_t* output,size_t local_item_size, char* deviceType);

#endif

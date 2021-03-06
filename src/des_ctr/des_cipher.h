
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
static cl_context context = NULL;
static cl_command_queue command_queue = NULL;
static cl_event event = NULL;

static cl_mem out = NULL;
static cl_mem in = NULL;
static cl_mem _esk = NULL;

static cl_program program = NULL;
static cl_kernel kernel = NULL;

static cl_int ret;

//cl file parameters
static FILE *fp;
static char clFileName[] = "src_cl/des_ctr.cl";

static char* source_str = NULL;
static size_t source_size;

cl_event desCtrEncrypt(char* fileName, uint8_t* key, uint8_t* output,size_t local_item_size, cl_device_id* device_id);
cl_event des2CtrEncrypt(char* fileName, uint8_t* key, uint8_t* output,size_t local_item_size, cl_device_id* device_id);
cl_event des3CtrEncrypt(char* fileName, uint8_t* key, uint8_t* output,size_t local_item_size, cl_device_id* device_id);
cl_event desEncrypt(char* fileName, uint8_t* key, uint8_t* output,size_t local_item_size, cl_device_id* device_id);
cl_event des2Encrypt(char* fileName, uint8_t* key, uint8_t* output,size_t local_item_size, cl_device_id* device_id);
cl_event des3Encrypt(char* fileName, uint8_t* key, uint8_t* output,size_t local_item_size, cl_device_id* device_id);
cl_event desCtrDecrypt(char* fileName, uint8_t* key, uint8_t* output,size_t local_item_size, cl_device_id* device_id);
cl_event des2CtrDecrypt(char* fileName, uint8_t* key, uint8_t* output,size_t local_item_size, cl_device_id* device_id);
cl_event des3CtrDecrypt(char* fileName, uint8_t* key, uint8_t* output,size_t local_item_size, cl_device_id* device_id);

#endif

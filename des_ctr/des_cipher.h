
#ifndef AES_CIPHER_H
#define AES_CIPHER_H

#include "../cipher_utils.h"

#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#ifdef __APPLE__
#include <OpenCL/opencl.h>
#else
#include <CL/cl.h>
#endif


/** -- opencl parameters initialization to run the kernel -- **/
static cl_device_id device_id = NULL;
static cl_context context = NULL;
static cl_command_queue command_queue = NULL;

static cl_mem out = NULL;
static cl_mem in = NULL;
static cl_mem exKey = NULL;

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

byte* desCtrEncrypt(char* fileName, word* key, char* output,size_t local_item_size);
byte* des3CtrEncrypt(char* fileName, word* key, char* output,size_t local_item_size);
byte* desEncrypt(char* fileName, word* key, char* output,size_t local_item_size);
byte* des3Encrypt(char* fileName, word* key, char* output,size_t local_item_size);

#endif

#ifndef MISTY1_CIPHER_H
#define MISTY1_CIPHER_H

#include "misty1_expansion.h"
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


#define BLOCK_SIZE (128 / 8)

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
static char clFileName[] = "misty1_ctr/misty1_ctr.cl";

static char* source_str;
byte* misty1Encrypt(char* fileName, uint8_t* key, char* outFileName,size_t local_item_size);
byte* misty1CtrEncrypt(char* fileName, uint8_t* key, char* outFileName,size_t local_item_size);

#endif

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

/** -- opencl parameters initialization to run the kernel -- **/
static cl_device_id device_id = NULL;
static cl_device_type device_type;
static cl_context context = NULL;
static cl_command_queue command_queue = NULL;
static cl_event event = NULL;
static cl_ulong time_start, time_end;

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
cl_event misty1Encrypt(char* fileName, uint8_t* key, uint64_t* output,size_t local_item_size, char* deviceType);
cl_event misty1CtrEncrypt(char* fileName, uint8_t* key, uint64_t* output,size_t local_item_size, char* deviceType);
cl_event misty1CtrDecrypt(char* fileName, uint8_t* key, uint64_t* output,size_t local_item_size, char* deviceType);

#endif

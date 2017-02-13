#ifndef CLEFIA_CIPHER_H
#define CLEFIA_CIPHER_H

#include "clefia_expansion.h"
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
static cl_mem rk = NULL;
static cl_mem r = NULL;

static cl_program program = NULL;
static cl_kernel kernel = NULL;
static cl_platform_id platform_id = NULL;
static cl_uint ret_num_devices;
static cl_uint ret_num_platforms;
static cl_int ret;

//cl file parameters
static FILE *fp;
static char clFileName[] = "clefia_ctr/clefia_ctr.cl";

static char* source_str;
uint8_t* clefia_128_Encrypt(char* fileName, uint8_t* key, char* outFileName, size_t local_item_size, char* deviceType);
uint8_t* clefia_192_Encrypt(char* fileName, uint8_t* key, char* outFileName, size_t local_item_size, char* deviceType);
uint8_t* clefia_256_Encrypt(char* fileName, uint8_t* key, char* outFileName, size_t local_item_size, char* deviceType);
uint8_t* clefia_128_CtrEncrypt(char* fileName, uint8_t* key, char* outFileName, size_t local_item_size, char* deviceType);
uint8_t* clefia_192_Ctrncrypt(char* fileName, uint8_t* key, char* outFileName, size_t local_item_size, char* deviceType);
uint8_t* clefia_256_CtrEncrypt(char* fileName, uint8_t* key, char* outFileName, size_t local_item_size, char* deviceType);

#endif
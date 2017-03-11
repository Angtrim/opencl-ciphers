#ifndef HIGHT_CIPHER_H
#define HIGHT_CIPHER_H

#include "hight_expansion.h"
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

static cl_context context = NULL;
static cl_command_queue command_queue = NULL;
static cl_event event = NULL;

static cl_mem out = NULL;
static cl_mem in = NULL;
static cl_mem wk = NULL;
static cl_mem sk = NULL;

static cl_program program = NULL;
static cl_kernel kernel = NULL;

static cl_int ret;


static char clFileName[] = "src_cl/hight_ctr.cl";

static char* source_str = NULL;
static size_t source_size;
cl_event hightEncrypt(char* fileName, uint8_t* key, uint64_t* output,size_t local_item_size, cl_device_id* device_id);
cl_event hightCtrEncrypt(char* fileName, uint8_t* key, uint64_t* output,size_t local_item_size, cl_device_id* device_id);
cl_event hightCtrDecrypt(char* fileName, uint8_t* key, uint64_t* output,size_t local_item_size, cl_device_id* device_id);

#endif

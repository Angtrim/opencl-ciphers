#ifndef PRESENT_CIPHER_H
#define PRESENT_CIPHER_H

#include "present_expansion.h"
#include "../cipher_utils.h"

#include <stdio.h>
#include <stdint.h>

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
static cl_mem _SK = NULL;

static cl_program program = NULL;
static cl_kernel kernel = NULL;

static cl_int ret;


static char clFileName[] = "src_cl/present_ctr.cl";

static char* source_str = NULL;
static size_t source_size;

cl_event present_memory_Encrypt(char* fileName, uint64_t* Key, uint64_t* output, size_t local_item_size, cl_device_id* device_id);
cl_event present_speed_Encrypt(char* fileName, uint64_t* Key, uint64_t* output, size_t local_item_size, cl_device_id* device_id);
cl_event present_memory_CtrEncrypt(char* fileName, uint64_t* Key, uint64_t* output, size_t local_item_size, cl_device_id* device_id);
cl_event present_speed_CtrEncrypt(char* fileName, uint64_t* Key, uint64_t* output, size_t local_item_size, cl_device_id* device_id);
cl_event present_memory_CtrDecrypt(char* fileName, uint64_t* Key, uint64_t* output, size_t local_item_size, cl_device_id* device_id);
cl_event present_speed_CtrDecrypt(char* fileName, uint64_t* Key, uint64_t* output, size_t local_item_size, cl_device_id* device_id);

#endif

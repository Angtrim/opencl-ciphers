#ifndef SEED_CIPHER_H
#define SEED_CIPHER_H

#include "seed_expansion.h"
#include "../cipher_utils.h"

#include <stdio.h>
#include <stdint.h>

#ifdef __APPLE__
#include <OpenCL/opencl.h>
#else
#include <CL/cl.h>
#endif


static cl_context context = NULL;
static cl_command_queue command_queue = NULL;
static cl_event event = NULL;

static cl_mem out = NULL;
static cl_mem in = NULL;
static cl_mem _Ki = NULL;

static cl_program program = NULL;
static cl_kernel kernel = NULL;

static cl_int ret;


static char clFileName[] = "src_cl/seed_ctr.cl";

static char* source_str = NULL;
static size_t source_size;

cl_event seed_old_Encrypt(char* fileName, uint32_t* Key, uint64_t* output, size_t local_item_size, cl_device_id* device_id);
cl_event seed_Encrypt(char* fileName, uint32_t* Key, uint64_t* output, size_t local_item_size, cl_device_id* device_id);
cl_event seed_old_CtrEncrypt(char* fileName, uint32_t* Key, uint64_t* output, size_t local_item_size, cl_device_id* device_id);
cl_event seed_CtrEncrypt(char* fileName, uint32_t* Key, uint64_t* output, size_t local_item_size, cl_device_id* device_id);
cl_event seed_old_CtrDecrypt(char* fileName, uint32_t* Key, uint64_t* output, size_t local_item_size, cl_device_id* device_id);
cl_event seed_CtrDecrypt(char* fileName, uint32_t* Key, uint64_t* output, size_t local_item_size, cl_device_id* device_id);

#endif

#ifndef CAMELLIA_CIPHER_H
#define CAMELLIA_CIPHER_H

#include "camellia_expansion.h"
#include "../cipher_utils.h"

#include <stdio.h>
#include <stdint.h>

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
static cl_mem _k = NULL;
static cl_mem _ke = NULL;
static cl_mem _kw = NULL;

static cl_program program = NULL;
static cl_kernel kernel = NULL;
static cl_platform_id platform_id = NULL;
static cl_uint ret_num_devices;
static cl_uint ret_num_platforms;
static cl_int ret;

//cl file parameters
static FILE *fp;
static char clFileName[] = "camellia_ctr/camellia_ctr.cl";

static char* source_str;

uint64_t* camellia128Encrypt(char* fileName, uint64_t* K, char* outFileName, size_t local_item_size);
uint64_t* camellia192Encrypt(char* fileName, uint64_t* K, char* outFileName, size_t local_item_size);
uint64_t* camellia256Encrypt(char* fileName, uint64_t* K, char* outFileName, size_t local_item_size);
uint64_t* camelliaCtr128Encrypt(char* fileName, uint64_t* K, char* outFileName, size_t local_item_size);
uint64_t* camelliaCtr192Encrypt(char* fileName, uint64_t* K, char* outFileName, size_t local_item_size);
uint64_t* camelliaCtr256Encrypt(char* fileName, uint64_t* K, char* outFileName, size_t local_item_size);

#endif

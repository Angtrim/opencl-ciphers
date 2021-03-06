#ifndef AES_CIPHER_H
#define AES_CIPHER_H

#include "aes_expansion.h"
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

#define Nk128 4
#define Nb128 4
#define Nr128 10

#define Nk192 6
#define Nb192 4
#define Nr192 12

#define Nk256 8
#define Nb256 4
#define Nr256 14
#define BLOCK_SIZE (128 / 8)

/** -- opencl parameters initialization to run the kernel -- **/

static cl_context context = NULL;
static cl_command_queue command_queue = NULL;
static cl_event event = NULL;
static cl_mem out = NULL;
static cl_mem in = NULL;
static cl_mem exKey = NULL;

static cl_program program = NULL;
static cl_kernel kernel = NULL;
static cl_int ret;

//cl file parameters
static FILE *fp;
static char clFileName[] = "src_cl/aes_ctr.cl";

static char* source_str = NULL;
static size_t source_size;

cl_event aes128CtrEncrypt(char* fileName, word* key, uint8_t* output,size_t local_item_size, cl_device_id* device_id);
cl_event aes192CtrEncrypt(char* fileName, word* key, uint8_t* output,size_t local_item_size, cl_device_id* device_id);
cl_event aes256CtrEncrypt(char* fileName, word* key, uint8_t* output,size_t local_item_size, cl_device_id* device_id);
cl_event aes128CtrDecrypt(char* fileName, word* key, uint8_t* output,size_t local_item_size, cl_device_id* device_id);
cl_event aes192CtrDecrypt(char* fileName, word* key, uint8_t* output,size_t local_item_size, cl_device_id* device_id);
cl_event aes256CtrDecrypt(char* fileName, word* key, uint8_t* output,size_t local_item_size, cl_device_id* device_id);
cl_event aes128Encrypt(char* fileName, word* key, uint8_t* output,size_t local_item_size, cl_device_id* device_id);
cl_event aes192Encrypt(char* fileName, word* key, uint8_t* output,size_t local_item_size, cl_device_id* device_id);
cl_event aes256Encrypt(char* fileName, word* key, uint8_t* output,size_t local_item_size, cl_device_id* device_id);

#endif

#ifndef CIPHER_UTILS_H
#define CIPHER_UTILS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <inttypes.h>
#ifdef __APPLE__
#include <OpenCL/opencl.h>
#else
#include <CL/cl.h>
#endif

typedef uint32_t word;
typedef uint8_t byte;

static struct FileInfo {
		byte* filePointer;
		long lenght;
}; 

static struct FileInfo64 {
		uint64_t* filePointer;
		long lenght;
}; 

struct FileInfo64 getFileUint64(char* filePath);

struct FileInfo getFileBytes(char* filePath);

long getByteLenght(char* filePath);
void loadClaProgramSource(char* fileName,char** source_str,size_t* source_size);
void setDeviceType(char* deviceType,cl_device_type* deviceTypeCl);
void logBuildError(cl_int* ret, cl_program* program, cl_device_id* deviceId);
void getSystemInfo();
void initClSetup(cl_device_id* device_id,cl_device_type* device_type,cl_context* context,cl_command_queue* command_queue);
#endif

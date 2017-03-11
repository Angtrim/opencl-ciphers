#ifndef BENCHMARKS_AES_H
#define BENCHMARKS_AES_H
#include "benchmarks_utils.h"
//includes for testing normal encryption


void benchAes128(int fileSize,int localSize, struct BenchInfo* benchInfo,cl_device_id* device_id);
void benchAes128Multiple(int fileSizes,int* localSize, int numOfLocalSizes, cl_device_id* device_id);
void benchAes192(int fileSize,int localSize, struct BenchInfo* benchInfo,cl_device_id* device_id);
void benchAes192Multiple(int fileSizes,int* localSize, int numOfLocalSizes, cl_device_id* device_id);
void benchAes256(int fileSize,int localSize, struct BenchInfo* benchInfo,cl_device_id* device_id);
void benchAes256Multiple(int fileSizes,int* localSize, int numOfLocalSizes, cl_device_id* device_id);
#endif

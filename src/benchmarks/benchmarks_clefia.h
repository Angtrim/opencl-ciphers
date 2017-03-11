#ifndef BENCHMARKS_CLEFIA_H
#define BENCHMARKS_CLEFIA_H
#include "benchmarks_utils.h"
//includes for testing normal encryption


void benchClefia128(int fileSize,int localSize, struct BenchInfo* benchInfo,cl_device_id* device_id);
void benchClefia128Multiple(int fileSizes,int* localSize, int numOfLocalSizes, cl_device_id* device_id);
void benchClefia192(int fileSize,int localSize, struct BenchInfo* benchInfo,cl_device_id* device_id);
void benchClefia192Multiple(int fileSizes,int* localSize, int numOfLocalSizes,cl_device_id* device_id);
void benchClefia256(int fileSize,int localSize, struct BenchInfo* benchInfo,cl_device_id* device_id);
void benchClefia256Multiple(int fileSizes,int* localSize, int numOfLocalSizes, cl_device_id* device_id);

#endif

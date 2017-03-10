#ifndef BENCHMARKS_DES_H
#define BENCHMARKS_DES_H
#include "benchmarks_utils.h"
//includes for testing normal encryption


void benchDes(int fileSize,int localSize,struct BenchInfo* benchInfo,cl_device_id* device_id);
void benchDesMultiple(int fileSizes,int* localSize, int numOfLocalSizes, cl_device_id* device_id);
void benchDes2(int fileSize,int localSize, struct BenchInfo* benchInfo,cl_device_id* device_id);
void benchDes2Multiple(int fileSizes,int* localSize, int numOfLocalSizes, cl_device_id* device_id);
void benchDes3(int fileSize,int localSize, struct BenchInfo* benchInfo,cl_device_id* device_id);
void benchDes3Multiple(int fileSizes,int* localSize, int numOfLocalSizes, cl_device_id* device_id);

#endif

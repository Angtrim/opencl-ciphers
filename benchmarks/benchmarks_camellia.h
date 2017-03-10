#ifndef BENCHMARKS_CAM_H
#define BENCHMARKS_CAM_H
#include "benchmarks_utils.h"
//includes for testing normal encryption


void benchCam128(int fileSize,int localSize,struct BenchInfo* benchInfo,cl_device_id* device_id);
void benchCam128Multiple(int fileSizes,int* localSize, int numOfLocalSizes, cl_device_id* device_id);
void benchCam192(int fileSize,int localSize, struct BenchInfo* benchInfo,cl_device_id* device_id);
void benchCam192Multiple(int fileSizes,int* localSize, int numOfLocalSizes, cl_device_id* device_id);
void benchCam256(int fileSize,int localSize,struct BenchInfo* benchInfo,cl_device_id* device_id);
void benchCam256Multiple(int fileSizes,int* localSize, int numOfLocalSizes, cl_device_id* device_id);

#endif

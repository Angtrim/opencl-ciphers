#ifndef BENCHMARKS_CAM_H
#define BENCHMARKS_CAM_H
#include "benchmarks_utils.h"
//includes for testing normal encryption


void benchCam128(int fileSize,int localSize,int onGPU, struct BenchInfo* benchInfo);
void benchCam128Multiple(int fileSizes,int* localSize, int numOfLocalSizes, int onGPU);
void benchCam192(int fileSize,int localSize,int onGPU, struct BenchInfo* benchInfo);
void benchCam192Multiple(int fileSizes,int* localSize, int numOfLocalSizes, int onGPU);
void benchCam256(int fileSize,int localSize,int onGPU, struct BenchInfo* benchInfo);
void benchCam256Multiple(int fileSizes,int* localSize, int numOfLocalSizes, int onGPU);

#endif

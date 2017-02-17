#ifndef BENCHMARKS_CLEFIA_H
#define BENCHMARKS_CLEFIA_H
#include "benchmarks_utils.h"
//includes for testing normal encryption


void benchClefia128(int fileSize,int localSize,int onGPU, struct BenchInfo* benchInfo);
void benchClefia128Multiple(int fileSizes,int* localSize, int numOfLocalSizes, int onGPU);
void benchClefia192(int fileSize,int localSize,int onGPU, struct BenchInfo* benchInfo);
void benchClefia192Multiple(int fileSizes,int* localSize, int numOfLocalSizes, int onGPU);
void benchClefia256(int fileSize,int localSize,int onGPU, struct BenchInfo* benchInfo);
void benchClefia256Multiple(int fileSizes,int* localSize, int numOfLocalSizes, int onGPU);

#endif

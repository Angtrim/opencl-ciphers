#ifndef BENCHMARKS_DES_H
#define BENCHMARKS_DES_H
#include "benchmarks_utils.h"
//includes for testing normal encryption


void benchDes(int fileSize,int localSize,int onGPU, struct BenchInfo* benchInfo);
void benchDesMultiple(int fileSizes,int* localSize, int numOfLocalSizes, int onGPU);
void benchDes2(int fileSize,int localSize,int onGPU, struct BenchInfo* benchInfo);
void benchDes2Multiple(int fileSizes,int* localSize, int numOfLocalSizes, int onGPU);
void benchDes3(int fileSize,int localSize,int onGPU, struct BenchInfo* benchInfo);
void benchDes3Multiple(int fileSizes,int* localSize, int numOfLocalSizes, int onGPU);

#endif

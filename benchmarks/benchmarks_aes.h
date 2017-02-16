#ifndef BENCHMARKS_AES_H
#define BENCHMARKS_AES_H
#include "benchmarks_utils.h"
//includes for testing normal encryption


void benchAes128(int fileSize,int localSize,int onGPU, struct BenchInfo* benchInfo);
void benchAes128Multiple(int fileSizes,int* localSize, int numOfLocalSizes, int onGPU);

#endif

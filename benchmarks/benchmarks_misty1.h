#ifndef BENCHMARKS_MISTY1_H
#define BENCHMARKS_MISTY1_H
#include "benchmarks_utils.h"


void benchMisty1Ctr(int fileSize,int localSize,int onGPU, struct BenchInfo* benchInfo);
void benchMisty1CtrMultiple(int fileSizes,int* localSize, int numOfLocalSizes, int onGPU);

#endif
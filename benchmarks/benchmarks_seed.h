#ifndef BENCHMARKS_SEED_H
#define BENCHMARKS_SEED_H
#include "benchmarks_utils.h"


void benchSeedOldCtr(int fileSize,int localSize,int onGPU, struct BenchInfo* benchInfo);
void benchSeedCtr(int fileSize,int localSize,int onGPU, struct BenchInfo* benchInfo);
void benchSeedOldCtrMultiple(int fileSizes,int* localSize, int numOfLocalSizes, int onGPU);
void benchSeedCtrMultiple(int fileSizes,int* localSize, int numOfLocalSizes, int onGPU);

#endif
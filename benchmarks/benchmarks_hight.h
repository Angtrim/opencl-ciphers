#ifndef BENCHMARKS_HIGHT_H
#define BENCHMARKS_HIGHT_H
#include "benchmarks_utils.h"
//includes for testing normal encryption


void benchHight(int fileSize,int localSize,int onGPU, struct BenchInfo* benchInfo);
void benchHightMultiple(int fileSizes,int* localSize, int numOfLocalSizes, int onGPU);

#endif
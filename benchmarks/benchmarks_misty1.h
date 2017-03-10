#ifndef BENCHMARKS_MISTY1_H
#define BENCHMARKS_MISTY1_H
#include "benchmarks_utils.h"


void benchMisty1Ctr(int fileSize,int localSize,struct BenchInfo* benchInfo,cl_device_id* device_id);
void benchMisty1CtrMultiple(int fileSizes,int* localSize, int numOfLocalSizes, cl_device_id* device_id);

#endif
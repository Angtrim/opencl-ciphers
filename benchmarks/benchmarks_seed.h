#ifndef BENCHMARKS_SEED_H
#define BENCHMARKS_SEED_H
#include "benchmarks_utils.h"


void benchSeedOldCtr(int fileSize,int localSize, struct BenchInfo* benchInfo,cl_device_id* device_id);
void benchSeedCtr(int fileSize,int localSize, struct BenchInfo* benchInfo,cl_device_id* device_id);
void benchSeedOldCtrMultiple(int fileSizes,int* localSize, int numOfLocalSizes, cl_device_id* device_id);
void benchSeedCtrMultiple(int fileSizes,int* localSize, int numOfLocalSizes, cl_device_id* device_id);

#endif
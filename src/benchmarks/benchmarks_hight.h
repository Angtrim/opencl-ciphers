#ifndef BENCHMARKS_HIGHT_H
#define BENCHMARKS_HIGHT_H
#include "benchmarks_utils.h"
//includes for testing normal encryption


void benchHight(int fileSize,int localSize, struct BenchInfo* benchInfo,cl_device_id* device_id);
void benchHightMultiple(int fileSizes,int* localSize, int numOfLocalSizes, cl_device_id* device_id);

#endif
#ifndef BENCHMARKS_PRESENT_H
#define BENCHMARKS_PRESENT_H
#include "benchmarks_utils.h"


void benchPresentMemoryCtr(int fileSize,int localSize, struct BenchInfo* benchInfo,cl_device_id* device_id);
void benchPresentSpeedCtr(int fileSize,int localSize, struct BenchInfo* benchInfo,cl_device_id* device_id);
void benchPresentMemoryMultiple(int fileSize,int* localSize, int numOfLocalSizes, cl_device_id* device_id);
void benchPresentSpeedMultiple(int fileSize,int* localSize, int numOfLocalSizes, cl_device_id* device_id);

#endif
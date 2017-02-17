#ifndef BENCHMARKS_PRESENT_H
#define BENCHMARKS_PRESENT_H
#include "benchmarks_utils.h"


void benchPresentMemoryCtr(int fileSize,int localSize,int onGPU, struct BenchInfo* benchInfo);
void benchPresentSpeedCtr(int fileSize,int localSize,int onGPU, struct BenchInfo* benchInfo);
void benchPresentMemoryMultiple(int fileSize,int* localSize, int numOfLocalSizes, int onGPU);
void benchPresentSpeedMultiple(int fileSize,int* localSize, int numOfLocalSizes, int onGPU);

#endif
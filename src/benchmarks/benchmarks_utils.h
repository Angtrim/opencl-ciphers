#ifndef BENCHMARKS_UTILS_H
#define BENCHMARKS_UTILS_H
#ifdef __APPLE__
#include <OpenCL/opencl.h>
#else
#include <CL/cl.h>
#endif


struct BenchInfo {
		long totalTime;
		int localSize;
		long fileSize;
}; 

void buildFileOfZeroes(char* outFileName,long lenght);
void saveDataToFile(char* nameCiph,struct BenchInfo* infos,int numInfos);


#endif

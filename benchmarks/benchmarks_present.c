#ifdef __APPLE__
#include <OpenCL/opencl.h>
#else
#include <CL/cl.h>
#endif
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "../seed_ctr/seed_cipher.h"
#include "benchmarks_present.h"

static uint64_t PresentKey[2] = {0, 0};

void benchPresentMemoryCtr(int fileSize,int localSize,int onGPU, struct BenchInfo* benchInfo){
	char* device;
	if(onGPU){
		device = "GPU";
	}else{
		device = "CPU";
	}
	// Pad file size
	fileSize = fileSize + (fileSize%8);
	char* fileName = "benchmarks/benchPresent";
	buildFileOfZeroes(fileName,fileSize);
	uint64_t* presentCiphertext = (uint64_t*)malloc((fileSize/8)*sizeof(uint64_t));
	cl_event event = NULL;
	cl_ulong time_start, time_end;
	event = present_memory_CtrEncrypt(fileName, PresentKey, presentCiphertext, localSize, device);
	/* compute execution time */
	double total_time;
	clGetEventProfilingInfo(event, CL_PROFILING_COMMAND_START, sizeof(time_start), &time_start, NULL);
	clGetEventProfilingInfo(event, CL_PROFILING_COMMAND_END, sizeof(time_end), &time_end, NULL);
	total_time = time_end-time_start;
	printf("PRESENT MEMORY CTR execution time is: %0.3f ms\n",total_time/1000000.0);
	free(presentCiphertext);
	remove(fileName);
	benchInfo->totalTime = total_time;
	benchInfo->localSize = localSize;
	benchInfo->fileSize = fileSize;
}

void benchPresentSpeedCtr(int fileSize,int localSize,int onGPU, struct BenchInfo* benchInfo){
	char* device;
	if(onGPU){
		device = "GPU";
	}else{
		device = "CPU";
	}
	// Pad file size
	fileSize = fileSize + (fileSize%8);
	char* fileName = "benchmarks/benchPresent";
	buildFileOfZeroes(fileName,fileSize);
	uint64_t* presentCiphertext = (uint64_t*)malloc((fileSize/8)*sizeof(uint64_t));
	cl_event event = NULL;
	cl_ulong time_start, time_end;
	event = present_speed_CtrEncrypt(fileName, PresentKey, presentCiphertext, localSize, device);
	/* compute execution time */
	double total_time;
	clGetEventProfilingInfo(event, CL_PROFILING_COMMAND_START, sizeof(time_start), &time_start, NULL);
	clGetEventProfilingInfo(event, CL_PROFILING_COMMAND_END, sizeof(time_end), &time_end, NULL);
	total_time = time_end-time_start;
	printf("PRESENT SPEED CTR execution time is: %0.3f ms\n",total_time/1000000.0);
	free(presentCiphertext);
	remove(fileName);
	benchInfo->totalTime = total_time;
	benchInfo->localSize = localSize;
	benchInfo->fileSize = fileSize;
}

void benchPresentMemoryMultiple(int fileSize,int* localSize, int numOfLocalSizes, int onGPU){
	struct BenchInfo* infos = (struct BenchInfo*)malloc(numOfLocalSizes*sizeof(struct BenchInfo));
	for(int i = 0;i<numOfLocalSizes;i++){
		benchPresentMemoryCtr(fileSize,localSize[i],onGPU,&infos[i]);
	}
	saveDataToFile("PresMem",infos,numOfLocalSizes);
}

void benchPresentSpeedMultiple(int fileSize,int* localSize, int numOfLocalSizes, int onGPU){
	struct BenchInfo* infos = (struct BenchInfo*)malloc(numOfLocalSizes*sizeof(struct BenchInfo));
	for(int i = 0;i<numOfLocalSizes;i++){
		benchPresentSpeedCtr(fileSize,localSize[i],onGPU,&infos[i]);
	}
	saveDataToFile("PresSpeed",infos,numOfLocalSizes);
}
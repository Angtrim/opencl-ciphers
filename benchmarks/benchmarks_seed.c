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
#include "benchmarks_seed.h"

static uint32_t SeedKey[4][4] = {
	{0x00000000, 0x00000000, 0x00000000, 0x00000000},
	{0x00010203, 0x04050607, 0x08090A0B, 0x0C0D0E0F},
	{0x47064808, 0x51E61BE8, 0x5D74BFB3, 0xFD956185},
	{0x28DBC3BC, 0x49FFD87D, 0xCFA509B1, 0x1D422BE7}
};

void benchSeedOldCtr(int fileSize,int localSize, struct BenchInfo* benchInfo,cl_device_id* device_id){

	// Pad file size
	fileSize = fileSize + (fileSize%16);
	char* fileName = "benchmarks/benchSeed";
	buildFileOfZeroes(fileName,fileSize);
	uint64_t* seedCiphertext = (uint64_t*)malloc((fileSize/8)*sizeof(uint64_t));
	cl_event event = NULL;
	cl_ulong time_start, time_end;
	event = seed_old_CtrEncrypt(fileName, SeedKey[3], seedCiphertext, localSize, device_id);
	/* compute execution time */
	double total_time;
	clGetEventProfilingInfo(event, CL_PROFILING_COMMAND_START, sizeof(time_start), &time_start, NULL);
	clGetEventProfilingInfo(event, CL_PROFILING_COMMAND_END, sizeof(time_end), &time_end, NULL);
	total_time = time_end-time_start;
	printf("SEED OLD CTR execution time is: %0.3f ms\n",total_time/1000000.0);
	free(seedCiphertext);
	remove(fileName);
	benchInfo->totalTime = total_time;
	benchInfo->localSize = localSize;
	benchInfo->fileSize = fileSize;
}

void benchSeedCtr(int fileSize,int localSize, struct BenchInfo* benchInfo,cl_device_id* device_id){

	// Pad file size
	fileSize = fileSize + (fileSize%16);
	char* fileName = "benchmarks/benchSeed";
	buildFileOfZeroes(fileName,fileSize);
	uint64_t* seedCiphertext = (uint64_t*)malloc((fileSize/8)*sizeof(uint64_t));
	cl_event event = NULL;
	cl_ulong time_start, time_end;
	event = seed_CtrEncrypt(fileName, SeedKey[3], seedCiphertext, localSize, device_id);
	/* compute execution time */
	double total_time;
	clGetEventProfilingInfo(event, CL_PROFILING_COMMAND_START, sizeof(time_start), &time_start, NULL);
	clGetEventProfilingInfo(event, CL_PROFILING_COMMAND_END, sizeof(time_end), &time_end, NULL);
	total_time = time_end-time_start;
	printf("SEED CTR execution time is: %0.3f ms\n",total_time/1000000.0);
	free(seedCiphertext);
	remove(fileName);
	benchInfo->totalTime = total_time;
	benchInfo->localSize = localSize;
	benchInfo->fileSize = fileSize;
}

void benchSeedOldCtrMultiple(int fileSize,int* localSize, int numOfLocalSizes, cl_device_id* device_id){
	struct BenchInfo* infos = (struct BenchInfo*)malloc(numOfLocalSizes*sizeof(struct BenchInfo));
	for(int i = 0;i<numOfLocalSizes;i++){
		benchSeedOldCtr(fileSize,localSize[i],&infos[i], device_id);
	}
	saveDataToFile("SeedOLD",infos,numOfLocalSizes);
}

void benchSeedCtrMultiple(int fileSize,int* localSize, int numOfLocalSizes, cl_device_id* device_id){
	struct BenchInfo* infos = (struct BenchInfo*)malloc(numOfLocalSizes*sizeof(struct BenchInfo));
	for(int i = 0;i<numOfLocalSizes;i++){
		benchSeedCtr(fileSize,localSize[i],&infos[i],device_id);
	}
	saveDataToFile("Seed",infos,numOfLocalSizes);
}
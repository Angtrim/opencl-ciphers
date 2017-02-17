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
#include "benchmarks_aes.h"

static uint32_t SeedKey[4][4] = {
  {0x00000000, 0x00000000, 0x00000000, 0x00000000},
  {0x00010203, 0x04050607, 0x08090A0B, 0x0C0D0E0F},
  {0x47064808, 0x51E61BE8, 0x5D74BFB3, 0xFD956185},
  {0x28DBC3BC, 0x49FFD87D, 0xCFA509B1, 0x1D422BE7}
};

void benchSeedOldCtr(int fileSize,int localSize,int onGPU, struct BenchInfo* benchInfo){
	char* device;
	if(onGPU){
		device = "GPU";
	}else{
		device = "CPU";
	}
	// Pad file size
	fileSize = fileSize + (fileSize%16);
	char* fileName = "benchmarks/benchSeedOld";
	buildFileOfZeroes(fileName,fileSize);
	uint64_t* seedCiphertext = (uint64_t*)malloc((fileSize)*sizeof(uint64_t));
	cl_event event = NULL;
	cl_ulong time_start, time_end;
	event = seed_old_CtrEncrypt(fileName, SeedKey[3], seedCiphertext, 1, device);
	/* compute execution time */
	double total_time;
	clGetEventProfilingInfo(event, CL_PROFILING_COMMAND_START, sizeof(time_start), &time_start, NULL);
	clGetEventProfilingInfo(event, CL_PROFILING_COMMAND_END, sizeof(time_end), &time_end, NULL);
	total_time = time_end-time_start;
	printf("Aes 128 CTR execution time is: %0.3f ms\n",total_time/1000000.0);
	free(seedCiphertext);
	remove(fileName);
	benchInfo->totalTime = total_time;
	benchInfo->localSize = localSize;
	benchInfo->fileSize = fileSize;

}
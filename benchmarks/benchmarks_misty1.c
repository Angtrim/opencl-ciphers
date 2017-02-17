#ifdef __APPLE__
#include <OpenCL/opencl.h>
#else
#include <CL/cl.h>
#endif
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "../misty1_ctr/misty1_cipher.h"
#include "benchmarks_misty1.h"

static uint8_t Misty1Key[16] = {
	0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77,
	0x88, 0x99, 0xaa, 0xbb, 0xcc, 0xdd, 0xee, 0xff
};

void benchMisty1Ctr(int fileSize,int localSize,int onGPU, struct BenchInfo* benchInfo){
	char* device;
	if(onGPU){
		device = "GPU";
	}else{
		device = "CPU";
	}
	// Pad file size
	fileSize = fileSize + (fileSize%8);
	char* fileName = "benchmarks/benchMisty1";
	buildFileOfZeroes(fileName,fileSize);
	uint64_t* misty1Ciphertext = (uint64_t*)malloc((fileSize/8)*sizeof(uint64_t));
	cl_event event = NULL;
	cl_ulong time_start, time_end;
	event = misty1CtrEncrypt(fileName, Misty1Key, misty1Ciphertext, localSize, device);
	/* compute execution time */
	double total_time;
	clGetEventProfilingInfo(event, CL_PROFILING_COMMAND_START, sizeof(time_start), &time_start, NULL);
	clGetEventProfilingInfo(event, CL_PROFILING_COMMAND_END, sizeof(time_end), &time_end, NULL);
	total_time = time_end-time_start;
	printf("MISTY1 CTR execution time is: %0.3f ms\n",total_time/1000000.0);
	free(misty1Ciphertext);
	remove(fileName);
	benchInfo->totalTime = total_time;
	benchInfo->localSize = localSize;
	benchInfo->fileSize = fileSize;

}

void benchMisty1CtrMultiple(int fileSize,int* localSize, int numOfLocalSizes, int onGPU){
	struct BenchInfo* infos = (struct BenchInfo*)malloc(numOfLocalSizes*sizeof(struct BenchInfo));
	for(int i = 0;i<numOfLocalSizes;i++){
		benchMisty1Ctr(fileSize,localSize[i],onGPU,&infos[i]);
	}
	saveDataToFile("Misty",infos,numOfLocalSizes);
}
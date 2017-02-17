#ifdef __APPLE__
#include <OpenCL/opencl.h>
#else
#include <CL/cl.h>
#endif
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "../clefia_ctr/clefia_cipher.h"
#include "benchmarks_clefia.h"

static const unsigned char skey[32] = {
	0xffU,0xeeU,0xddU,0xccU,0xbbU,0xaaU,0x99U,0x88U,
	0x77U,0x66U,0x55U,0x44U,0x33U,0x22U,0x11U,0x00U,
	0xf0U,0xe0U,0xd0U,0xc0U,0xb0U,0xa0U,0x90U,0x80U,
	0x70U,0x60U,0x50U,0x40U,0x30U,0x20U,0x10U,0x00U
};



void benchClefia128(int fileSize,int localSize,int onGPU, struct BenchInfo* benchInfo){
	char* device;
	if(onGPU){
		device = "GPU";
	}else{
		device = "CPU";
	}
	// Pad file size
	fileSize = fileSize + (fileSize%16);
	char* fileName = "benchClef";
	buildFileOfZeroes(fileName,fileSize);
	uint8_t* clefCiphertext = (uint8_t*)malloc(fileSize*sizeof(uint8_t));
	cl_event event = NULL;
	cl_ulong time_start, time_end;
	event = clefia_128_Encrypt(fileName, skey, clefCiphertext, 1,device);

	/* compute execution time */
	double total_time;
	clGetEventProfilingInfo(event, CL_PROFILING_COMMAND_START, sizeof(time_start), &time_start, NULL);
	clGetEventProfilingInfo(event, CL_PROFILING_COMMAND_END, sizeof(time_end), &time_end, NULL);
	total_time = time_end-time_start;
	printf("Clefia 128 execution time is: %0.3f ms\n",total_time/1000000.0);
	free(clefCiphertext);
	remove(fileName);
	benchInfo->totalTime = total_time;
	benchInfo->localSize = localSize;
	benchInfo->fileSize = fileSize;
}

void benchClefia128Multiple(int fileSize,int* localSize, int numOfLocalSizes, int onGPU){
	struct BenchInfo* infos = (struct BenchInfo*)malloc(numOfLocalSizes*sizeof(struct BenchInfo));
	for(int i = 0;i<numOfLocalSizes;i++){
		benchClefia128(fileSize,localSize[i],onGPU,&infos[i]);
	}
}

void benchClefia256(int fileSize,int localSize,int onGPU, struct BenchInfo* benchInfo){
	char* device;
	if(onGPU){
		device = "GPU";
	}else{
		device = "CPU";
	}
	// Pad file size
	fileSize = fileSize + (fileSize%16);
	char* fileName = "benchClef";
	buildFileOfZeroes(fileName,fileSize);
	uint8_t* clefCiphertext = (uint8_t*)malloc(fileSize*sizeof(uint8_t));
	cl_event event = NULL;
	cl_ulong time_start, time_end;
	event = clefia_256_Encrypt(fileName, skey, clefCiphertext, 1,device);

	/* compute execution time */
	double total_time;
	clGetEventProfilingInfo(event, CL_PROFILING_COMMAND_START, sizeof(time_start), &time_start, NULL);
	clGetEventProfilingInfo(event, CL_PROFILING_COMMAND_END, sizeof(time_end), &time_end, NULL);
	total_time = time_end-time_start;
	printf("Clefia 256 execution time is: %0.3f ms\n",total_time/1000000.0);
	free(clefCiphertext);
	remove(fileName);
	benchInfo->totalTime = total_time;
	benchInfo->localSize = localSize;
	benchInfo->fileSize = fileSize;
}

void benchClefia256Multiple(int fileSize,int* localSize, int numOfLocalSizes, int onGPU){
	struct BenchInfo* infos = (struct BenchInfo*)malloc(numOfLocalSizes*sizeof(struct BenchInfo));
	for(int i = 0;i<numOfLocalSizes;i++){
		benchClefia256(fileSize,localSize[i],onGPU,&infos[i]);
	}
}

void benchClefia192(int fileSize,int localSize,int onGPU, struct BenchInfo* benchInfo){
	char* device;
	if(onGPU){
		device = "GPU";
	}else{
		device = "CPU";
	}
	// Pad file size
	fileSize = fileSize + (fileSize%16);
	char* fileName = "benchClef";
	buildFileOfZeroes(fileName,fileSize);
	uint8_t* clefCiphertext = (uint8_t*)malloc(fileSize*sizeof(uint8_t));
	cl_event event = NULL;
	cl_ulong time_start, time_end;
	event = clefia_192_Encrypt(fileName, skey, clefCiphertext, 1,device);

	/* compute execution time */
	double total_time;
	clGetEventProfilingInfo(event, CL_PROFILING_COMMAND_START, sizeof(time_start), &time_start, NULL);
	clGetEventProfilingInfo(event, CL_PROFILING_COMMAND_END, sizeof(time_end), &time_end, NULL);
	total_time = time_end-time_start;
	printf("Clefia 192 execution time is: %0.3f ms\n",total_time/1000000.0);
	free(clefCiphertext);
	remove(fileName);
	benchInfo->totalTime = total_time;
	benchInfo->localSize = localSize;
	benchInfo->fileSize = fileSize;
}

void benchClefia192Multiple(int fileSize,int* localSize, int numOfLocalSizes, int onGPU){
	struct BenchInfo* infos = (struct BenchInfo*)malloc(numOfLocalSizes*sizeof(struct BenchInfo));
	for(int i = 0;i<numOfLocalSizes;i++){
		benchClefia192(fileSize,localSize[i],onGPU,&infos[i]);
	}
}


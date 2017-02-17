#ifdef __APPLE__
#include <OpenCL/opencl.h>
#else
#include <CL/cl.h>
#endif
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "../camellia_ctr/camellia_cipher.h"
#include "benchmarks_camellia.h"

static uint64_t Key128[2] = {0x0123456789abcdef, 0xfedcba9876543210};
static uint64_t Key192[3] = {0x0123456789abcdef, 0xfedcba9876543210, 0x0011223344556677};
static uint64_t Key256[4] = {0x0123456789abcdef, 0xfedcba9876543210,0x0011223344556677, 0x8899aabbccddeeff};



void benchCam128(int fileSize,int localSize,int onGPU, struct BenchInfo* benchInfo){
	char* device;
	if(onGPU){
		device = "GPU";
	}else{
		device = "CPU";
	}
	// Pad file size
	fileSize = fileSize + (fileSize%16);
	char* fileName = "benchCam";
	buildFileOfZeroes(fileName,fileSize);
	uint64_t* camCiphertext = (uint64_t*)malloc((fileSize/8)*sizeof(uint64_t));
	cl_event event = NULL;
	cl_ulong time_start, time_end;
	event = camelliaCtr128Encrypt(fileName, Key128, camCiphertext, 1,device);

	/* compute execution time */
	double total_time;
	clGetEventProfilingInfo(event, CL_PROFILING_COMMAND_START, sizeof(time_start), &time_start, NULL);
	clGetEventProfilingInfo(event, CL_PROFILING_COMMAND_END, sizeof(time_end), &time_end, NULL);
	total_time = time_end-time_start;
	printf("Camellia 128 execution time is: %0.3f ms\n",total_time/1000000.0);
	free(camCiphertext);
	remove(fileName);
	benchInfo->totalTime = total_time;
	benchInfo->localSize = localSize;
	benchInfo->fileSize = fileSize;
}

void benchCam128Multiple(int fileSize,int* localSize, int numOfLocalSizes, int onGPU){
	struct BenchInfo* infos = (struct BenchInfo*)malloc(numOfLocalSizes*sizeof(struct BenchInfo));
	for(int i = 0;i<numOfLocalSizes;i++){
		benchCam128(fileSize,localSize[i],onGPU,&infos[i]);
	}
	saveDataToFile("CAM128",infos,numOfLocalSizes);
}

void benchCam192(int fileSize,int localSize,int onGPU, struct BenchInfo* benchInfo){
	char* device;
	if(onGPU){
		device = "GPU";
	}else{
		device = "CPU";
	}
	// Pad file size
	fileSize = fileSize + (fileSize%16);
	char* fileName = "benchCam";
	buildFileOfZeroes(fileName,fileSize);
	uint64_t* camCiphertext = (uint64_t*)malloc((fileSize/8)*sizeof(uint64_t));
	cl_event event = NULL;
	cl_ulong time_start, time_end;
	event = camelliaCtr192Encrypt(fileName, Key192, camCiphertext, 1,device);

	/* compute execution time */
	double total_time;
	clGetEventProfilingInfo(event, CL_PROFILING_COMMAND_START, sizeof(time_start), &time_start, NULL);
	clGetEventProfilingInfo(event, CL_PROFILING_COMMAND_END, sizeof(time_end), &time_end, NULL);
	total_time = time_end-time_start;
	printf("Camellia 192 execution time is: %0.3f ms\n",total_time/1000000.0);
	free(camCiphertext);
	remove(fileName);
	benchInfo->totalTime = total_time;
	benchInfo->localSize = localSize;
	benchInfo->fileSize = fileSize;
}

void benchCam192Multiple(int fileSize,int* localSize, int numOfLocalSizes, int onGPU){
	struct BenchInfo* infos = (struct BenchInfo*)malloc(numOfLocalSizes*sizeof(struct BenchInfo));
	for(int i = 0;i<numOfLocalSizes;i++){
		benchCam192(fileSize,localSize[i],onGPU,&infos[i]);
	}
	saveDataToFile("CAM192",infos,numOfLocalSizes);
}

void benchCam256(int fileSize,int localSize,int onGPU, struct BenchInfo* benchInfo){
	char* device;
	if(onGPU){
		device = "GPU";
	}else{
		device = "CPU";
	}
	// Pad file size
	fileSize = fileSize + (fileSize%16);
	char* fileName = "benchCam";
	buildFileOfZeroes(fileName,fileSize);
	uint64_t* camCiphertext = (uint64_t*)malloc((fileSize/8)*sizeof(uint64_t));
	cl_event event = NULL;
	cl_ulong time_start, time_end;
	event = camelliaCtr256Encrypt(fileName, Key256, camCiphertext, 1,device);

	/* compute execution time */
	double total_time;
	clGetEventProfilingInfo(event, CL_PROFILING_COMMAND_START, sizeof(time_start), &time_start, NULL);
	clGetEventProfilingInfo(event, CL_PROFILING_COMMAND_END, sizeof(time_end), &time_end, NULL);
	total_time = time_end-time_start;
	printf("Camellia 256 execution time is: %0.3f ms\n",total_time/1000000.0);
	free(camCiphertext);
	remove(fileName);
	benchInfo->totalTime = total_time;
	benchInfo->localSize = localSize;
	benchInfo->fileSize = fileSize;
}

void benchCam256Multiple(int fileSize,int* localSize, int numOfLocalSizes, int onGPU){
	struct BenchInfo* infos = (struct BenchInfo*)malloc(numOfLocalSizes*sizeof(struct BenchInfo));
	for(int i = 0;i<numOfLocalSizes;i++){
		benchCam256(fileSize,localSize[i],onGPU,&infos[i]);
	}
	saveDataToFile("CAM256",infos,numOfLocalSizes);
}

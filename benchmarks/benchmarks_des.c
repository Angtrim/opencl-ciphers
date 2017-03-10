#ifdef __APPLE__
#include <OpenCL/opencl.h>
#else
#include <CL/cl.h>
#endif
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "../des_ctr/des_cipher.h"
#include "benchmarks_des.h"

static uint8_t DES3_keys[24] = {0x2B, 0xD6, 0x45, 0x9F, 0x82, 0xC5, 0xB3, 0x00, 0x95, 0x2C, 0x49, 0x10, 0x48, 0x81, 0xFF, 0x48, 0x2B, 0xD6, 0x45, 0x9F, 0x82, 0xC5, 0xB3, 0x00};


void benchDes(int fileSize,int localSize, struct BenchInfo* benchInfo,cl_device_id* device_id){

	// Pad file size
	fileSize = fileSize + (fileSize%8);
	char* fileName = "benchDes";
	buildFileOfZeroes(fileName,fileSize);
	uint8_t* desCiphertext = (uint8_t*)malloc((fileSize)*sizeof(uint8_t));
	cl_event event = NULL;
	cl_ulong time_start, time_end;
	event = desCtrEncrypt(fileName, DES3_keys, desCiphertext ,localSize, device_id);
	/* compute execution time */
	double total_time;
	clGetEventProfilingInfo(event, CL_PROFILING_COMMAND_START, sizeof(time_start), &time_start, NULL);
	clGetEventProfilingInfo(event, CL_PROFILING_COMMAND_END, sizeof(time_end), &time_end, NULL);
	total_time = time_end-time_start;
	printf("Des execution time is: %0.3f ms\n",total_time/1000000.0);
	free(desCiphertext);
	remove(fileName);
	benchInfo->totalTime = total_time;
	benchInfo->localSize = localSize;
	benchInfo->fileSize = fileSize;

}

void benchDes2(int fileSize,int localSize,struct BenchInfo* benchInfo,cl_device_id* device_id){

	// Pad file size
	fileSize = fileSize + (fileSize%16);
	char* fileName = "benchDes";
	buildFileOfZeroes(fileName,fileSize);
	uint8_t* desCiphertext = (uint8_t*)malloc((fileSize)*sizeof(uint8_t));
	cl_event event = NULL;
	cl_ulong time_start, time_end;
	event = des2CtrEncrypt(fileName, DES3_keys, desCiphertext ,localSize, device_id);
	/* compute execution time */
	double total_time;
	clGetEventProfilingInfo(event, CL_PROFILING_COMMAND_START, sizeof(time_start), &time_start, NULL);
	clGetEventProfilingInfo(event, CL_PROFILING_COMMAND_END, sizeof(time_end), &time_end, NULL);
	total_time = time_end-time_start;
	printf("Des 2 execution time is: %0.3f ms\n",total_time/1000000.0);
	free(desCiphertext);
	remove(fileName);
	benchInfo->totalTime = total_time;
	benchInfo->localSize = localSize;
	benchInfo->fileSize = fileSize;

}

void benchDes3(int fileSize,int localSize, struct BenchInfo* benchInfo,cl_device_id* device_id){

	// Pad file size
	fileSize = fileSize + (fileSize%16);
	char* fileName = "benchDes";
	buildFileOfZeroes(fileName,fileSize);
	uint8_t* desCiphertext = (uint8_t*)malloc((fileSize)*sizeof(uint8_t));
	cl_event event = NULL;
	cl_ulong time_start, time_end;
	event = des3CtrEncrypt(fileName, DES3_keys, desCiphertext ,localSize, device_id);
	/* compute execution time */
	double total_time;
	clGetEventProfilingInfo(event, CL_PROFILING_COMMAND_START, sizeof(time_start), &time_start, NULL);
	clGetEventProfilingInfo(event, CL_PROFILING_COMMAND_END, sizeof(time_end), &time_end, NULL);
	total_time = time_end-time_start;
	printf("Des 3 execution time is: %0.3f ms\n",total_time/1000000.0);
	free(desCiphertext);
	remove(fileName);
	benchInfo->totalTime = total_time;
	benchInfo->localSize = localSize;
	benchInfo->fileSize = fileSize;

}


void benchDesMultiple(int fileSize,int* localSize, int numOfLocalSizes, cl_device_id* device_id){
	struct BenchInfo* infos = (struct BenchInfo*)malloc(numOfLocalSizes*sizeof(struct BenchInfo));
	for(int i = 0;i<numOfLocalSizes;i++){
		benchDes(fileSize,localSize[i],&infos[i],device_id);
	}
	saveDataToFile("Des",infos,numOfLocalSizes);
}

void benchDes2Multiple(int fileSize,int* localSize, int numOfLocalSizes,cl_device_id* device_id){
	struct BenchInfo* infos = (struct BenchInfo*)malloc(numOfLocalSizes*sizeof(struct BenchInfo));
	for(int i = 0;i<numOfLocalSizes;i++){
		benchDes2(fileSize,localSize[i],&infos[i], device_id);
	}
	saveDataToFile("Des2",infos,numOfLocalSizes);
}

void benchDes3Multiple(int fileSize,int* localSize, int numOfLocalSizes, cl_device_id* device_id){
	struct BenchInfo* infos = (struct BenchInfo*)malloc(numOfLocalSizes*sizeof(struct BenchInfo));
	for(int i = 0;i<numOfLocalSizes;i++){
		benchDes3(fileSize,localSize[i],&infos[i],device_id);
	}
	saveDataToFile("Des3",infos,numOfLocalSizes);
}
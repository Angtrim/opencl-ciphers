
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "../aes_ctr/aes_cipher.h"
#include "benchmarks_aes.h"

static uint32_t aes128Key[16] = {0x6ede8bab, 0xdad63ffa, 0x796c767d, 0x92e52e38};
static uint32_t aes192Key[24] = {0xded642b8, 0x2daf73f0, 0x7a0f5caa, 0x554dc9c6, 0x21437628, 0x99c92ca2};
static uint32_t aes256Key[32] = {0x63375233, 0xca899f06, 0x2f868c91, 0x8eb2ccc1, 0x36eb5001, 0x58c75cbb, 0x2ee60020, 0x3286d86f};


void benchAes128(int fileSize,int localSize,struct BenchInfo* benchInfo,cl_device_id* device_id){
	// Pad file size
	fileSize = fileSize + (fileSize%16);
	char* fileName = "benchmarks/benchAes";
	buildFileOfZeroes(fileName,fileSize);
	uint8_t* aesCiphertext = (uint8_t*)malloc((fileSize)*sizeof(uint8_t));
	cl_event event = NULL;
	cl_ulong time_start, time_end;
	event = aes128CtrEncrypt(fileName, aes128Key, aesCiphertext, localSize, device_id);
	/* compute execution time */
	double total_time;
	clGetEventProfilingInfo(event, CL_PROFILING_COMMAND_START, sizeof(time_start), &time_start, NULL);
	clGetEventProfilingInfo(event, CL_PROFILING_COMMAND_END, sizeof(time_end), &time_end, NULL);
	total_time = time_end-time_start;
	printf("Aes 128 CTR execution time is: %0.3f ms\n",total_time/1000000.0);
	free(aesCiphertext);
	remove(fileName);
	benchInfo->totalTime = total_time;
	benchInfo->localSize = localSize;
	benchInfo->fileSize = fileSize;

}

void benchAes192(int fileSize,int localSize, struct BenchInfo* benchInfo,cl_device_id* device_id){
	// Pad file size
	fileSize = fileSize + (fileSize%16);
	char* fileName = "benchmarks/benchAes";
	buildFileOfZeroes(fileName,fileSize);
	uint8_t* aesCiphertext = (uint8_t*)malloc((fileSize)*sizeof(uint8_t));
	cl_event event = NULL;
	cl_ulong time_start, time_end;
	event = aes192CtrEncrypt(fileName, aes192Key, aesCiphertext, localSize, device_id);
	/* compute execution time */
	double total_time;
	clGetEventProfilingInfo(event, CL_PROFILING_COMMAND_START, sizeof(time_start), &time_start, NULL);
	clGetEventProfilingInfo(event, CL_PROFILING_COMMAND_END, sizeof(time_end), &time_end, NULL);
	total_time = time_end-time_start;
	printf("Aes 192 CTR execution time is: %0.3f ms\n",total_time/1000000.0);
	free(aesCiphertext);
	remove(fileName);
	benchInfo->totalTime = total_time;
	benchInfo->localSize = localSize;
	benchInfo->fileSize = fileSize;

}

void benchAes256(int fileSize,int localSize, struct BenchInfo* benchInfo,cl_device_id* device_id){

	// Pad file size
	fileSize = fileSize + (fileSize%16);
	char* fileName = "benchmarks/benchAes";
	buildFileOfZeroes(fileName,fileSize);
	uint8_t* aesCiphertext = (uint8_t*)malloc((fileSize)*sizeof(uint8_t));
	cl_event event = NULL;
	cl_ulong time_start, time_end;
	event = aes256CtrEncrypt(fileName, aes256Key, aesCiphertext, localSize, device_id);
	/* compute execution time */
	double total_time;
	clGetEventProfilingInfo(event, CL_PROFILING_COMMAND_START, sizeof(time_start), &time_start, NULL);
	clGetEventProfilingInfo(event, CL_PROFILING_COMMAND_END, sizeof(time_end), &time_end, NULL);
	total_time = time_end-time_start;
	printf("Aes 256 CTR execution time is: %0.3f ms\n",total_time/1000000.0);
	free(aesCiphertext);
	remove(fileName);
	benchInfo->totalTime = total_time;
	benchInfo->localSize = localSize;
	benchInfo->fileSize = fileSize;

}


void benchAes128Multiple(int fileSize,int* localSize, int numOfLocalSizes, cl_device_id* device_id){
	struct BenchInfo* infos = (struct BenchInfo*)malloc(numOfLocalSizes*sizeof(struct BenchInfo));
	for(int i = 0;i<numOfLocalSizes;i++){
		benchAes128(fileSize,localSize[i],&infos[i], device_id);
	}
	saveDataToFile("AES128",infos,numOfLocalSizes);
}

void benchAes192Multiple(int fileSize,int* localSize, int numOfLocalSizes, cl_device_id* device_id){
	struct BenchInfo* infos = (struct BenchInfo*)malloc(numOfLocalSizes*sizeof(struct BenchInfo));
	for(int i = 0;i<numOfLocalSizes;i++){
		benchAes192(fileSize,localSize[i],&infos[i],device_id);
	}
	saveDataToFile("AES192",infos,numOfLocalSizes);
}

void benchAes256Multiple(int fileSize,int* localSize, int numOfLocalSizes, cl_device_id* device_id){
	struct BenchInfo* infos = (struct BenchInfo*)malloc(numOfLocalSizes*sizeof(struct BenchInfo));
	for(int i = 0;i<numOfLocalSizes;i++){
		benchAes256(fileSize,localSize[i],&infos[i], device_id);
	}
	saveDataToFile("AES256",infos,numOfLocalSizes);
}

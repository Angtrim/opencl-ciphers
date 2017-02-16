#ifdef __APPLE__
#include <OpenCL/opencl.h>
#else
#include <CL/cl.h>
#endif
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "../aes_test_vect.h"
#include "../aes_ctr/aes_cipher.h"
#include "benchmarks_utils.h"




// to save ciphertext to file (uint8_t)
static void buildFileOfZeroes(char* outFileName,long lenght){
	uint8_t* zeroes = (uint8_t*)malloc(lenght*sizeof(uint8_t));
	// fill with zeroes
	for(int i = 0; i <lenght; i++){
		 zeroes[i] = 0;	
	}
	FILE* fp = fopen(outFileName, "wb");
	if (!fp) {
	fprintf(stderr, "Failed to load file.\n");
	exit(1);
	}
	fwrite(zeroes, sizeof(char), lenght, fp);
	fclose(fp);
	free(zeroes);
}






void benchAes128(int fileSize,int localSize,int onGPU, struct BenchInfo* benchInfo){
	char* device;
	if(onGPU){
		device = "GPU";
	}else{
		device = "CPU";
	}
	// Pad file size
	fileSize = fileSize + (fileSize%16);
	char* fileName = "benchAes128";
	buildFileOfZeroes(fileName,fileSize);
	uint8_t* aesCiphertext = (uint8_t*)malloc((fileSize)*sizeof(uint8_t));
	cl_event event = NULL;
 cl_ulong time_start, time_end;
 event = aes128CtrEncrypt("benchAes128", aes128Key, aesCiphertext, 1, device);
	/* compute execution time */
	double total_time;
	clGetEventProfilingInfo(event, CL_PROFILING_COMMAND_START, sizeof(time_start), &time_start, NULL);
 clGetEventProfilingInfo(event, CL_PROFILING_COMMAND_END, sizeof(time_end), &time_end, NULL);
 total_time = time_end-time_start;
 printf("Aes 128 execution time is: %0.3f ms\n",total_time/1000000.0);
	free(aesCiphertext);
	remove(fileName);
	benchInfo->totalTime = total_time;
	benchInfo->localSize = localSize;
	benchInfo->fileSize = fileSize;

}


void benchAes128Multiple(int fileSize,int* localSize, int numOfLocalSizes, int onGPU){
		struct BenchInfo* infos = (struct BenchInfo*)malloc(numOfLocalSizes*sizeof(struct BenchInfo));
		for(int i = 0;i<numOfLocalSizes;i++){
			benchAes128(fileSize,localSize[i],onGPU,&infos[i]);
		}
}

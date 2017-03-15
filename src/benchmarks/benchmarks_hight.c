#ifdef __APPLE__
#include <OpenCL/opencl.h>
#else
#include <CL/cl.h>
#endif
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "../hight_ctr/hight_cipher.h"
#include "benchmarks_hight.h"

static uint8_t HightKeys[2][16] = { 
	{0xff, 0xee, 0xdd, 0xcc, 0xbb, 0xaa, 0x99, 0x88,
		0x77, 0x66, 0x55, 0x44, 0x33, 0x22, 0x11, 0x00},
		{0xff, 0xee, 0xdd, 0xcc, 0xbb, 0xaa, 0x99, 0x88,
			0x77, 0x66, 0x55, 0x44, 0x33, 0x22, 0x11, 0x00}
		};

		void benchHight(int fileSize,int localSize, struct BenchInfo* benchInfo,cl_device_id* device_id){

	// Pad file size
	fileSize = fileSize - (fileSize%(8*localSize));
			char* fileName = "benchHight";
			buildFileOfZeroes(fileName,fileSize);
			uint64_t* hightCiphertext = (uint64_t*)malloc((fileSize/8)*sizeof(uint64_t));
			cl_event event = NULL;
			cl_ulong time_start, time_end;
			event = hightCtrEncrypt(fileName, HightKeys[0], hightCiphertext ,localSize,device_id);
	/* compute execution time */
			double total_time;
			clGetEventProfilingInfo(event, CL_PROFILING_COMMAND_START, sizeof(time_start), &time_start, NULL);
			clGetEventProfilingInfo(event, CL_PROFILING_COMMAND_END, sizeof(time_end), &time_end, NULL);
			total_time = time_end-time_start;
			printf("Hight execution time is: %0.3f ms\n",total_time/1000000.0);
			free(hightCiphertext);
			remove(fileName);
			benchInfo->totalTime = total_time;
			benchInfo->localSize = localSize;
			benchInfo->fileSize = fileSize;
			clReleaseEvent(event);

		}

		void benchHightMultiple(int fileSize,int* localSize, int numOfLocalSizes, cl_device_id* device_id){
			struct BenchInfo* infos = (struct BenchInfo*)malloc(numOfLocalSizes*sizeof(struct BenchInfo));
			for(int i = 0;i<numOfLocalSizes;i++){
				benchHight(fileSize,localSize[i],&infos[i], device_id);
			}
			saveDataToFile("Hight",infos,numOfLocalSizes);
			free(infos);
		}
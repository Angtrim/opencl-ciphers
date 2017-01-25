#include "benchmark.h"
#include "aes_cipher.h"

#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#ifdef __APPLE__
#include <OpenCL/opencl.h>
#else
#include <CL/cl.h>
#endif

#define WORK_GROUP_SIZE 1
#define MEM_SIZE (128)
#define MAX_SOURCE_SIZE (0x100000)



/*
   This function adds two string pointers together
*/
char* stradd(const char* a, const char* b){
	size_t len = strlen(a) + strlen(b);
	char *ret = (char*)malloc(len * sizeof(char) + 1);
	*ret = '\0';
	return strcat(strcat(ret, a) ,b);
} 

void aesCtrEncrypt(long num_blocks, byte inputText[num_blocks][BLOCK_SIZE], word key[Nk],  byte output[num_blocks][BLOCK_SIZE]) {
	//opencl parameters initialization
	//to run the kernel
	cl_device_id device_id = NULL;
	cl_context context = NULL;
	cl_command_queue command_queue = NULL;

	cl_mem out = NULL;
	cl_mem in = NULL;
	cl_mem exKey = NULL;

	cl_program program = NULL;
	cl_kernel kernel = NULL;
	cl_platform_id platform_id = NULL;
	cl_uint ret_num_devices;
	cl_uint ret_num_platforms;
	cl_int ret;
	
	// number of work-items equal to the number of blocks
	size_t global_item_size = NUM_BLOCKS;
	size_t local_item_size = WORK_GROUP_SIZE;

	//key expansion is performed on cpu
 	int exKeyDim = Nb*(Nr+1);
	word w[exKeyDim];
	BEGIN_KEYSCHED;
	KeyExpansion(key, w);
	END_KEYSCHED;

	FILE *fp;
	char fileName[] = "aes_ctr/aes_ctr.cl";

	// to pass the constant parameter Nb and mode of operation flag
	char *append_str = "";
	char temp1[10];
	// pass Nb and Nr to opencl file
	append_str = stradd(append_str, "#define Nb");
	sprintf(temp1, " %d\n", Nb); // puts string into buffer
	append_str = stradd(append_str, temp1);
	append_str = stradd(append_str, "#define Nr");
	sprintf(temp1, " %d\n", Nr); // puts string into buffer
	append_str = stradd(append_str, temp1);
	// pass NUM_BLOCKS and BLOCK_SIZE to opencl file
	append_str = stradd(append_str, "#define NUM_BLOCKS");
	sprintf(temp1, " %d\n", NUM_BLOCKS); // puts string into buffer
	append_str = stradd(append_str, temp1);
	append_str = stradd(append_str, "#define BLOCK_SIZE");
	sprintf(temp1, " %d\n", BLOCK_SIZE); // puts string into buffer
	append_str = stradd(append_str, temp1);
	printf("%s\n", append_str);

	char *source_str;
	size_t source_size;

	/* Load the source code containing the kernel*/
	fp = fopen(fileName, "r");
	if (!fp) {
	fprintf(stderr, "Failed to load kernel.\n");
	exit(1);
	}
	source_str = (char*)malloc(MAX_SOURCE_SIZE);
	fread(source_str, 1, MAX_SOURCE_SIZE, fp);
	fclose(fp);

	append_str = stradd(append_str, source_str);
	source_size = strlen(append_str);

	/* Get Platform and Device Info */
	ret = clGetPlatformIDs(1, &platform_id, &ret_num_platforms);
	ret = clGetDeviceIDs(platform_id, CL_DEVICE_TYPE_GPU, 1, &device_id, &ret_num_devices);

	/* Create OpenCL context */
	context = clCreateContext(NULL, 1, &device_id, NULL, NULL, &ret);

	/* Create Command Queue */
	command_queue = clCreateCommandQueue(context, device_id, 0, &ret);

	/* Create Memory Buffers */
	in = clCreateBuffer(context, CL_MEM_READ_WRITE, NUM_BLOCKS * BLOCK_SIZE * sizeof(byte), NULL, &ret);
	exKey = clCreateBuffer(context, CL_MEM_READ_WRITE, exKeyDim * sizeof(word), NULL, &ret); 
	out = clCreateBuffer(context, CL_MEM_READ_WRITE, NUM_BLOCKS * BLOCK_SIZE * sizeof(byte), NULL, &ret);

	/* Copy input data to Memory Buffer */
	ret = clEnqueueWriteBuffer(command_queue, in, CL_TRUE, 0, NUM_BLOCKS * BLOCK_SIZE * sizeof(byte), inputText, 0, NULL, NULL);
	ret = clEnqueueWriteBuffer(command_queue, exKey, CL_TRUE, 0, exKeyDim * sizeof(word), w, 0, NULL, NULL);

	/* Create Kernel Program from the source */
	program = clCreateProgramWithSource(context, 1, (const char **)&append_str,
	(const size_t *)&source_size, &ret);

	/* Build Kernel Program */
	ret = clBuildProgram(program, 1, &device_id, NULL, NULL, NULL);
	if(ret != CL_SUCCESS){
	printf("\nBuild Error = %i", ret);
			// Determine the size of the log
	size_t log_size;
	clGetProgramBuildInfo(program, device_id, CL_PROGRAM_BUILD_LOG, 0, NULL, &log_size);

		// Allocate memory for the log
	char *log = (char *) malloc(log_size);

		// Get the log
	clGetProgramBuildInfo(program, device_id, CL_PROGRAM_BUILD_LOG, log_size, log, NULL);

		// Print the log
	printf("%s\n", log);
	}

	/* Create OpenCL Kernel */
	kernel = clCreateKernel(program, "aesCipherCtr", &ret);

	/* Set OpenCL Kernel Parameters */
	ret = clSetKernelArg(kernel, 0, sizeof(cl_mem), (void *)&in);
	ret = clSetKernelArg(kernel, 1, sizeof(cl_mem), (void *)&exKey);
	ret = clSetKernelArg(kernel, 2, sizeof(cl_mem), (void *)&out);

	/* Execute OpenCL Kernel instances */
	ret = clEnqueueNDRangeKernel(command_queue, kernel, 1, NULL, &global_item_size, &local_item_size, 0, NULL,NULL);

	/* Copy results from the memory buffer */
	ret = clEnqueueReadBuffer(command_queue, out, CL_TRUE, 0,
	NUM_BLOCKS * BLOCK_SIZE * sizeof(byte),output, 0, NULL, NULL);

	/* Finalization */
	ret = clFlush(command_queue);
	ret = clFinish(command_queue);
	ret = clReleaseKernel(kernel);
	ret = clReleaseProgram(program);
	ret = clReleaseMemObject(in);
	ret = clReleaseMemObject(exKey);
	ret = clReleaseMemObject(out);
	ret = clReleaseCommandQueue(command_queue);
	ret = clReleaseContext(context);
	free(source_str);

}

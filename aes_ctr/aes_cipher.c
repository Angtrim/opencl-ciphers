#include "benchmark.h"
#include "aes_cipher.h"
#include "cipher_utils.h"


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




/** -- opencl parameters initialization to run the kernel -- **/
cl_device_id device_id = NULL;
cl_context context = NULL;
cl_command_queue command_queue = NULL;

cl_mem out = NULL;
cl_mem in = NULL;
cl_mem exKey = NULL;
cl_mem Nb = NULL;
cl_mem Nr = NULL;

cl_program program = NULL;
cl_kernel kernel = NULL;
cl_platform_id platform_id = NULL;
cl_uint ret_num_devices;
cl_uint ret_num_platforms;
cl_int ret;

//cl file parameters
FILE *fp;
char fileName[] = "aes_ctr/aes_ctr.cl";






/*
   This function adds two string pointers together
*/
char* stradd(const char* a, const char* b){
	size_t len = strlen(a) + strlen(b);
	char *ret = (char*)malloc(len * sizeof(char) + 1);
	*ret = '\0';
	return strcat(strcat(ret, a) ,b);
} 

char* loadClProgramSource(){
	/* Load the source code containing the kernel*/
	fp = fopen(fileName, "r");
	if (!fp) {
	fprintf(stderr, "Failed to load kernel.\n");
	exit(1);
	}
	char* source_str = (char*)malloc(MAX_SOURCE_SIZE);
	fread(source_str, 1, MAX_SOURCE_SIZE, fp);
	fclose(fp);
	return source_str;
}

void setUpOpenCl(byte* inputText, word* w,  byte* output, char* kernelName, char* source_str, long source_size, long exKeyDim, long bufferLenght, int _Nb, int _Nr){
	/* Get Platform and Device Info */
	ret = clGetPlatformIDs(1, &platform_id, &ret_num_platforms);
	ret = clGetDeviceIDs(platform_id, CL_DEVICE_TYPE_GPU, 1, &device_id, &ret_num_devices);

	/* Create OpenCL context */
	context = clCreateContext(NULL, 1, &device_id, NULL, NULL, &ret);

	/* Create Command Queue */
	command_queue = clCreateCommandQueue(context, device_id, 0, &ret);

	/* Create Memory Buffers */
	in = clCreateBuffer(context, CL_MEM_READ_WRITE, bufferLenght * sizeof(byte), NULL, &ret);
	exKey = clCreateBuffer(context, CL_MEM_READ_WRITE, exKeyDim * sizeof(word), NULL, &ret); 
	out = clCreateBuffer(context, CL_MEM_READ_WRITE, bufferLenght * sizeof(byte), NULL, &ret);
	Nb = clCreateBuffer(context, CL_MEM_READ_WRITE, sizeof(int), NULL, &ret);
	Nr = clCreateBuffer(context, CL_MEM_READ_WRITE, sizeof(int), NULL, &ret);


	/* Copy input data to Memory Buffer */
	ret = clEnqueueWriteBuffer(command_queue, in, CL_TRUE, 0, bufferLenght * sizeof(byte), inputText, 0, NULL, NULL);
	ret = clEnqueueWriteBuffer(command_queue, exKey, CL_TRUE, 0, exKeyDim * sizeof(word), w, 0, NULL, NULL);
 ret = clEnqueueWriteBuffer(command_queue, Nb, CL_TRUE, 0, sizeof(int), _Nb, 0, NULL, NULL);
	ret = clEnqueueWriteBuffer(command_queue, Nr, CL_TRUE, 0, sizeof(int), _Nr, 0, NULL, NULL);

	/* Create Kernel Program from the source */
	program = clCreateProgramWithSource(context, 1, (const char **)&source_str,
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
	kernel = clCreateKernel(program, &kernelName, &ret);

	/* Set OpenCL Kernel Parameters */
	ret = clSetKernelArg(kernel, 0, sizeof(cl_mem), (void *)&in);
	ret = clSetKernelArg(kernel, 1, sizeof(cl_mem), (void *)&exKey);
	ret = clSetKernelArg(kernel, 2, sizeof(cl_mem), (void *)&out);
	ret = clSetKernelArg(kernel, 3, sizeof(cl_mem), (void *)&Nb);
	ret = clSetKernelArg(kernel, 4, sizeof(cl_mem), (void *)&Nr);
}

void finalizeExecution(){
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

void aes128Encrypt(char* fileName, word* key, byte* output,size_t local_item_size) {
	struct FileInfo fileInfo = getFileBytes(fileName);

	byte* inputText = fileInfo.filePointer;

	long exKeyDim = Nb128*(Nr128+1);
	//key expansion is performed on cpu
	word w[exKeyDim];
	BEGIN_KEYSCHED;
	KeyExpansion(key, w, Nk128, Nb128, Nr128);
	END_KEYSCHED;

	char* source_str = loadClProgramSource();
	long source_size = strlen(source_str);
	
	setUpOpenCl(inputText, w, output, "aesCipher",source_str,source_size,exKeyDim,fileInfo.lenght,Nb128,Nr128);
	
	size_t global_item_size = fileInfo.lenght/BLOCK_SIZE
	/* Execute OpenCL Kernel instances */
	ret = clEnqueueNDRangeKernel(command_queue, kernel, 1, NULL, &global_item_size, &local_item_size, 0, NULL,NULL);

	/* Copy results from the memory buffer */
	byte* outputBuffer = (byte *)malloc((fileInfo.lenght+1)*sizeof(byte)); // Enough memory for file + \0

	ret = clEnqueueReadBuffer(command_queue, out, CL_TRUE, 0,
	fileInfo.lenght* sizeof(byte),outputBuffer, 0, NULL, NULL);

	output = outputBuffer;

	finalizeExecution();
	
}

void aesCtr128Encrypt(char* fileName, word* key, byte* output,size_t local_item_size) {
	struct FileInfo fileInfo = getFileBytes(fileName);

	byte* inputText = fileInfo.filePointer;

	long exKeyDim = Nb128*(Nr128+1);
	//key expansion is performed on cpu
	word w[exKeyDim];
	BEGIN_KEYSCHED;
	KeyExpansion(key, w, Nk128, Nb128, Nr128);
	END_KEYSCHED;

	char* source_str = loadClProgramSource();
	long source_size = strlen(source_str);
	
	setUpOpenCl(inputText, w, output, "aesCTRCipher",source_str,source_size,exKeyDim,fileInfo.lenght,Nb128,Nr128);
	
	size_t global_item_size = fileInfo.lenght/BLOCK_SIZE
	/* Execute OpenCL Kernel instances */
	ret = clEnqueueNDRangeKernel(command_queue, kernel, 1, NULL, &global_item_size, &local_item_size, 0, NULL,NULL);

	/* Copy results from the memory buffer */
	byte* outputBuffer = (byte *)malloc((fileInfo.lenght+1)*sizeof(byte)); // Enough memory for file + \0

	ret = clEnqueueReadBuffer(command_queue, out, CL_TRUE, 0,
	fileInfo.lenght* sizeof(byte),outputBuffer, 0, NULL, NULL);

	output = outputBuffer;

	finalizeExecution();

}

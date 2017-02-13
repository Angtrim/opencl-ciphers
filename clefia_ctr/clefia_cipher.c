#include "clefia_cipher.h"

#define MAX_SOURCE_SIZE (0x100000)

/*
   This function adds two string pointers together
*/
static char* stradd(const char* a, const char* b){
	size_t len = strlen(a) + strlen(b);
	char *ret = (char*)malloc(len * sizeof(char) + 1);
	*ret = '\0';
	return strcat(strcat(ret, a) ,b);
} 

static void loadClProgramSource(){
	/* Load the source code containing the kernel*/
	fp = fopen(clFileName, "r");
	if (!fp) {
	fprintf(stderr, "Failed to load kernel.\n");
	exit(1);
	}
	source_str = (char*)malloc(MAX_SOURCE_SIZE);
	fread(source_str, 1, MAX_SOURCE_SIZE, fp);
	fclose(fp);
}

static void writeOutputToFile(char* outFileName,char* output, long lenght){
	fp = fopen(outFileName, "wb");
	if (!fp) {
	fprintf(stderr, "Failed to load kernel.\n");
	exit(1);
	}
	fwrite(output, sizeof(char), lenght, fp);
	fclose(fp);
}

static void setUpOpenCl(byte* inputText, char* kernelName, uint8_t* key, int R, char* source_str, long source_size, long bufferLenght){
	
	/* Get Platform and Device Info */
	ret = clGetPlatformIDs(1, &platform_id, &ret_num_platforms);
	// allocate memory, get list of platforms
  	cl_platform_id *platforms = (cl_platform_id *) malloc(ret_num_platforms*sizeof(platform_id));

   	clGetPlatformIDs(ret_num_platforms, platforms, NULL);

	// iterate over platforms
	for (cl_uint i = 0; i < ret_num_platforms; ++i)
	{
		ret = clGetDeviceIDs(platforms[i], device_type, 1, &device_id, &ret_num_devices);
		if(ret == CL_SUCCESS){
			if(device_type == CL_DEVICE_TYPE_CPU){
				printf("\nCPU DEVICE FOUND\n");			
			}
			else {
				printf("\nGPU DEVICE FOUND\n");
			}	
		}   
	}

	free(platforms);

	/* Create OpenCL context */
	context = clCreateContext(NULL, 1, &device_id, NULL, NULL, &ret);
	if(ret != CL_SUCCESS){
		printf("Failed to create context\n");
	}

	/* Create Command Queue */
	command_queue = clCreateCommandQueue(context, device_id, CL_QUEUE_PROFILING_ENABLE, &ret);
	if(ret != CL_SUCCESS){
		printf("Failed to create command queue\n");
	}

	/* Create Memory Buffers */
	rk = clCreateBuffer(context, CL_MEM_READ_WRITE, (8 * 26 + 16) * sizeof(uint8_t), NULL, &ret); 
	in = clCreateBuffer(context, CL_MEM_READ_WRITE, bufferLenght * sizeof(uint8_t), NULL, &ret);
	out = clCreateBuffer(context, CL_MEM_READ_WRITE, bufferLenght * sizeof(uint8_t), NULL, &ret);
	r = clCreateBuffer(context, CL_MEM_READ_WRITE, sizeof(int), NULL, &ret);


	/* Copy input data to Memory Buffer */
	ret = clEnqueueWriteBuffer(command_queue, rk, CL_TRUE, 0, (8 * 26 + 16) * sizeof(uint8_t), rk, 0, NULL, NULL);
	ret = clEnqueueWriteBuffer(command_queue, in, CL_TRUE, 0, bufferLenght * sizeof(uint8_t), inputText, 0, NULL, NULL);
	ret = clEnqueueWriteBuffer(command_queue, r, CL_TRUE, 0, sizeof(int), R, 0, NULL, NULL);

	/* Create Kernel Program from the source */
	program = clCreateProgramWithSource(context, 1, (const char **)&source_str,
	(const size_t *)&source_size, &ret);
	if(ret != CL_SUCCESS){
		printf("Failed to create program with source\n");
	}
	
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
	kernel = clCreateKernel(program, kernelName, &ret);
	if(ret != CL_SUCCESS){
		printf("Failed to create kernel error: %d\n", ret);
	}

	/* Set OpenCL Kernel Parameters */
	ret = clSetKernelArg(kernel, 0, sizeof(cl_mem), (void *)&in);
	ret = clSetKernelArg(kernel, 1, sizeof(cl_mem), (void *)&rk);
	ret = clSetKernelArg(kernel, 2, sizeof(cl_mem), (void *)&out);
	ret = clSetKernelArg(kernel, 3, sizeof(cl_mem), (void *)&r);
}

static void finalizeExecution(char* source_str){
	printf("Releasing resources..\n");
	/* Finalization */
	ret = clFlush(command_queue);
	ret = clFinish(command_queue);
	ret = clReleaseKernel(kernel);
	ret = clReleaseProgram(program);
	ret = clReleaseMemObject(in);
	ret = clReleaseMemObject(rk);
	ret = clReleaseMemObject(r);
	ret = clReleaseMemObject(out);
	ret = clReleaseCommandQueue(command_queue);
	ret = clReleaseContext(context);
	//free(source_str);
}

/* Selecting the device */
static void setDeviceType(char* deviceType){

	if(strcmp(deviceType,"CPU") == 0)
		device_type = CL_DEVICE_TYPE_CPU;
	else if(strcmp(deviceType, "GPU") == 0)
		device_type = CL_DEVICE_TYPE_GPU;
}

uint8_t* clefia_encryption(char* fileName, uint8_t* key, char* outFileName,size_t local_item_size,int mode, int isCtr){

	struct FileInfo fileInfo = getFileBytes(fileName);
    
 	uint8_t* inputText = fileInfo.filePointer;

	//number of blocks 
        long lenght = fileInfo.lenght;
    
 	// load program source to build the kernel program
 	if(source_str == NULL){
		loadClProgramSource();
	}
    
	long source_size = strlen(source_str);

	// Key expansion is performed on CPU
	uint8_t rk[8 * 26 + 16];
	int R = 0;
	switch(mode){
	case 128:
		Clefia128KeySet(rk, key);
		R = 18;
		break;
	case 192:
		Clefia192KeySet(rk, key);
		R = 22;
		break;
	case 256:
		Clefia256KeySet(rk, key);
		R = 26;
		break;	
	}
	
	char* modality;

	if(isCtr){
		modality = "clefiaCtrCipher";
	}else{
		modality = "clefiaCipher";
	}
	
	setUpOpenCl(inputText, modality, rk, R, source_str, source_size, lenght);
	
	size_t global_item_size = lenght/16;
	/* Execute OpenCL Kernel instances */
	ret = clEnqueueNDRangeKernel(command_queue, kernel, 1, NULL, &global_item_size, &local_item_size, 0, NULL, &event);

	/* compute execution time */
	double total_time;
	clGetEventProfilingInfo(event, CL_PROFILING_COMMAND_START, sizeof(time_start), &time_start, NULL);
        clGetEventProfilingInfo(event, CL_PROFILING_COMMAND_END, sizeof(time_end), &time_end, NULL);
        total_time = time_end-time_start;

        printf("OpenCl Execution time is: %0.3f ms\n",total_time/1000000.0);

	/* Copy results from the memory buffer */
	uint8_t* output = (byte*)malloc((fileInfo.lenght+1)*sizeof(byte)); // Enough memory for file + \0
	
	ret = clEnqueueReadBuffer(command_queue, out, CL_TRUE, 0,
	fileInfo.lenght * sizeof(uint8_t),output, 0, NULL, NULL);
	
	finalizeExecution(source_str);
	
	return output;
}

uint8_t* clefia_128_Encrypt(char* fileName, uint8_t* key, char* outFileName, size_t local_item_size, char* deviceType){

	setDeviceType(deviceType);

	int mode = 128;	
	return clefia_encryption(fileName, key, outFileName, local_item_size, mode, 0);
}

uint8_t* clefia_192_Encrypt(char* fileName, uint8_t* key, char* outFileName, size_t local_item_size, char* deviceType){

	setDeviceType(deviceType);

	int mode = 192;	
	return clefia_encryption(fileName, key, outFileName, local_item_size, mode, 0);
}

uint8_t* clefia_256_Encrypt(char* fileName, uint8_t* key, char* outFileName, size_t local_item_size, char* deviceType){

	setDeviceType(deviceType);

	int mode = 256;	
	return clefia_encryption(fileName, key, outFileName, local_item_size, mode, 0);
}

uint8_t* clefia_128_CtrEncrypt(char* fileName, uint8_t* key, char* outFileName, size_t local_item_size, char* deviceType){

	setDeviceType(deviceType);
	
	int mode = 128;
	return clefia_encryption(fileName, key, outFileName, local_item_size, mode, 1);
}

uint8_t* clefia_192_CtrEncrypt(char* fileName, uint8_t* key, char* outFileName, size_t local_item_size, char* deviceType){

	setDeviceType(deviceType);
	
	int mode = 192;
	return clefia_encryption(fileName, key, outFileName, local_item_size, mode, 1);
}

uint8_t* clefia_256_CtrEncrypt(char* fileName, uint8_t* key, char* outFileName, size_t local_item_size, char* deviceType){

	setDeviceType(deviceType);
	
	int mode = 256;
	return clefia_encryption(fileName, key, outFileName, local_item_size, mode, 1);
}
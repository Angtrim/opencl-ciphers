#include "present_cipher.h"

#define MAX_SOURCE_SIZE (0x100000)

/* Function to load cl source */
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
/* set up the opencl parameters */
static void setUpOpenCl(uint64_t* inputText, uint64_t* SK, char* kernelName, char* source_str, long source_size, long bufferLenght){
	/* Get Platform and Device Info */
	ret = clGetPlatformIDs(1, &platform_id, &ret_num_platforms);
	ret = clGetDeviceIDs(platform_id, CL_DEVICE_TYPE_GPU, 1, &device_id, &ret_num_devices);

	/* Create OpenCL context */
	context = clCreateContext(NULL, 1, &device_id, NULL, NULL, &ret);
	if(ret != CL_SUCCESS){
		printf("failed to create context\n");
	}

	/* Create Command Queue */
	command_queue = clCreateCommandQueue(context, device_id, 0, &ret);
	if(ret != CL_SUCCESS){
		printf("failed to create commandqueue\n");
	}

	/* Create Memory Buffers */
	in = clCreateBuffer(context, CL_MEM_READ_WRITE, bufferLenght * sizeof(uint64_t), NULL, &ret);
	_SK = clCreateBuffer(context, CL_MEM_READ_WRITE, 32 * sizeof(uint64_t), NULL, &ret); 
        out = clCreateBuffer(context, CL_MEM_READ_WRITE, bufferLenght * sizeof(uint64_t), NULL, &ret);


	/* Copy input data to Memory Buffer */
	ret = clEnqueueWriteBuffer(command_queue, in, CL_TRUE, 0, bufferLenght * sizeof(uint64_t), inputText, 0, NULL, NULL);
	ret = clEnqueueWriteBuffer(command_queue, _SK, CL_TRUE, 0, 32 * sizeof(uint64_t), SK, 0, NULL, NULL);
        if(ret != CL_SUCCESS){
        	printf("failed to create Ki\n");
        }

	/* Create Kernel Program from the source */
	program = clCreateProgramWithSource(context, 1, (const char **)&source_str,
	(const size_t *)&source_size, &ret);
	if(ret != CL_SUCCESS){
		printf("failed to create program with source\n");
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
		printf("failed to create kernel error: %d\n", ret);
	}

	/* Set OpenCL Kernel Parameters */
	ret = clSetKernelArg(kernel, 0, sizeof(cl_mem), (void *)&in);
	ret = clSetKernelArg(kernel, 1, sizeof(cl_mem), (void *)&_SK);
	ret = clSetKernelArg(kernel, 2, sizeof(cl_mem), (void *)&out);
}

static void finalizeExecution(char* source_str){
	/* Finalization */
	ret = clFlush(command_queue);
	ret = clFinish(command_queue);
	ret = clReleaseKernel(kernel);
	ret = clReleaseProgram(program);
	ret = clReleaseMemObject(in);
	ret = clReleaseMemObject(_SK);
	ret = clReleaseMemObject(out);
	ret = clReleaseCommandQueue(command_queue);
	ret = clReleaseContext(context);
	//free(source_str);
}

uint64_t* present_memory_encrypt(char* fileName, uint64_t* Key, char* outFileName, size_t local_item_size){
	
	struct FileInfo64 fileInfo = getFileUint64(fileName);

        //plaintext divided in blocks of uint64_t
        uint64_t* inputText = fileInfo.filePointer;
        //number of blocks 
        long lenght = fileInfo.lenght;
	
	uint64_t subkey[32];

        //key expansion is performed on the cpu
	present_memory_expandKey(Key, subkey);

	// load program source to build the kernel program
	if(source_str == NULL){
		loadClProgramSource();
	}

	long source_size = strlen(source_str);
        
        setUpOpenCl(inputText, subkey, "present_memoryCipher", source_str, source_size, lenght);

        size_t global_item_size = lenght;
	/* Execute OpenCL Kernel instances */
	ret = clEnqueueNDRangeKernel(command_queue, kernel, 1, NULL, &global_item_size, &local_item_size, 0, NULL, NULL);

	/* Copy results from the memory buffer */
	uint64_t* output = (uint64_t*)malloc((lenght+1)*sizeof(uint64_t)); // Enough memory for file + \0
	
	ret = clEnqueueReadBuffer(command_queue, out, CL_TRUE, 0,
	lenght * sizeof(uint64_t),output, 0, NULL, NULL);
	
	finalizeExecution(source_str);
	
	return output;
}

uint64_t* present_speed_encrypt(char* fileName, uint64_t* Key, char* outFileName, size_t local_item_size){

	struct FileInfo64 fileInfo = getFileUint64(fileName);

        //plaintext divided in blocks of uint64_t
        uint64_t* inputText = fileInfo.filePointer;
        //number of blocks 
        long lenght = fileInfo.lenght;
	
	uint64_t subkey[32];

        //key expansion is performed on the cpu
	present_memory_expandKey(Key, subkey);

	// load program source to build the kernel program
	if(source_str == NULL){
		loadClProgramSource();
	}

	long source_size = strlen(source_str);
        
        setUpOpenCl(inputText, subkey, "present_speedCipher", source_str, source_size, lenght);

        size_t global_item_size = lenght;
	/* Execute OpenCL Kernel instances */
	ret = clEnqueueNDRangeKernel(command_queue, kernel, 1, NULL, &global_item_size, &local_item_size, 0, NULL, NULL);

	/* Copy results from the memory buffer */
	uint64_t* output = (uint64_t*)malloc((lenght+1)*sizeof(uint64_t)); // Enough memory for file + \0
	
	ret = clEnqueueReadBuffer(command_queue, out, CL_TRUE, 0,
	lenght * sizeof(uint64_t),output, 0, NULL, NULL);
	
	finalizeExecution(source_str);
	
	return output;
}

uint64_t* present_memory_Ctrencrypt(char* fileName, uint64_t* Key, char* outFileName, size_t local_item_size){

	struct FileInfo64 fileInfo = getFileUint64(fileName);

        //plaintext divided in blocks of uint64_t
        uint64_t* inputText = fileInfo.filePointer;
        //number of blocks 
        long lenght = fileInfo.lenght;
	
	uint64_t subkey[32];

        //key expansion is performed on the cpu
	present_memory_expandKey(Key, subkey);

	// load program source to build the kernel program
	if(source_str == NULL){
		loadClProgramSource();
	}

	long source_size = strlen(source_str);
        
        setUpOpenCl(inputText, subkey, "present_memoryCtrCipher", source_str, source_size, lenght);

        size_t global_item_size = lenght;
	/* Execute OpenCL Kernel instances */
	ret = clEnqueueNDRangeKernel(command_queue, kernel, 1, NULL, &global_item_size, &local_item_size, 0, NULL, NULL);

	/* Copy results from the memory buffer */
	uint64_t* output = (uint64_t*)malloc((lenght+1)*sizeof(uint64_t)); // Enough memory for file + \0
	
	ret = clEnqueueReadBuffer(command_queue, out, CL_TRUE, 0,
	lenght * sizeof(uint64_t),output, 0, NULL, NULL);
	
	finalizeExecution(source_str);
	
	return output;
}

uint64_t* present_speed_Ctrencrypt(char* fileName, uint64_t* Key, char* outFileName, size_t local_item_size){

	struct FileInfo64 fileInfo = getFileUint64(fileName);

        //plaintext divided in blocks of uint64_t
        uint64_t* inputText = fileInfo.filePointer;
        //number of blocks 
        long lenght = fileInfo.lenght;
	
	uint64_t subkey[32];

        //key expansion is performed on the cpu
	present_memory_expandKey(Key, subkey);

	// load program source to build the kernel program
	if(source_str == NULL){
		loadClProgramSource();
	}

	long source_size = strlen(source_str);
        
        setUpOpenCl(inputText, subkey, "present_speedCtrCipher", source_str, source_size, lenght);

        size_t global_item_size = lenght;
	/* Execute OpenCL Kernel instances */
	ret = clEnqueueNDRangeKernel(command_queue, kernel, 1, NULL, &global_item_size, &local_item_size, 0, NULL, NULL);

	/* Copy results from the memory buffer */
	uint64_t* output = (uint64_t*)malloc((lenght+1)*sizeof(uint64_t)); // Enough memory for file + \0
	
	ret = clEnqueueReadBuffer(command_queue, out, CL_TRUE, 0,
	lenght * sizeof(uint64_t),output, 0, NULL, NULL);
	
	finalizeExecution(source_str);
	
	return output;
}

#include "seed_cipher.h"

#define MAX_SOURCE_SIZE (0x100000)

#define SEED_OLD "SEED_OLD"
#define SEED "SEED"

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
	strcat(source_str, "\0");
	fclose(fp);
}
/* set up the opencl parameters */
static void setUpOpenCl(uint64_t* inputText, uint32_t* Ki, char* kernelName, char* source_str, long source_size, long bufferLenght){
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
	in = clCreateBuffer(context, CL_MEM_READ_WRITE, bufferLenght * sizeof(uint64_t), NULL, &ret);
	_Ki = clCreateBuffer(context, CL_MEM_READ_WRITE, 32 * sizeof(uint32_t), NULL, &ret); 
        out = clCreateBuffer(context, CL_MEM_READ_WRITE, bufferLenght * sizeof(uint64_t), NULL, &ret);


	/* Copy input data to Memory Buffer */
	ret = clEnqueueWriteBuffer(command_queue, in, CL_TRUE, 0, bufferLenght * sizeof(uint64_t), inputText, 0, NULL, NULL);
	ret = clEnqueueWriteBuffer(command_queue, _Ki, CL_TRUE, 0, 32 * sizeof(uint32_t), Ki, 0, NULL, NULL);

	/* Create Kernel Program from the source */
	program = clCreateProgramWithSource(context, 1, (const char **)&source_str,
	(const size_t *)&source_size, &ret);
	if(ret != CL_SUCCESS){
		printf("Failed to create program with source\n");
	}
	
	/* Build Kernel Program */
	ret = clBuildProgram(program, 1, &device_id, NULL, NULL, NULL);
	if(ret != CL_SUCCESS){
		printf("Build Error = %i", ret);
		
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
	ret = clSetKernelArg(kernel, 0, sizeof(cl_mem), (void *)&out);
	ret = clSetKernelArg(kernel, 1, sizeof(cl_mem), (void *)&in);
	ret = clSetKernelArg(kernel, 2, sizeof(cl_mem), (void *)&_Ki);
}

static void finalizeExecution(char* source_str, uint64_t* inputText){
	printf("Releasing resources..\n");
	/* Finalization */
	ret = clFlush(command_queue);
	ret = clFinish(command_queue);
	ret = clReleaseKernel(kernel);
	ret = clReleaseProgram(program);
	ret = clReleaseMemObject(in);
	ret = clReleaseMemObject(_Ki);
	ret = clReleaseMemObject(out);
	ret = clReleaseCommandQueue(command_queue);
	ret = clReleaseContext(context);
	free(inputText);
	inputText = NULL;
	//free(source_str);
}

/* Selecting the device */
static void setDeviceType(char* deviceType){

	if(strcmp(deviceType,"CPU") == 0)
		device_type = CL_DEVICE_TYPE_CPU;
	else if(strcmp(deviceType, "GPU") == 0)
		device_type = CL_DEVICE_TYPE_GPU;
}

cl_event seed_encryption(char* fileName, uint32_t* Key, uint64_t* output, size_t local_item_size, int isCtr, char* encryptionType){
	
	struct FileInfo64 fileInfo = getFileUint64(fileName);

        //plaintext divided in blocks of uint64_t
        uint64_t* inputText = fileInfo.filePointer;
        //number of blocks 
        long lenght = fileInfo.lenght;
	
	uint32_t Ki[32];
	
	char* modality;

	//type of encryption
	if(strcmp(encryptionType, SEED_OLD) == 0){
		//key expansion is performed on the cpu
		seed_old_expandkey(Key, Ki);
		
		if(isCtr)
		modality = "seed_oldCtrCipher";
		else
		modality = "seed_oldCipher";
		
	} else if(strcmp(encryptionType, SEED) == 0){
		//key expansion is performed on the cpu
		seed_expandkey(Key, Ki);
		
		if(isCtr)
		modality = "seedCtrCipher";
		else
		modality = "seedCipher";
			
	}

	// load program source to build the kernel program
	if(source_str == NULL){
		loadClProgramSource();
	}

	long source_size = strlen(source_str);
        
        setUpOpenCl(inputText, Ki, modality, source_str, source_size, lenght);

        size_t global_item_size = lenght/2;
	/* Execute OpenCL Kernel instances */
	ret = clEnqueueNDRangeKernel(command_queue, kernel, 1, NULL, &global_item_size, &local_item_size, 0, NULL, &event);
	if(ret != CL_SUCCESS){
		printf("Failed to enqueue NDRangeKernel. Error code: %d", ret);	
	}

	clWaitForEvents(1, &event);
	clFinish(command_queue);

	/* Copy results from the memory buffer */
	ret = clEnqueueReadBuffer(command_queue, out, CL_TRUE, 0,
	lenght * sizeof(uint64_t),output, 0, NULL, NULL);
	
	finalizeExecution(source_str, inputText);
	
	return event;
}

cl_event seed_old_Encrypt(char* fileName, uint32_t* Key, uint64_t* output, size_t local_item_size, char* deviceType){

	setDeviceType(deviceType);

	return seed_encryption(fileName, Key, output, local_item_size, 0, SEED_OLD);
}

cl_event seed_Encrypt(char* fileName, uint32_t* Key, uint64_t* output, size_t local_item_size, char* deviceType){

	setDeviceType(deviceType);

	return seed_encryption(fileName, Key, output, local_item_size, 0, SEED);
}

cl_event seed_old_CtrEncrypt(char* fileName, uint32_t* Key, uint64_t* output, size_t local_item_size, char* deviceType){

	setDeviceType(deviceType);

	return seed_encryption(fileName, Key, output, local_item_size, 1, SEED_OLD);
}

cl_event seed_CtrEncrypt(char* fileName, uint32_t* Key, uint64_t* output, size_t local_item_size, char* deviceType){

	setDeviceType(deviceType);

	return seed_encryption( fileName, Key, output, local_item_size, 1, SEED);  
}

cl_event seed_old_CtrDecrypt(char* fileName, uint32_t* Key, uint64_t* output, size_t local_item_size, char* deviceType){

	setDeviceType(deviceType);

	return seed_encryption(fileName, Key, output, local_item_size, 1, SEED_OLD);
}

cl_event seed_CtrDecrypt(char* fileName, uint32_t* Key, uint64_t* output, size_t local_item_size, char* deviceType){

	setDeviceType(deviceType);

	return seed_encryption( fileName, Key, output, local_item_size, 1, SEED);  
}

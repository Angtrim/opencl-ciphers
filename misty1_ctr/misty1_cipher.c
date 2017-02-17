#include "misty1_cipher.h"

#define MAX_SOURCE_SIZE (0x1000000)

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
	source_size = fread(source_str, 1, MAX_SOURCE_SIZE, fp);
	fclose(fp);
}

static void setUpOpenCl(uint64_t* inputText,  char* kernelName,uint16_t* EK, long bufferLenght){
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
		printf("Failed to create commandqueue\n");
	}

	/* Create Memory Buffers */
	in = clCreateBuffer(context, CL_MEM_READ_WRITE, bufferLenght * sizeof(uint64_t), NULL, &ret);
	exKey = clCreateBuffer(context, CL_MEM_READ_WRITE, EX_KEY_SIZE * sizeof(uint16_t), NULL, &ret); 
	out = clCreateBuffer(context, CL_MEM_READ_WRITE, bufferLenght * sizeof(uint64_t), NULL, &ret);

	/* Copy input data to Memory Buffer */
	ret = clEnqueueWriteBuffer(command_queue, in, CL_TRUE, 0, bufferLenght * sizeof(uint64_t), inputText, 0, NULL, NULL);
	ret = clEnqueueWriteBuffer(command_queue, exKey, CL_TRUE, 0, EX_KEY_SIZE * sizeof(uint16_t), EK, 0, NULL, NULL);

	/* Create Kernel Program from the source */
	program = clCreateProgramWithSource(context, 1, (const char **)&source_str,
	(const size_t *)&source_size, &ret);
	if(ret != CL_SUCCESS){
		printf("Failed to create program with source\n");
	}
	
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
	ret = clSetKernelArg(kernel, 1, sizeof(cl_mem), (void *)&exKey);
	ret = clSetKernelArg(kernel, 2, sizeof(cl_mem), (void *)&out);
}

static void finalizeExecution(uint64_t* inputText){
	printf("Releasing resources..\n");
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

cl_event mEncript(char* fileName, uint8_t* key, uint64_t* output,size_t local_item_size,int isCtr) {

	
	struct FileInfo64 fileInfo = getFileUint64(fileName);
    
 	uint64_t* inputText = fileInfo.filePointer;
	//number of blocks 
        long lenght = fileInfo.lenght;
    
 	// load program source to build the kernel program
 	if(source_str == NULL){
		loadClProgramSource();
	}
    

	char* modality;

	if(isCtr){
		modality = "misty1CtrCipher";
	}else{
		modality = "misty1Cipher";
	}
	
	// Key expansion is performed on CPU
	uint16_t EK[32];
	misty1_expandkey(EK,key);
	
	setUpOpenCl(inputText, modality, EK, lenght);
	
	size_t global_item_size = lenght;
	/* Execute OpenCL Kernel instances */
	ret = clEnqueueNDRangeKernel(command_queue, kernel, 1, NULL, &global_item_size, &local_item_size, 0, NULL, &event);
	if(ret != CL_SUCCESS){
		printf("Failed to enqueue NDRangeKernel. Error code: %d", ret);	
	}

	clWaitForEvents(1, &event);
	clFinish(command_queue);

	/* Copy results from the memory buffer */
	ret = clEnqueueReadBuffer(command_queue, out, CL_TRUE, 0,
	fileInfo.lenght * sizeof(uint64_t),output, 0, NULL, NULL);
	
	finalizeExecution(inputText);
	
	return event;
}	


cl_event misty1Encrypt(char* fileName, uint8_t* key, uint64_t* output,size_t local_item_size, char* deviceType) {

	setDeviceType(deviceType);
	return mEncript(fileName,key,output,local_item_size,0);
}	

cl_event misty1CtrEncrypt(char* fileName, uint8_t* key, uint64_t* output,size_t local_item_size, char* deviceType) {
	
	setDeviceType(deviceType);	
	return mEncript(fileName,key,output,local_item_size,1);
}

cl_event misty1CtrDecrypt(char* fileName, uint8_t* key, uint64_t* output,size_t local_item_size, char* deviceType) {
	
	setDeviceType(deviceType);	
	return mEncript(fileName,key,output,local_item_size,1);
}	




#include "misty1_cipher.h"

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
	source_str = (char*)malloc(MAX_SOURCE_SIZE+1);
	fread(source_str, 1, MAX_SOURCE_SIZE, fp);
	strcat(source_str, "\0");
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



static void setUpOpenCl(byte* inputText,  char* kernelName,uint8_t* key, char* source_str, long source_size, long bufferLenght){
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
	exKey = clCreateBuffer(context, CL_MEM_READ_WRITE, EX_KEY_SIZE * sizeof(byte), NULL, &ret); 
	out = clCreateBuffer(context, CL_MEM_READ_WRITE, bufferLenght * sizeof(uint64_t), NULL, &ret);

	uint16_t EK[32];
	misty1_expandkey(EK,key);
	/* Copy input data to Memory Buffer */
	ret = clEnqueueWriteBuffer(command_queue, in, CL_TRUE, 0, bufferLenght * sizeof(byte), inputText, 0, NULL, NULL);
	ret = clEnqueueWriteBuffer(command_queue, exKey, CL_TRUE, 0, EX_KEY_SIZE * sizeof(word), EK, 0, NULL, NULL);

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

static void finalizeExecution(char* source_str){
	printf("Releasing the resources..\n");
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
	//free(source_str);
}

/* Selecting the device */
static void setDeviceType(char* deviceType){

	if(strcmp(deviceType,"CPU") == 0)
		device_type = CL_DEVICE_TYPE_CPU;
	else if(strcmp(deviceType, "GPU") == 0)
		device_type = CL_DEVICE_TYPE_GPU;
}

byte* mEncript(char* fileName, uint8_t* key, char* outFileName,size_t local_item_size,int isCtr) {

	
	struct FileInfo fileInfo = getFileBytes(fileName);
    
 	byte* inputText = fileInfo.filePointer;
    
 	// load program source to build the kernel program
 	if(source_str == NULL){
		loadClProgramSource();
	}
    
	long source_size = strlen(source_str);

	char* modality;

	if(isCtr){
		modality = "misty1Cipher";
	}else{
		modality = "misty1CtrCipher";
	}
	
	setUpOpenCl(inputText,modality,key,source_str,source_size,fileInfo.lenght);
	
	size_t global_item_size = fileInfo.lenght/BLOCK_SIZE;
	/* Execute OpenCL Kernel instances */
	ret = clEnqueueNDRangeKernel(command_queue, kernel, 1, NULL, &global_item_size, &local_item_size, 0, NULL, &event);

	clWaitForEvents(1, &event);
	clFinish(command_queue);
	
	/* compute execution time */
	double total_time;
	clGetEventProfilingInfo(event, CL_PROFILING_COMMAND_START, sizeof(time_start), &time_start, NULL);
        clGetEventProfilingInfo(event, CL_PROFILING_COMMAND_END, sizeof(time_end), &time_end, NULL);
        total_time = time_end-time_start;

        printf("OpenCl Execution time is: %0.3f ms\n",total_time/1000000.0);

	/* Copy results from the memory buffer */
	byte* output = (byte*)malloc((fileInfo.lenght+1)*sizeof(byte)); // Enough memory for file + \0
	
	ret = clEnqueueReadBuffer(command_queue, out, CL_TRUE, 0,
	fileInfo.lenght * sizeof(byte),output, 0, NULL, NULL);
	
	finalizeExecution(source_str);
	
	return output;
}	


byte* misty1Encrypt(char* fileName, uint8_t* key, char* outFileName,size_t local_item_size, char* deviceType) {

	setDeviceType(deviceType);
	return mEncript(fileName,key,outFileName,local_item_size,0);
}	

byte* misty1CtrEncrypt(char* fileName, uint8_t* key, char* outFileName,size_t local_item_size, char* deviceType) {
	
	setDeviceType(deviceType);	
	return mEncript(fileName,key,outFileName,local_item_size,1);
}	




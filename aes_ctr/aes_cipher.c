#include "aes_cipher.h"

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
	fprintf(stderr, "Failed to load program source\n");
	exit(1);
	}
	source_str = (char*)malloc(MAX_SOURCE_SIZE);
	ret = fread(source_str, 1, MAX_SOURCE_SIZE, fp);
	strcat(source_str, "\0");
	fclose(fp);
}

static char* setUpBuildOptions(int mode){
	char* res = "";
	switch(mode){
	case 128:
		return res = "-D Nb=4 -D Nr=10";
		break;
	case 192:
		return res = "-D Nb=4 -D Nr=12";
		break;
	case 256:
		return res = "-D Nb=4 -D Nr=14";
		break;
	}
	return res;
}

static void setUpOpenCl(byte* inputText, word* w, char* kernelName, char* source_str, long source_size, long exKeyDim, long bufferLenght, int mode){
	
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
	in = clCreateBuffer(context, CL_MEM_READ_WRITE, bufferLenght * sizeof(byte), NULL, &ret);
	if(ret != CL_SUCCESS){
		printf("cannot write in buffer error code: %d", ret);	
	}
	exKey = clCreateBuffer(context, CL_MEM_READ_WRITE, exKeyDim * sizeof(word), NULL, &ret); 
	out = clCreateBuffer(context, CL_MEM_READ_WRITE, bufferLenght * sizeof(byte), NULL, &ret);

	/* Copy input data to Memory Buffer */
	ret = clEnqueueWriteBuffer(command_queue, in, CL_TRUE, 0, bufferLenght * sizeof(byte), inputText, 0, NULL, NULL);
	if(ret != CL_SUCCESS){
		printf("\nthe problem is here %i", ret);	
	}
	ret = clEnqueueWriteBuffer(command_queue, exKey, CL_TRUE, 0, exKeyDim * sizeof(word), w, 0, NULL, NULL);

	/* Create Kernel Program from the source */
	program = clCreateProgramWithSource(context, 1, (const char **)&source_str,
	(const size_t *)&source_size, &ret);
	if(ret != CL_SUCCESS){
		printf("Failed to create program with source\n");
	}
	
	/* Build Kernel Program */
	char* buildOptions = setUpBuildOptions(mode);
	ret = clBuildProgram(program, 1, &device_id, buildOptions, NULL, NULL);
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

static void finalizeExecution(char* source_str, uint8_t* inputText){
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

cl_event aesEncrypt(char* fileName, word* key, uint8_t* output,size_t local_item_size, int mode, int isCtr) {

	int Nk;
	int Nr;
	int Nb;
	switch(mode){
		case 128:
			Nk = Nk128;
			Nr = Nr128;
			Nb = Nb128;
			break;
		case 192:
			Nk = Nk192;
			Nr = Nr192;
			Nb = Nb192;
			break;
		case 256:
			Nk = Nk256;
			Nr = Nr256;
			Nb = Nb256;
			break;
	}
	
	struct FileInfo fileInfo = getFileBytes(fileName);
    
 	byte* inputText = fileInfo.filePointer;

	long exKeyDim = Nb*(Nr+1);
	//key expansion is performed on cpu
	word w[exKeyDim];
	KeyExpansion(key, w, Nk, Nb, Nr);
    
    	// load program source to build the kernel program
 	if(source_str == NULL){
		loadClProgramSource();
	}
    
	long source_size = strlen(source_str);
	char* modality;

	if(isCtr){
		modality = "aesCipherCtr";
	}else{
		modality = "aesCipher";
	}
	
	setUpOpenCl(inputText, w, modality,source_str,source_size,exKeyDim,fileInfo.lenght,mode);
	
	size_t global_item_size = fileInfo.lenght/BLOCK_SIZE;
	/* Execute OpenCL Kernel instances */
	ret = clEnqueueNDRangeKernel(command_queue, kernel, 1, NULL, &global_item_size, &local_item_size, 0, NULL, &event);
	if(ret != CL_SUCCESS){
		printf("\nFailed to enqueue kernels %i\n", ret);
	}

	clWaitForEvents(1, &event);
	clFinish(command_queue);
	
	ret = clEnqueueReadBuffer(command_queue, out, CL_TRUE, 0,
	fileInfo.lenght * sizeof(byte),output, 0, NULL, NULL);
	
	finalizeExecution(source_str, inputText);

	return event;
}	


cl_event aes128Encrypt(char* fileName, word* key, uint8_t* output,size_t local_item_size, char* deviceType) {

	setDeviceType(deviceType);	
	
	int mode = 128;
	return aesEncrypt(fileName,key,output,local_item_size,mode,0);
}	

cl_event aes192Encrypt(char* fileName, word* key, uint8_t* output,size_t local_item_size, char* deviceType) {

	setDeviceType(deviceType);

	int mode = 192;
	return aesEncrypt(fileName,key,output,local_item_size,mode,0);
}	

cl_event aes256Encrypt(char* fileName, word* key, uint8_t* output,size_t local_item_size, char* deviceType) {

	setDeviceType(deviceType);

	int mode = 256;
	return aesEncrypt(fileName,key,output,local_item_size,mode,0);
}

cl_event aes128CtrEncrypt(char* fileName, word* key, uint8_t* output,size_t local_item_size, char* deviceType) {

	setDeviceType(deviceType);

	int mode = 128;
	return aesEncrypt(fileName,key,output,local_item_size,mode,1);
}	

cl_event aes192CtrEncrypt(char* fileName, word* key, uint8_t* output,size_t local_item_size, char* deviceType) {

	setDeviceType(deviceType);

	int mode = 192;
	return aesEncrypt(fileName,key,output,local_item_size,mode,1);
}	

cl_event aes256CtrEncrypt(char* fileName, word* key, uint8_t* output,size_t local_item_size, char* deviceType) {

	setDeviceType(deviceType);

	int mode = 256;
	return aesEncrypt(fileName,key,output,local_item_size,mode,1);
}

cl_event aes128CtrDecrypt(char* fileName, word* key, uint8_t* output,size_t local_item_size, char* deviceType) {

	setDeviceType(deviceType);

	int mode = 128;
	return aesEncrypt(fileName,key,output,local_item_size,mode,1);
}	

cl_event aes192CtrDecrypt(char* fileName, word* key, uint8_t* output,size_t local_item_size, char* deviceType) {

	setDeviceType(deviceType);

	int mode = 192;
	return aesEncrypt(fileName,key,output,local_item_size,mode,1);
}	

cl_event aes256CtrDecrypt(char* fileName, word* key, uint8_t* output,size_t local_item_size, char* deviceType) {

	setDeviceType(deviceType);

	int mode = 256;
	return aesEncrypt(fileName,key,output,local_item_size,mode,1);
}

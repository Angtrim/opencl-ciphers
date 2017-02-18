#include "des_cipher.h"


static void setUpOpenCl(byte* inputText, char* kernelName, des_context* K, long bufferLenght){
	
	/* Get Platform and Device Info */
	ret = clGetPlatformIDs(1, &platform_id, &ret_num_platforms);
	// allocate memory, get list of platforms
  	cl_platform_id *platforms = (cl_platform_id *) malloc(ret_num_platforms*sizeof(platform_id));

   	clGetPlatformIDs(ret_num_platforms, platforms, NULL);

	// iterate over platforms
	for (cl_uint i = 0; i < ret_num_platforms; ++i)
	{
		ret = clGetDeviceIDs(platforms[i], device_type, 1, &device_id, &ret_num_devices);  
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
	_esk = clCreateBuffer(context, CL_MEM_READ_WRITE, 32*sizeof(uint32_t), NULL, &ret); 
	in = clCreateBuffer(context, CL_MEM_READ_WRITE, bufferLenght * sizeof(uint8_t), NULL, &ret);
	out = clCreateBuffer(context, CL_MEM_READ_WRITE, bufferLenght * sizeof(uint8_t), NULL, &ret);


	/* Copy input data to Memory Buffer */
	ret = clEnqueueWriteBuffer(command_queue, _esk, CL_TRUE, 0, 32*sizeof(uint32_t), K->esk, 0, NULL, NULL);
	ret = clEnqueueWriteBuffer(command_queue, in, CL_TRUE, 0, bufferLenght * sizeof(uint8_t), inputText, 0, NULL, NULL);

	/* Create Kernel Program from the source */
	program = clCreateProgramWithSource(context, 1, (const char **)&source_str,
	(const size_t *)&source_size, &ret);
	if(ret != CL_SUCCESS){
		printf("Failed to create program with source\n");
	}
	
	/* Build Kernel Program */
	ret = clBuildProgram(program, 1, &device_id, NULL, NULL, NULL);
	if(ret != CL_SUCCESS){
			logBuildError(&ret,&program,&device_id);
	}
	
	/* Create OpenCL Kernel */
	kernel = clCreateKernel(program, kernelName, &ret);
	if(ret != CL_SUCCESS){
		printf("Failed to create kernel error: %d\n", ret);
	}

	/* Set OpenCL Kernel Parameters */
	ret = clSetKernelArg(kernel, 0, sizeof(cl_mem), (void *)&_esk);
	ret = clSetKernelArg(kernel, 1, sizeof(cl_mem), (void *)&in);
	ret = clSetKernelArg(kernel, 2, sizeof(cl_mem), (void *)&out);
}

static void setUpOpenCl_2_3(uint8_t* inputText, char* kernelName, des3_context* K, long bufferLenght){

	/* Get Platform and Device Info */
	ret = clGetPlatformIDs(1, &platform_id, &ret_num_platforms);
	// allocate memory, get list of platforms
  	cl_platform_id *platforms = (cl_platform_id *) malloc(ret_num_platforms*sizeof(platform_id));

   	clGetPlatformIDs(ret_num_platforms, platforms, NULL);

	// iterate over platforms
	for (cl_uint i = 0; i < ret_num_platforms; ++i)
	{
		ret = clGetDeviceIDs(platforms[i], device_type, 1, &device_id, &ret_num_devices);   
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
	_esk = clCreateBuffer(context, CL_MEM_READ_WRITE, sizeof(uint32_t)*96, NULL, &ret); 
	in = clCreateBuffer(context, CL_MEM_READ_WRITE, bufferLenght * sizeof(uint8_t), NULL, &ret);
	out = clCreateBuffer(context, CL_MEM_READ_WRITE, bufferLenght * sizeof(uint8_t), NULL, &ret);


	/* Copy input data to Memory Buffer */
	ret = clEnqueueWriteBuffer(command_queue, _esk, CL_TRUE, 0, sizeof(uint32_t)*96, K->esk, 0, NULL, NULL);
	ret = clEnqueueWriteBuffer(command_queue, in, CL_TRUE, 0, bufferLenght * sizeof(uint8_t), inputText, 0, NULL, NULL);

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
	ret = clSetKernelArg(kernel, 0, sizeof(cl_mem), (void *)&_esk);
	ret = clSetKernelArg(kernel, 1, sizeof(cl_mem), (void *)&in);
	ret = clSetKernelArg(kernel, 2, sizeof(cl_mem), (void *)&out);
}

static void finalizeExecution( uint8_t* inputText){
	/* Finalization */
	ret = clFlush(command_queue);
	ret = clFinish(command_queue);
	ret = clReleaseKernel(kernel);
	ret = clReleaseProgram(program);
	ret = clReleaseMemObject(in);
	ret = clReleaseMemObject(_esk);
	ret = clReleaseMemObject(out);
	ret = clReleaseCommandQueue(command_queue);
	ret = clReleaseContext(context);
	free(inputText);
	inputText = NULL;
	free(source_str);
	source_str = NULL;
}

cl_event des_encryption(char* fileName, uint8_t* key, uint8_t* output,size_t local_item_size, int mode, int isCtr) {

	
	struct FileInfo fileInfo = getFileBytes(fileName);
    
 	uint8_t* inputText = fileInfo.filePointer;

 if(source_str == NULL){
		loadClProgramSource(clFileName,&source_str,&source_size);
	}
    
	char* modality;
	if(isCtr){
		if(mode == 1){
			modality = "desCtrCipher";
		}else{
			modality = "des3CtrCipher";
		}
	}else{
		if(mode == 1){
			modality = "desCipher";

		}else{
			modality = "des3Cipher";
		}
	}

	des_context K;
	des3_context _K;
	// Key expansion is performed on CPU
	switch(mode){
	case 1:
		des_expandkey(&K, key);
		setUpOpenCl(inputText, modality, &K, fileInfo.lenght);
		break;
	case 2:
		tdes2_expandkey(&_K, key);
		setUpOpenCl_2_3(inputText, modality, &_K, fileInfo.lenght);
		break;
	case 3: 
		tdes3_expandkey(&_K, key);
		setUpOpenCl_2_3(inputText, modality, &_K, fileInfo.lenght);
		break;	
	}
	
	size_t global_item_size = fileInfo.lenght/BLOCK_DIMEN;
	/* Execute OpenCL Kernel instances */
	ret = clEnqueueNDRangeKernel(command_queue, kernel, 1, NULL, &global_item_size, &local_item_size, 0, NULL, &event);
	if(ret != CL_SUCCESS){
		printf("Failed to enqueue NDRangeKernel. Error code: %d", ret);	
	}

	clWaitForEvents(1, &event);
	clFinish(command_queue);

	/* Copy results from the memory buffer */
	ret = clEnqueueReadBuffer(command_queue, out, CL_TRUE, 0,
	fileInfo.lenght * sizeof(uint8_t),output, 0, NULL, NULL);
	
	finalizeExecution(inputText);
	
	return event;
}	

cl_event desEncrypt(char* fileName, uint8_t* key, uint8_t* output,size_t local_item_size, char* deviceType) {

	setDeviceType(deviceType,&device_type);

	int mode = 1;
	return des_encryption(fileName,key,output,local_item_size,mode,0);
}

cl_event des2Encrypt(char* fileName, uint8_t* key, uint8_t* output,size_t local_item_size, char* deviceType) {

	setDeviceType(deviceType,&device_type);

	int mode = 2;
	return des_encryption(fileName,key,output,local_item_size,mode,0);
}	

cl_event des3Encrypt(char* fileName, uint8_t* key, uint8_t* output,size_t local_item_size, char* deviceType) {

	setDeviceType(deviceType,&device_type);

	int mode = 3;
	return des_encryption(fileName,key,output,local_item_size,mode,0);
}

cl_event desCtrEncrypt(char* fileName, uint8_t* key, uint8_t* output,size_t local_item_size, char* deviceType) {

	setDeviceType(deviceType,&device_type);		

	int mode = 1;
	return des_encryption(fileName,key,output,local_item_size,mode,1);
}

cl_event des2CtrEncrypt(char* fileName, uint8_t* key, uint8_t* output,size_t local_item_size, char* deviceType) {

	setDeviceType(deviceType,&device_type);		

	int mode = 2;
	return des_encryption(fileName,key,output,local_item_size,mode,1);
}	

cl_event des3CtrEncrypt(char* fileName, uint8_t* key, uint8_t* output,size_t local_item_size, char* deviceType) {

	setDeviceType(deviceType,&device_type);	
	
	int mode = 3;
	return des_encryption(fileName,key,output,local_item_size,mode,1);
}

cl_event desCtrDecrypt(char* fileName, uint8_t* key, uint8_t* output,size_t local_item_size, char* deviceType) {

	setDeviceType(deviceType,&device_type);		

	int mode = 1;
	return des_encryption(fileName,key,output,local_item_size,mode,1);
}

cl_event des2CtrDecrypt(char* fileName, uint8_t* key, uint8_t* output,size_t local_item_size, char* deviceType) {

	setDeviceType(deviceType,&device_type);		

	int mode = 2;
	return des_encryption(fileName,key,output,local_item_size,mode,1);
}	

cl_event des3CtrDecrypt(char* fileName, uint8_t* key, uint8_t* output,size_t local_item_size, char* deviceType) {

	setDeviceType(deviceType,&device_type);	
	
	int mode = 3;
	return des_encryption(fileName,key,output,local_item_size,mode,1);
}

#include "clefia_cipher.h"


static void setUpOpenCl(uint8_t* inputText, char* kernelName, uint8_t* key, int R, int rk_dim,long bufferLenght){
	
	initClSetup(&device_id,&device_type,&context,&command_queue);

	/* Create Memory Buffers */
	_rk = clCreateBuffer(context, CL_MEM_READ_WRITE, rk_dim * sizeof(uint8_t), NULL, &ret);
	in = clCreateBuffer(context, CL_MEM_READ_WRITE, bufferLenght * sizeof(uint8_t), NULL, &ret);
	out = clCreateBuffer(context, CL_MEM_READ_WRITE, bufferLenght * sizeof(uint8_t), NULL, &ret);
	r = clCreateBuffer(context, CL_MEM_READ_WRITE, sizeof(int), NULL, &ret);
	

	/* Copy input data to Memory Buffer */
	ret = clEnqueueWriteBuffer(command_queue, _rk, CL_TRUE, 0, rk_dim * sizeof(uint8_t), key, 0, NULL, NULL);
	ret = clEnqueueWriteBuffer(command_queue, in, CL_TRUE, 0, bufferLenght * sizeof(uint8_t), inputText, 0, NULL, NULL);
	ret = clEnqueueWriteBuffer(command_queue, r, CL_TRUE, 0, sizeof(int), &R, 0, NULL, NULL);

	/* Create Kernel Program from the source */
	program = clCreateProgramWithSource(context, 1, (const char **)&source_str,
	(const size_t *)&source_size, &ret);
	if(ret != CL_SUCCESS){
		printf("Failed to create program with source\n");
	}
	
	/* Build Kernel Program */
	char* opt = "-cl-opt-disable";
	ret = clBuildProgram(program, 1, &device_id, opt, NULL, NULL);
	if(ret != CL_SUCCESS){
		logBuildError(&ret,&program,&device_id);
	}
	
	/* Create OpenCL Kernel */
	kernel = clCreateKernel(program, kernelName, &ret);
	if(ret != CL_SUCCESS){
		printf("Failed to create kernel error: %d\n", ret);
	}

	/* Set OpenCL Kernel Parameters */
	ret = clSetKernelArg(kernel, 0, sizeof(cl_mem), (void *)&out);
	ret = clSetKernelArg(kernel, 1, sizeof(cl_mem), (void *)&in);
	ret = clSetKernelArg(kernel, 2, sizeof(cl_mem), (void *)&_rk);
	ret = clSetKernelArg(kernel, 3, sizeof(cl_mem), (void *)&r);
}

static void finalizeExecution(uint8_t* inputText){
	/* Finalization */
	ret = clFlush(command_queue);
	ret = clFinish(command_queue);
	ret = clReleaseKernel(kernel);
	ret = clReleaseProgram(program);
	ret = clReleaseMemObject(in);
	ret = clReleaseMemObject(_rk);
	ret = clReleaseMemObject(r);
	ret = clReleaseMemObject(out);
	ret = clReleaseCommandQueue(command_queue);
	ret = clReleaseContext(context);
	free(inputText);
	inputText = NULL;
	free(source_str);
	source_str = NULL;
}

cl_event clefia_encryption(char* fileName, uint8_t* key, uint8_t* output,size_t local_item_size,int mode, int isCtr){

	struct FileInfo fileInfo = getFileBytes(fileName);
	
	uint8_t* inputText = fileInfo.filePointer;

	//number of blocks 
	long lenght = fileInfo.lenght;
	if(source_str == NULL){
		loadClProgramSource(clFileName,&source_str,&source_size);
	}

	// Key expansion is performed on CPU
	int rk_dim = 8 * 26 + 16;
	uint8_t rk[rk_dim];
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
	
	setUpOpenCl(inputText, modality, rk, R, rk_dim,lenght);

	size_t global_item_size = lenght/16;
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

cl_event clefia_128_Encrypt(char* fileName, uint8_t* key, uint8_t* output, size_t local_item_size, char* deviceType){

	setDeviceType(deviceType,&device_type);

	int mode = 128;	
	return clefia_encryption(fileName, key, output, local_item_size, mode, 0);
}

cl_event clefia_192_Encrypt(char* fileName, uint8_t* key, uint8_t* output, size_t local_item_size, char* deviceType){

	setDeviceType(deviceType,&device_type);

	int mode = 192;	
	return clefia_encryption(fileName, key, output, local_item_size, mode, 0);
}

cl_event clefia_256_Encrypt(char* fileName, uint8_t* key, uint8_t* output, size_t local_item_size, char* deviceType){

	setDeviceType(deviceType,&device_type);

	int mode = 256;	
	return clefia_encryption(fileName, key, output, local_item_size, mode, 0);
}

cl_event clefia_128_CtrEncrypt(char* fileName, uint8_t* key, uint8_t* output, size_t local_item_size, char* deviceType){

	setDeviceType(deviceType,&device_type);
	
	int mode = 128;
	return clefia_encryption(fileName, key, output, local_item_size, mode, 1);
}

cl_event clefia_192_CtrEncrypt(char* fileName, uint8_t* key, uint8_t* output, size_t local_item_size, char* deviceType){

	setDeviceType(deviceType,&device_type);
	
	int mode = 192;
	return clefia_encryption(fileName, key, output, local_item_size, mode, 1);
}

cl_event clefia_256_CtrEncrypt(char* fileName, uint8_t* key, uint8_t* output, size_t local_item_size, char* deviceType){

	setDeviceType(deviceType,&device_type);
	
	int mode = 256;
	return clefia_encryption(fileName, key, output, local_item_size, mode, 1);
}

cl_event clefia_128_CtrDecrypt(char* fileName, uint8_t* key, uint8_t* output, size_t local_item_size, char* deviceType){

	setDeviceType(deviceType,&device_type);
	
	int mode = 128;
	return clefia_encryption(fileName, key, output, local_item_size, mode, 1);
}

cl_event clefia_192_CtrDecrypt(char* fileName, uint8_t* key, uint8_t* output, size_t local_item_size, char* deviceType){

	setDeviceType(deviceType,&device_type);
	
	int mode = 192;
	return clefia_encryption(fileName, key, output, local_item_size, mode, 1);
}

cl_event clefia_256_CtrDecrypt(char* fileName, uint8_t* key, uint8_t* output, size_t local_item_size, char* deviceType){

	setDeviceType(deviceType,&device_type);
	
	int mode = 256;
	return clefia_encryption(fileName, key, output, local_item_size, mode, 1);
}

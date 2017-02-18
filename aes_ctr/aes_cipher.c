#include "aes_cipher.h"

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

static void setUpOpenCl(byte* inputText, word* w, char* kernelName,long exKeyDim, long bufferLenght, int mode){
	
 initClSetup(&device_id,&device_type,&context,&command_queue);
	/* Create Memory Buffers */
	in = clCreateBuffer(context, CL_MEM_READ_WRITE, bufferLenght * sizeof(byte), NULL, &ret);
	exKey = clCreateBuffer(context, CL_MEM_READ_WRITE, exKeyDim * sizeof(word), NULL, &ret); 
	out = clCreateBuffer(context, CL_MEM_READ_WRITE, bufferLenght * sizeof(byte), NULL, &ret);

	/* Copy input data to Memory Buffer */
	ret = clEnqueueWriteBuffer(command_queue, in, CL_TRUE, 0, bufferLenght * sizeof(byte), inputText, 0, NULL, NULL);
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
  logBuildError(&ret,&program,&device_id);
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

static void finalizeExecution(uint8_t* inputText){
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
	free(source_str);
	source_str = NULL;
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
		loadClProgramSource(clFileName,&source_str,&source_size);
	}

	char* modality;

	if(isCtr){
		modality = "aesCipherCtr";
	}else{
		modality = "aesCipher";
	}
	
	setUpOpenCl(inputText, w, modality,exKeyDim,fileInfo.lenght,mode);

	size_t global_item_size = fileInfo.lenght/BLOCK_SIZE;
	/* Execute OpenCL Kernel instances */
	ret = clEnqueueNDRangeKernel(command_queue, kernel, 1, NULL, &global_item_size, &local_item_size, 0, NULL, &event);
	if(ret != CL_SUCCESS){
		printf("Failed to enqueue NDRangeKernel. Error code: %d", ret);	
	}

	clWaitForEvents(1, &event);
	clFinish(command_queue);
	
	ret = clEnqueueReadBuffer(command_queue, out, CL_TRUE, 0,
	fileInfo.lenght * sizeof(byte),output, 0, NULL, NULL);
	
	finalizeExecution(inputText);

	return event;
}	


cl_event aes128Encrypt(char* fileName, word* key, uint8_t* output,size_t local_item_size, char* deviceType) {

	setDeviceType(deviceType,&device_type);	
	
	int mode = 128;
	return aesEncrypt(fileName,key,output,local_item_size,mode,0);
}	

cl_event aes192Encrypt(char* fileName, word* key, uint8_t* output,size_t local_item_size, char* deviceType) {

	setDeviceType(deviceType,&device_type);

	int mode = 192;
	return aesEncrypt(fileName,key,output,local_item_size,mode,0);
}	

cl_event aes256Encrypt(char* fileName, word* key, uint8_t* output,size_t local_item_size, char* deviceType) {

	setDeviceType(deviceType,&device_type);

	int mode = 256;
	return aesEncrypt(fileName,key,output,local_item_size,mode,0);
}

cl_event aes128CtrEncrypt(char* fileName, word* key, uint8_t* output,size_t local_item_size, char* deviceType) {

	setDeviceType(deviceType,&device_type);

	int mode = 128;
	return aesEncrypt(fileName,key,output,local_item_size,mode,1);
}	

cl_event aes192CtrEncrypt(char* fileName, word* key, uint8_t* output,size_t local_item_size, char* deviceType) {

	setDeviceType(deviceType,&device_type);

	int mode = 192;
	return aesEncrypt(fileName,key,output,local_item_size,mode,1);
}	

cl_event aes256CtrEncrypt(char* fileName, word* key, uint8_t* output,size_t local_item_size, char* deviceType) {

	setDeviceType(deviceType,&device_type);

	int mode = 256;
	return aesEncrypt(fileName,key,output,local_item_size,mode,1);
}

cl_event aes128CtrDecrypt(char* fileName, word* key, uint8_t* output,size_t local_item_size, char* deviceType) {

	setDeviceType(deviceType,&device_type);

	int mode = 128;
	return aesEncrypt(fileName,key,output,local_item_size,mode,1);
}	

cl_event aes192CtrDecrypt(char* fileName, word* key, uint8_t* output,size_t local_item_size, char* deviceType) {

	setDeviceType(deviceType,&device_type);

	int mode = 192;
	return aesEncrypt(fileName,key,output,local_item_size,mode,1);
}	

cl_event aes256CtrDecrypt(char* fileName, word* key, uint8_t* output,size_t local_item_size, char* deviceType) {

	setDeviceType(deviceType,&device_type);

	int mode = 256;
	return aesEncrypt(fileName,key,output,local_item_size,mode,1);
}

#include "misty1_cipher.h"



static void setUpOpenCl(uint64_t* inputText,  char* kernelName,uint16_t* EK, long bufferLenght){
	/* Get Platform and Device Info */
 initClSetup(&device_id,&device_type,&context,&command_queue);

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

static void finalizeExecution(uint64_t* inputText){
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

cl_event mEncript(char* fileName, uint8_t* key, uint64_t* output,size_t local_item_size,int isCtr) {

	
	struct FileInfo64 fileInfo = getFileUint64(fileName);
    
 	uint64_t* inputText = fileInfo.filePointer;
	//number of blocks 
        long lenght = fileInfo.lenght;
    
 if(source_str == NULL){
		loadClProgramSource(clFileName,&source_str,&source_size);
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

	setDeviceType(deviceType,&device_type);
	return mEncript(fileName,key,output,local_item_size,0);
}	

cl_event misty1CtrEncrypt(char* fileName, uint8_t* key, uint64_t* output,size_t local_item_size, char* deviceType) {
	
	setDeviceType(deviceType,&device_type);	
	return mEncript(fileName,key,output,local_item_size,1);
}

cl_event misty1CtrDecrypt(char* fileName, uint8_t* key, uint64_t* output,size_t local_item_size, char* deviceType) {
	
	setDeviceType(deviceType,&device_type);	
	return mEncript(fileName,key,output,local_item_size,1);
}	




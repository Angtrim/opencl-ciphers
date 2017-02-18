#include "present_cipher.h"


#define MEMORY "MEMORY"
#define SPEED "SPEED"


/* set up the opencl parameters */
static void setUpOpenCl(uint64_t* inputText, uint64_t* SK, char* kernelName,long bufferLenght){
 initClSetup(&device_id,&device_type,&context,&command_queue);

	/* Create Memory Buffers */
	in = clCreateBuffer(context, CL_MEM_READ_WRITE, bufferLenght * sizeof(uint64_t), NULL, &ret);
	if(ret != CL_SUCCESS){
        	printf("Failed to create in\n");
        }
	_SK = clCreateBuffer(context, CL_MEM_READ_WRITE, 32 * sizeof(uint64_t), NULL, &ret); 
        if(ret != CL_SUCCESS){
        	printf("Failed to create _SK\n");
        }
        out = clCreateBuffer(context, CL_MEM_READ_WRITE, bufferLenght * sizeof(uint64_t), NULL, &ret);
	if(ret != CL_SUCCESS){
        	printf("Failed to create out\n");
        }


	/* Copy input data to Memory Buffer */
	ret = clEnqueueWriteBuffer(command_queue, in, CL_TRUE, 0, bufferLenght * sizeof(uint64_t), inputText, 0, NULL, NULL);
	ret = clEnqueueWriteBuffer(command_queue, _SK, CL_TRUE, 0, 32 * sizeof(uint64_t), SK, 0, NULL, NULL);
   
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
	ret = clSetKernelArg(kernel, 0, sizeof(cl_mem), (void *)&in);
	ret = clSetKernelArg(kernel, 1, sizeof(cl_mem), (void *)&_SK);
	ret = clSetKernelArg(kernel, 2, sizeof(cl_mem), (void *)&out);
}

static void finalizeExecution(uint64_t* inputText){
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
	free(inputText);
	inputText = NULL;
	free(source_str);
	source_str = NULL;
}



cl_event present_encryption(char* fileName, uint64_t* Key, uint64_t* output, size_t local_item_size, char* encryptionType, int isCtr){
	
	struct FileInfo64 fileInfo = getFileUint64(fileName);

        //plaintext divided in blocks of uint64_t
        uint64_t* inputText = fileInfo.filePointer;
        //number of blocks 
        long lenght = fileInfo.lenght;
	
	uint64_t subkey[32];

	char* modality;

	//type of encryption
	if(strcmp(encryptionType, MEMORY) == 0){
		//key expansion is performed on the cpu
		present_memory_expandKey(Key, subkey);
		
		if(isCtr)
		modality = "present_memoryCtrCipher";
		else
		modality = "present_memoryCipher";
		
	} else if(strcmp(encryptionType, SPEED) == 0){
		//key expansion is performed on the cpu
		present_speed_expandKey(Key, subkey);
		
		if(isCtr)
		modality = "present_speedCtrCipher";
		else
		modality = "present_speedCipher";
			
	}

 if(source_str == NULL){
		loadClProgramSource(clFileName,&source_str,&source_size);
	}

        
        setUpOpenCl(inputText, subkey, modality,lenght);


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
	lenght * sizeof(uint64_t),output, 0, NULL, NULL);
	
	finalizeExecution(inputText);
	
	return event;
}

cl_event present_memory_Encrypt(char* fileName, uint64_t* Key, uint64_t* output, size_t local_item_size, char* deviceType){
	
	setDeviceType(deviceType,&device_type);
	
	return present_encryption(fileName, Key, output, local_item_size, MEMORY, 0);
}

cl_event present_speed_Encrypt(char* fileName, uint64_t* Key, uint64_t* output, size_t local_item_size, char* deviceType){

	setDeviceType(deviceType,&device_type);

	return present_encryption(fileName, Key, output, local_item_size, SPEED, 0);
}

cl_event present_memory_CtrEncrypt(char* fileName, uint64_t* Key, uint64_t* output, size_t local_item_size, char* deviceType){

	setDeviceType(deviceType,&device_type);
	
	return present_encryption(fileName, Key, output, local_item_size, MEMORY, 1);
}

cl_event present_speed_CtrEncrypt(char* fileName, uint64_t* Key, uint64_t* output, size_t local_item_size, char* deviceType){

	setDeviceType(deviceType,&device_type);

	return present_encryption(fileName, Key, output, local_item_size, SPEED, 1);
}

cl_event present_memory_CtrDecrypt(char* fileName, uint64_t* Key, uint64_t* output, size_t local_item_size, char* deviceType){

	setDeviceType(deviceType,&device_type);
	
	return present_encryption(fileName, Key, output, local_item_size, MEMORY, 1);
}

cl_event present_speed_CtrDecrypt(char* fileName, uint64_t* Key, uint64_t* output, size_t local_item_size, char* deviceType){

	setDeviceType(deviceType,&device_type);

	return present_encryption(fileName, Key, output, local_item_size, SPEED, 1);
}

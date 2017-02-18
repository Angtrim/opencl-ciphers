
#include "camellia_cipher.h"

#define MASK8   0xff
#define MASK32  0xffffffffu


/* set up the opencl parameters */
static void setUpOpenCl(uint64_t* inputText, uint64_t* k, uint64_t* ke, uint64_t* kw, char* kernelName, int kdim, int kedim, int kwdim, long bufferLenght){
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
	in = clCreateBuffer(context, CL_MEM_READ_WRITE, bufferLenght * sizeof(uint64_t), NULL, &ret);
	_k = clCreateBuffer(context, CL_MEM_READ_WRITE, kdim * sizeof(uint64_t), NULL, &ret); 
	_ke = clCreateBuffer(context, CL_MEM_READ_WRITE, kedim * sizeof(uint64_t), NULL, &ret);
        _kw = clCreateBuffer(context, CL_MEM_READ_WRITE, kwdim * sizeof(uint64_t), NULL, &ret);
        out = clCreateBuffer(context, CL_MEM_READ_WRITE, bufferLenght * sizeof(uint64_t), NULL, &ret);


	/* Copy input data to Memory Buffer */
	ret = clEnqueueWriteBuffer(command_queue, in, CL_TRUE, 0, bufferLenght * sizeof(uint64_t), inputText, 0, NULL, NULL);
	ret = clEnqueueWriteBuffer(command_queue, _k, CL_TRUE, 0, kdim * sizeof(uint64_t), k, 0, NULL, NULL);
        ret = clEnqueueWriteBuffer(command_queue, _ke, CL_TRUE, 0, kedim * sizeof(uint64_t), ke, 0, NULL, NULL);
        ret = clEnqueueWriteBuffer(command_queue, _kw, CL_TRUE, 0, kwdim * sizeof(uint64_t), kw, 0, NULL, NULL);

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
	ret = clSetKernelArg(kernel, 1, sizeof(cl_mem), (void *)&_kw);
	ret = clSetKernelArg(kernel, 2, sizeof(cl_mem), (void *)&_k);
        ret = clSetKernelArg(kernel, 3, sizeof(cl_mem), (void *)&_ke);
        ret = clSetKernelArg(kernel, 4, sizeof(cl_mem), (void *)&out);
}

static void finalizeExecution(uint64_t* inputText){
	/* Finalization */
	ret = clFlush(command_queue);
	ret = clFinish(command_queue);
	ret = clReleaseKernel(kernel);
	ret = clReleaseProgram(program);
	ret = clReleaseMemObject(in);
	ret = clReleaseMemObject(_k);
 ret = clReleaseMemObject(_ke);
	ret = clReleaseMemObject(_kw);
	ret = clReleaseMemObject(out);
	ret = clReleaseCommandQueue(command_queue);
	ret = clReleaseContext(context);
	free(inputText);
	inputText = NULL;
	free(source_str);
	source_str = NULL;

}


cl_event camellia_encryption(char* fileName, uint64_t* K, uint64_t* output, size_t local_item_size, int mode, int isCtr){
	
	struct FileInfo64 fileInfo = getFileUint64(fileName);
        
        //plaintext divided in blocks of uint64_t
        uint64_t* inputText = fileInfo.filePointer;
        //number of blocks 
        long lenght = fileInfo.lenght;

	int kdim;
        int kedim;
        int kwdim;

	char* modality;

	switch(mode){
	case 128:
		kdim = 18;
		kedim = 4;
		kwdim = 4;
		
		if(isCtr)
		modality = "camellia128CtrCipher";
		else
		modality = "camellia128Cipher";
		break;	
	case 192:
		kdim = 24;
		kedim = 6;
		kwdim = 4;
		
		if(isCtr)
		modality = "camellia192256CtrCipher";
		else
		modality = "camellia192256Cipher";
		break;	
	case 256:
		kdim = 24;
		kedim = 6;
		kwdim = 4;
		
		if(isCtr)
		modality = "camellia192256CtrCipher";
		else
		modality = "camellia192256Cipher";
		break;				
	}

        

	uint64_t k[kdim];
	uint64_t ke[kedim];
	uint64_t kw[kwdim];

	//key expansion is performed on cpu
	switch(mode){
	case 128:
		camellia_128_expandkey(K, kw, k, ke);
		break;
	case 192:
		camellia_192_expandkey(K, kw, k, ke);
		break;
	case 256:
		camellia_256_expandkey(K, kw, k, ke);
		break;
	}
	

 if(source_str == NULL){
		loadClProgramSource(clFileName,&source_str,&source_size);
	}
 setUpOpenCl(inputText, k, ke, kw, modality,kdim, kedim, kwdim, lenght);

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

	finalizeExecution(inputText);

	return event;
}

cl_event camellia128Encrypt(char* fileName, uint64_t* K, uint64_t* output, size_t local_item_size, char* deviceType){

	setDeviceType(deviceType,&device_type);

        int mode = 128;
	return camellia_encryption(fileName, K, output, local_item_size, mode, 0);
}

cl_event camellia192Encrypt(char* fileName, uint64_t* K, uint64_t* output, size_t local_item_size, char* deviceType){

	setDeviceType(deviceType,&device_type);
        
        int mode = 192;
	return camellia_encryption(fileName, K, output, local_item_size, mode, 0);
}

cl_event camellia256Encrypt(char* fileName, uint64_t* K, uint64_t* output, size_t local_item_size, char* deviceType){

	setDeviceType(deviceType,&device_type);

        int mode = 256;
	return camellia_encryption(fileName, K, output, local_item_size, mode, 0);
}

cl_event camelliaCtr128Encrypt(char* fileName, uint64_t* K, uint64_t* output, size_t local_item_size, char* deviceType){

	setDeviceType(deviceType,&device_type);

	int mode = 128;
	return camellia_encryption(fileName, K, output, local_item_size, mode, 1);
}

cl_event camelliaCtr192Encrypt(char* fileName, uint64_t* K, uint64_t* output, size_t local_item_size, char* deviceType){

	setDeviceType(deviceType,&device_type);

        int mode = 192;
	return camellia_encryption(fileName, K, output, local_item_size, mode, 1);
}

cl_event camelliaCtr256Encrypt(char* fileName, uint64_t* K, uint64_t* output, size_t local_item_size, char* deviceType){

	setDeviceType(deviceType,&device_type);

 	int mode = 256;
	return camellia_encryption(fileName, K, output, local_item_size, mode, 1);       
}

cl_event camelliaCtr128Decrypt(char* fileName, uint64_t* K, uint64_t* output, size_t local_item_size, char* deviceType){

	setDeviceType(deviceType,&device_type);

	int mode = 128;
	return camellia_encryption(fileName, K, output, local_item_size, mode, 1);
}

cl_event camelliaCtr192Decrypt(char* fileName, uint64_t* K, uint64_t* output, size_t local_item_size, char* deviceType){

	setDeviceType(deviceType,&device_type);

        int mode = 192;
	return camellia_encryption(fileName, K, output, local_item_size, mode, 1);
}

cl_event camelliaCtr256Decrypt(char* fileName, uint64_t* K, uint64_t* output, size_t local_item_size, char* deviceType){

	setDeviceType(deviceType,&device_type);

 	int mode = 256;
	return camellia_encryption(fileName, K, output, local_item_size, mode, 1);       
}

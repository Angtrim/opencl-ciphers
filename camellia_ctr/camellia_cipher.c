
#include "camellia_cipher.h"

#define MASK8   0xff
#define MASK32  0xffffffffu

#define MAX_SOURCE_SIZE (0x100000)

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
	fclose(fp);
}
/* set up the opencl parameters */
static void setUpOpenCl(uint64_t* inputText, uint64_t* k, uint64_t* ke, uint64_t* kw, char* kernelName, char* source_str, long source_size, int kdim, int kedim, int kwdim, long bufferLenght){
	/* Get Platform and Device Info */
	ret = clGetPlatformIDs(1, &platform_id, &ret_num_platforms);
	ret = clGetDeviceIDs(platform_id, CL_DEVICE_TYPE_GPU, 1, &device_id, &ret_num_devices);

	/* Create OpenCL context */
	context = clCreateContext(NULL, 1, &device_id, NULL, NULL, &ret);
	if(ret != CL_SUCCESS){
		printf("failed to create context\n");
	}

	/* Create Command Queue */
	command_queue = clCreateCommandQueue(context, device_id, 0, &ret);
	if(ret != CL_SUCCESS){
		printf("failed to create commandqueue\n");
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
		printf("failed to create program with source\n");
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
		printf("failed to create kernel error: %d\n", ret);
	}

	/* Set OpenCL Kernel Parameters */
	ret = clSetKernelArg(kernel, 0, sizeof(cl_mem), (void *)&in);
	ret = clSetKernelArg(kernel, 1, sizeof(cl_mem), (void *)&_kw);
	ret = clSetKernelArg(kernel, 2, sizeof(cl_mem), (void *)&_k);
        ret = clSetKernelArg(kernel, 3, sizeof(cl_mem), (void *)&_ke);
        ret = clSetKernelArg(kernel, 4, sizeof(cl_mem), (void *)&out);
}

static void finalizeExecution(char* source_str){
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
	//free(source_str);
}

uint64_t* camellia128Encrypt(char* fileName, uint64_t* K, char* outFileName, size_t local_item_size){

        struct FileInfo64 fileInfo = getFileUint64(fileName);
        
        //plaintext divided in blocks of uint64_t
        uint64_t* inputText = fileInfo.filePointer;
        //number of blocks 
        long lenght = fileInfo.lenght;

        int kdim = 18;
        int kedim = 4;
        int kwdim = 4;

	uint64_t k[kdim];
	uint64_t ke[kedim];
	uint64_t kw[kwdim];

	//key expansion is performed on cpu
	camellia_128_expandkey(K, kw, k, ke);

	// load program source to build the kernel program
	if(source_str == NULL){
		loadClProgramSource();
	}

	long source_size = strlen(source_str);
        
        setUpOpenCl(inputText, k, ke, kw, "camellia128Cipher",source_str,source_size,kdim, kedim, kwdim, lenght);

        size_t global_item_size = lenght/2;
	/* Execute OpenCL Kernel instances */
	ret = clEnqueueNDRangeKernel(command_queue, kernel, 1, NULL, &global_item_size, &local_item_size, 0, NULL, NULL);

	/* Copy results from the memory buffer */
	uint64_t* output = (uint64_t*)malloc((lenght+1)*sizeof(uint64_t)); // Enough memory for file + \0
	
	ret = clEnqueueReadBuffer(command_queue, out, CL_TRUE, 0,
	lenght * sizeof(uint64_t),output, 0, NULL, NULL);
	
	finalizeExecution(source_str);
	
	return output;
}

uint64_t* camellia192Encrypt(char* fileName, uint64_t* K, char* outFileName, size_t local_item_size){
        
        struct FileInfo64 fileInfo = getFileUint64(fileName);
        
        //plaintext divided in blocks of uint64_t
        uint64_t* inputText = fileInfo.filePointer;
        //number of blocks 
        long lenght = fileInfo.lenght;

        int kdim = 24;
        int kedim = 6;
        int kwdim = 4;

	uint64_t k[kdim];
	uint64_t ke[kedim];
	uint64_t kw[kwdim];

	//key expansion is performed on cpu
	camellia_192_expandkey(K, kw, k, ke);

	// load program source to build the kernel program
	if(source_str == NULL){
		loadClProgramSource();
	}

	long source_size = strlen(source_str);
        
        setUpOpenCl(inputText, k, ke, kw, "camellia192256Cipher",source_str,source_size,kdim, kedim, kwdim, lenght);

        size_t global_item_size = lenght/2;
	/* Execute OpenCL Kernel instances */
	ret = clEnqueueNDRangeKernel(command_queue, kernel, 1, NULL, &global_item_size, &local_item_size, 0, NULL, NULL);

	/* Copy results from the memory buffer */
	uint64_t* output1 = (uint64_t*)malloc((lenght+1)*sizeof(uint64_t)); // Enough memory for file + \0
	
	ret = clEnqueueReadBuffer(command_queue, out, CL_TRUE, 0,
	lenght * sizeof(uint64_t),output1, 0, NULL, NULL);
	
	finalizeExecution(source_str);
	
	return output1;
}

uint64_t* camellia256Encrypt(char* fileName, uint64_t* K, char* outFileName, size_t local_item_size){

        struct FileInfo64 fileInfo = getFileUint64(fileName);
        
        //plaintext divided in blocks of uint64_t
        uint64_t* inputText = fileInfo.filePointer;
        //number of blocks 
        long lenght = fileInfo.lenght;

        int kdim = 24;
        int kedim = 6;
        int kwdim = 4;

	uint64_t k[kdim];
	uint64_t ke[kedim];
	uint64_t kw[kwdim];

	//key expansion is performed on cpu
	camellia_256_expandkey(K, kw, k, ke);

	// load program source to build the kernel program
	if(source_str == NULL){
		loadClProgramSource();
	}

	long source_size = strlen(source_str);
        
        setUpOpenCl(inputText, k, ke, kw, "camellia192256Cipher",source_str,source_size,kdim, kedim, kwdim, lenght);

        size_t global_item_size = lenght/2;
	/* Execute OpenCL Kernel instances */
	ret = clEnqueueNDRangeKernel(command_queue, kernel, 1, NULL, &global_item_size, &local_item_size, 0, NULL, NULL);

	/* Copy results from the memory buffer */
	uint64_t* output = (uint64_t*)malloc((lenght+1)*sizeof(uint64_t)); // Enough memory for file + \0
	
	ret = clEnqueueReadBuffer(command_queue, out, CL_TRUE, 0,
	lenght * sizeof(uint64_t),output, 0, NULL, NULL);
	
	finalizeExecution(source_str);
	
	return output;
}

uint64_t* camelliaCtr128Encrypt(char* fileName, uint64_t* K, char* outFileName, size_t local_item_size){

        struct FileInfo64 fileInfo = getFileUint64(fileName);
        
        //plaintext divided in blocks of uint64_t
        uint64_t* inputText = fileInfo.filePointer;
        //number of blocks 
        long lenght = fileInfo.lenght;

        int kdim = 18;
        int kedim = 4;
        int kwdim = 4;

	uint64_t k[kdim];
	uint64_t ke[kedim];
	uint64_t kw[kwdim];

	//key expansion is performed on cpu
	camellia_128_expandkey(K, kw, k, ke);

	// load program source to build the kernel program
	if(source_str == NULL){
		loadClProgramSource();
	}

	long source_size = strlen(source_str);
        
        setUpOpenCl(inputText, k, ke, kw, "camellia128CtrCipher",source_str,source_size,kdim, kedim, kwdim, lenght);

        size_t global_item_size = lenght/2;
	/* Execute OpenCL Kernel instances */
	ret = clEnqueueNDRangeKernel(command_queue, kernel, 1, NULL, &global_item_size, &local_item_size, 0, NULL, NULL);

	/* Copy results from the memory buffer */
	uint64_t* output = (uint64_t*)malloc((lenght+1)*sizeof(uint64_t)); // Enough memory for file + \0
	
	ret = clEnqueueReadBuffer(command_queue, out, CL_TRUE, 0,
	lenght * sizeof(uint64_t),output, 0, NULL, NULL);
	
	finalizeExecution(source_str);
	
	return output;
}

uint64_t* camelliaCtr192Encrypt(char* fileName, uint64_t* K, char* outFileName, size_t local_item_size){

        struct FileInfo64 fileInfo = getFileUint64(fileName);
        
        //plaintext divided in blocks of uint64_t
        uint64_t* inputText = fileInfo.filePointer;
        //number of blocks 
        long lenght = fileInfo.lenght;

        int kdim = 24;
        int kedim = 6;
        int kwdim = 4;

	uint64_t k[kdim];
	uint64_t ke[kedim];
	uint64_t kw[kwdim];

	//key expansion is performed on cpu
	camellia_192_expandkey(K, kw, k, ke);

	// load program source to build the kernel program
	if(source_str == NULL){
		loadClProgramSource();
	}

	long source_size = strlen(source_str);
        
        setUpOpenCl(inputText, k, ke, kw, "camellia192256CtrCipher",source_str,source_size,kdim, kedim, kwdim, lenght);

        size_t global_item_size = lenght/2;
	/* Execute OpenCL Kernel instances */
	ret = clEnqueueNDRangeKernel(command_queue, kernel, 1, NULL, &global_item_size, &local_item_size, 0, NULL, NULL);

	/* Copy results from the memory buffer */
	uint64_t* output = (uint64_t*)malloc((lenght+1)*sizeof(uint64_t)); // Enough memory for file + \0
	
	ret = clEnqueueReadBuffer(command_queue, out, CL_TRUE, 0,
	lenght * sizeof(uint64_t),output, 0, NULL, NULL);
	
	finalizeExecution(source_str);
	
	return output;
}

uint64_t* camelliaCtr256Encrypt(char* fileName, uint64_t* K, char* outFileName, size_t local_item_size){

        struct FileInfo64 fileInfo = getFileUint64(fileName);
        
        //plaintext divided in blocks of uint64_t
        uint64_t* inputText = fileInfo.filePointer;
        //number of blocks 
        long lenght = fileInfo.lenght;

        int kdim = 24;
        int kedim = 6;
        int kwdim = 4;

	uint64_t k[kdim];
	uint64_t ke[kedim];
	uint64_t kw[kwdim];

	//key expansion is performed on cpu
	camellia_256_expandkey(K, kw, k, ke);

	// load program source to build the kernel program
	if(source_str == NULL){
		loadClProgramSource();
	}

	long source_size = strlen(source_str);
        
        setUpOpenCl(inputText, k, ke, kw, "camellia192256CtrCipher",source_str,source_size,kdim, kedim, kwdim, lenght);

        size_t global_item_size = lenght/2;
	/* Execute OpenCL Kernel instances */
	ret = clEnqueueNDRangeKernel(command_queue, kernel, 1, NULL, &global_item_size, &local_item_size, 0, NULL, NULL);

	/* Copy results from the memory buffer */
	uint64_t* output = (uint64_t*)malloc((lenght+1)*sizeof(uint64_t)); // Enough memory for file + \0
	
	ret = clEnqueueReadBuffer(command_queue, out, CL_TRUE, 0,
	lenght * sizeof(uint64_t),output, 0, NULL, NULL);
	
	finalizeExecution(source_str);
	
	return output;
}

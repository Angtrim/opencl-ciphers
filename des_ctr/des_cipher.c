#include "des_cipher.h"

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
	source_str = (char*)malloc(MAX_SOURCE_SIZE);
	fread(source_str, 1, MAX_SOURCE_SIZE, fp);
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





static void setUpOpenCl(byte* inputText, char* kernelName, uint8_t* key, char* source_str, long source_size, long bufferLenght){
	
	des_context K;
	des_expandkey(&K, key);

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
	ks = clCreateBuffer(context, CL_MEM_READ_WRITE, sizeof(word)*32, NULL, &ret); 
	in = clCreateBuffer(context, CL_MEM_READ_WRITE, bufferLenght * sizeof(byte), NULL, &ret);
	out = clCreateBuffer(context, CL_MEM_READ_WRITE, bufferLenght * sizeof(byte), NULL, &ret);


	/* Copy input data to Memory Buffer */
	ret = clEnqueueWriteBuffer(command_queue, ks, CL_TRUE, 0, sizeof(word)*32, K.esk, 0, NULL, NULL);
	ret = clEnqueueWriteBuffer(command_queue, in, CL_TRUE, 0, bufferLenght * sizeof(byte), inputText, 0, NULL, NULL);

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
	ret = clSetKernelArg(kernel, 1, sizeof(cl_mem), (void *)&ks);
	ret = clSetKernelArg(kernel, 2, sizeof(cl_mem), (void *)&out);
}

static void setUpOpenCl3(byte* inputText, char* kernelName, uint8_t* key,char* source_str, long source_size, long bufferLenght){
	
	des3_context K;
	tdes3_expandkey(&K,key);

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
	ks = clCreateBuffer(context, CL_MEM_READ_WRITE, sizeof(word)*96, NULL, &ret); 
	in = clCreateBuffer(context, CL_MEM_READ_WRITE, bufferLenght * sizeof(byte), NULL, &ret);
	out = clCreateBuffer(context, CL_MEM_READ_WRITE, bufferLenght * sizeof(byte), NULL, &ret);


	/* Copy input data to Memory Buffer */
	ret = clEnqueueWriteBuffer(command_queue, ks, CL_TRUE, 0, sizeof(word)*32, K.esk, 0, NULL, NULL);
	ret = clEnqueueWriteBuffer(command_queue, in, CL_TRUE, 0, bufferLenght * sizeof(byte), inputText, 0, NULL, NULL);

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
	ret = clSetKernelArg(kernel, 1, sizeof(cl_mem), (void *)&ks);
	ret = clSetKernelArg(kernel, 2, sizeof(cl_mem), (void *)&out);
}

static void finalizeExecution(char* source_str){
	/* Finalization */
	ret = clFlush(command_queue);
	ret = clFinish(command_queue);
	ret = clReleaseKernel(kernel);
	ret = clReleaseProgram(program);
	ret = clReleaseMemObject(in);
	ret = clReleaseMemObject(ks);
	ret = clReleaseMemObject(out);
	ret = clReleaseCommandQueue(command_queue);
	ret = clReleaseContext(context);
	//free(source_str);
}

byte* desEncrypt(char* fileName, uint8_t* key, char* outFileName,size_t local_item_size, int mode, int isCtr) {

	
	struct FileInfo fileInfo = getFileBytes(fileName);
    
 byte* inputText = fileInfo.filePointer;

   
 // load program source to build the kernel program
 if(source_str == NULL){
		loadClProgramSource();
	}
    
	long source_size = strlen(source_str);
	char* modality;
	if(isCtr){
		if(mode == 0){
					modality = "desCipherCtr";
					setUpOpenCl(inputText, modality,key,source_str,source_size,fileInfo.lenght);

		}else{
					modality = "des3CipherCtr";
					setUpOpenCl3(inputText, modality,key,source_str,source_size,fileInfo.lenght);

		}
	}else{

		if(mode == 0){
					modality = "desCipher";
					setUpOpenCl(inputText, modality,key,source_str,source_size,fileInfo.lenght);

		}else{
					modality = "des3Cipher";
					setUpOpenCl3(inputText, modality,key,source_str,source_size,fileInfo.lenght);
		}

	}
	
	
	size_t global_item_size = fileInfo.lenght/BLOCK_SIZE;
	/* Execute OpenCL Kernel instances */
	ret = clEnqueueNDRangeKernel(command_queue, kernel, 1, NULL, &global_item_size, &local_item_size, 0, NULL, NULL);

	/* Copy results from the memory buffer */
	byte* output = (byte*)malloc((fileInfo.lenght+1)*sizeof(byte)); // Enough memory for file + \0
	
	ret = clEnqueueReadBuffer(command_queue, out, CL_TRUE, 0,
	fileInfo.lenght * sizeof(byte),output, 0, NULL, NULL);
	
	finalizeExecution(source_str);
	
	return output;
}	


byte* desSingleCtrEncrypt(char* fileName, uint8_t* key, char* outFileName,size_t local_item_size) {
		int mode = 0;
		return desEncrypt(fileName,key,outFileName,local_item_size,mode,1);
}	

byte* des3CtrEncrypt(char* fileName, uint8_t* key, char* outFileName,size_t local_item_size) {
		int mode = 3;
		return desEncrypt(fileName,key,outFileName,local_item_size,mode,1);
}	


byte* desSingleEncrypt(char* fileName, uint8_t* key, char* outFileName,size_t local_item_size) {
		int mode = 0;
		return desEncrypt(fileName,key,outFileName,local_item_size,mode,0);
}	

byte* des3Encrypt(char* fileName, uint8_t* key, char* outFileName,size_t local_item_size) {
		int mode = 3;
		return desEncrypt(fileName,key,outFileName,local_item_size,mode,0);
}	





#include "cipher_utils.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#define MAX_PLATFORMS 32
#define MAX_DEVICES   32
#define MAX_STR_LEN   1024

#define MAX_SOURCE_SIZE (0x1000000)

/*
 * This function load the size in blocks of bytes 
 * of the plaintext generated from the makefile
 */ 
 struct FileInfo getFileBytes(char* filePath){
 	FILE *fileptr;
 	byte *buffer;
 	long filelen;

	fileptr = fopen(filePath, "rb");  // Open the file in binary mode
	fseek(fileptr, 0, SEEK_END);          // Jump to the end of the file
	filelen = ftell(fileptr);             // Get the current byte offset in the file
	rewind(fileptr);                      // Jump back to the beginning of the file

	buffer = (byte *)malloc((filelen+1)*sizeof(byte));
	fread(buffer, filelen, 1, fileptr); // Read in the entire file
	//strcat(buffer, "\0");
	fclose(fileptr); // Close the file
	struct FileInfo fileInfo;
	fileInfo.filePointer = buffer;
	fileInfo.lenght = filelen;
	return fileInfo;
}

/*
 * This function load the size in blocks of uint64_t 
 * of the plaintext generated from the makefile
 */ 
 struct FileInfo64 getFileUint64(char* filePath){
 	FILE *fileptr;
 	uint64_t *buffer;
 	uint8_t temp[8];
 	long filelen;

	fileptr = fopen(filePath, "rb");  // Open the file in binary mode
	fseek(fileptr, 0, SEEK_END);          // Jump to the end of the file
	filelen = ftell(fileptr);             // Get the current byte offset in the file
	rewind(fileptr);                      // Jump back to the beginning of the file

	buffer = (uint64_t *)malloc((filelen/8)*sizeof(uint64_t)); // Enough memory for file + \0

	int k = 0;
	while(fread(temp, sizeof(uint64_t), 1, fileptr) == 1){
		buffer[k] = *(uint64_t *)temp;
		k++;
	} 
	fclose(fileptr); // Close the file
	struct FileInfo64 fileInfo;
	fileInfo.filePointer = buffer;
	fileInfo.lenght = filelen/8;
	return fileInfo;
}

/*
 * Get byte dimension of a file
 */
 long getByteLenght(char* filePath){
 	FILE *fileptr;
 	long filelen;

	fileptr = fopen(filePath, "rb");  // Open the file in binary mode
	fseek(fileptr, 0, SEEK_END);          // Jump to the end of the file
	filelen = ftell(fileptr);             // Get the current byte offset in the file
	rewind(fileptr);                      // Jump back to the beginning of the file
	fclose(fileptr);
	return filelen;	
}


void getSystemInfo(){
	    unsigned int i, j;  // iterator variables for loops

	    cl_int err;
    cl_platform_id platforms[MAX_PLATFORMS]; // IDs of all the platforms    
    cl_uint num_platforms;  // number of platforms on this machine
    char vendor[MAX_STR_LEN];   // platforms vendor string
    char opencl_version[MAX_STR_LEN];   // opencl version string
    cl_device_id devices[MAX_DEVICES]; // number of devices for each platform 
    cl_uint num_devices; // number of devices on this machine
    char deviceName[MAX_STR_LEN];   // devices name    
    cl_uint numberOfCores;  // number of cores of on a device
    cl_long amountOfMemory; // amount of memory on a device
    cl_uint clockFreq;  // clock frequency of a device
    cl_ulong maxAllocatableMem; // maximum allocatable memory
    cl_ulong localMem;  // local memory for a device
    cl_bool available;  // tells if the device is available
    size_t device_wg_size; // ...

    //get the number of platforms
    err = clGetPlatformIDs (32, platforms, &num_platforms);

    int fs1 = 32, fs2=fs1-4, fs3=fs2-4;
    printf("\n%-*s %u\n\n", fs1, "Number of platforms:", num_platforms);    

    // iterate over platforms
    for(i = 0; i < num_platforms; i++) {

    	printf("Platform: %u\n\n", i);
    	err = clGetPlatformInfo (platforms[i], CL_PLATFORM_VENDOR, sizeof(vendor), vendor, NULL);
    	if (err != CL_SUCCESS) exit((printf("Error in clGetPlatformInfo"),1));

    	printf("    %-*s %s\n", fs2, "Platform Vendor:", vendor);

    	err = clGetDeviceIDs (platforms[i], CL_DEVICE_TYPE_ALL, sizeof(devices), devices, &num_devices);
    	if (err != CL_SUCCESS) exit((printf("Error in clGetDeviceIDs %d",err),1));

    	printf("    %-*s %u\n\n", fs2, "Number of devices:", num_devices);

        // iterate over devices
    	for (j = 0; j < num_devices; j++) {

            // scan in device information
    		err = clGetDeviceInfo(devices[j], CL_DEVICE_NAME, sizeof(deviceName), deviceName, NULL);
    		if (err != CL_SUCCESS) exit((printf("Error in retrieving CL_DEVICE_NAME"),1));

    		err = clGetDeviceInfo(devices[j], CL_DEVICE_VENDOR, sizeof(vendor), vendor, NULL);
    		if (err != CL_SUCCESS) exit((printf("Error in retrieving CL_DEVICE_VENDOR"),1));

    		err = clGetDeviceInfo(devices[j], CL_DEVICE_OPENCL_C_VERSION, sizeof(opencl_version), opencl_version, NULL);
    		if (err != CL_SUCCESS) exit((printf("Error in retrieving CL_DEVICE_OPENCL_C_VERSION"),1));

    		err = clGetDeviceInfo(devices[j], CL_DEVICE_MAX_COMPUTE_UNITS, sizeof(numberOfCores), &numberOfCores, NULL);
    		if (err != CL_SUCCESS) exit((printf("Error in retrieving CL_DEVICE_MAX_COMPUTE_UNITS"),1));

    		err = clGetDeviceInfo(devices[j], CL_DEVICE_GLOBAL_MEM_SIZE, sizeof(amountOfMemory), &amountOfMemory, NULL);
    		if (err != CL_SUCCESS) exit((printf("Error in retrieving CL_DEVICE_GLOBAL_MEM_SIZE"),1));

    		err = clGetDeviceInfo(devices[j], CL_DEVICE_MAX_CLOCK_FREQUENCY, sizeof(clockFreq), &clockFreq, NULL);
    		if (err != CL_SUCCESS) exit((printf("Error in retrieving CL_DEVICE_MAX_CLOCK_FREQUENCY"),1));

    		err = clGetDeviceInfo(devices[j], CL_DEVICE_MAX_MEM_ALLOC_SIZE, sizeof(maxAllocatableMem), &maxAllocatableMem, NULL);
    		if (err != CL_SUCCESS) exit((printf("Error in retrieving CL_DEVICE_MAX_MEM_ALLOC_SIZE"),1));

    		err = clGetDeviceInfo(devices[j], CL_DEVICE_LOCAL_MEM_SIZE, sizeof(localMem), &localMem, NULL);
    		if (err != CL_SUCCESS) exit((printf("Error in retrieving CL_DEVICE_LOCAL_MEM_SIZE"),1));

    		err = clGetDeviceInfo(devices[j], CL_DEVICE_AVAILABLE, sizeof(available), &available, NULL);
    		if (err != CL_SUCCESS) exit((printf("Error in retrieving CL_DEVICE_AVAILABLE"),1));

    		err = clGetDeviceInfo(devices[j], CL_DEVICE_MAX_WORK_GROUP_SIZE, sizeof(device_wg_size), &device_wg_size, NULL);
    		if (err != CL_SUCCESS) exit((printf("Error in retrieving CL_DEVICE_MAX_WORK_GROUP_SIZE"),1));

      // print out device info
    		printf("    %-*s %u\n", fs2, "Device:", j);
    		printf("        %-*s %s\n", fs3, "Name:", deviceName);
    		printf("        %-*s %s\n", fs3, "Vendor:", vendor);
    		printf("        %-*s %s\n", fs3, "OpenCL version:", opencl_version);
    		printf("        %-*s %s\n", fs3, "Available:", available ? "Yes" : "No");
    		printf("        %-*s %u\n", fs3, "Compute Units:", numberOfCores);
    		printf("        %-*s %u MHz\n", fs3, "Clock Frequency:", clockFreq);
    		printf("        %-*s %0.00f MB\n", fs3, "Global Memory:", (double)amountOfMemory/1048576);            
    		printf("        %-*s %0.00f MB\n", fs3, "Max Allocatable Memory:", (double)maxAllocatableMem/1048576);
    		printf("        %-*s %u KB\n", fs3, "Local Memory:", (unsigned int)localMem);
    		printf("        %-*s %lu\n\n", fs3, "Max work group size:", device_wg_size);
    	}
    }
   }


   void loadClProgramSource(char* fileName,char** source_str,size_t* source_size){
	/* Load the source code containing the kernel*/
   	FILE *fp = fopen(fileName, "r");
   	if (!fp) {
   		fprintf(stderr, "Failed to load program source\n");
   		exit(1);
   	}
   	*source_str = (char*)malloc(MAX_SOURCE_SIZE);
   	*source_size = fread(*source_str, 1, MAX_SOURCE_SIZE, fp);
   	fclose(fp);
   }



   void logBuildError(cl_int* ret,cl_program* program, cl_device_id* deviceId){
   	printf("\nBuild Error = %d", *ret);
				// Determine the size of the log
   	size_t log_size;
   	clGetProgramBuildInfo(*program, *deviceId, CL_PROGRAM_BUILD_LOG, 0, NULL, &log_size);
				// Allocate memory for the log
   	char *log = (char *) malloc(log_size);
				// Get the log
   	clGetProgramBuildInfo(*program, *deviceId, CL_PROGRAM_BUILD_LOG, log_size, log, NULL);
				// Print the log
   	printf("%s\n", log);
   }

   void initClSetup(cl_device_id* device_id,cl_context* context,cl_command_queue* command_queue){
   	cl_int ret;
				// Create OpenCL context 
   	*context = clCreateContext(NULL, 1,device_id, NULL, NULL, &ret);
   	if(ret != CL_SUCCESS){
   		printf("Failed to create context : %d\n",ret);
   	}
				// Create Command Queue 
   	*command_queue = clCreateCommandQueue(*context, *device_id, CL_QUEUE_PROFILING_ENABLE, &ret);
   	if(ret != CL_SUCCESS){
   		printf("Failed to create commandqueue\n");
   	}
   }




#include "cipher_utils.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>


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

/* Selecting the device */
void setDeviceType(char* deviceType,cl_device_type* deviceTypeCl){

	if(strcmp(deviceType,"CPU") == 0)
		*deviceTypeCl = CL_DEVICE_TYPE_CPU;
	else if(strcmp(deviceType, "GPU") == 0)
		*deviceTypeCl = CL_DEVICE_TYPE_GPU;
}

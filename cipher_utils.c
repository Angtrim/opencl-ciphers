#include "cipher_utils.h"

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

		buffer = (byte *)malloc((filelen+1)*sizeof(byte)); // Enough memory for file + \0
		fread(buffer, filelen, 1, fileptr); // Read in the entire file
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



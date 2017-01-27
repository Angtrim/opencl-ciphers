#include "cipher_utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


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



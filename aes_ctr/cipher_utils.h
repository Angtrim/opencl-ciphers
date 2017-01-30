#ifndef CIPHER_UTILS_H
#define CIPHER_UTILS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef uint32_t word;
typedef uint8_t byte;

static struct FileInfo {
		byte* filePointer;
		long lenght;
}; 



struct FileInfo getFileBytes(char* filePath);

#endif

#ifndef CIPHER_UTILS_H
#define CIPHER_UTILS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <inttypes.h>

typedef uint32_t word;
typedef uint8_t byte;

static struct FileInfo {
		byte* filePointer;
		long lenght;
}; 

static struct FileInfo64 {
		uint64_t* filePointer;
		long lenght;
}; 

struct FileInfo64 getFileUint64(char* filePath);

struct FileInfo getFileBytes(char* filePath);

long getByteLenght(char* filePath);

#endif

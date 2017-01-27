#ifndef CIPHER_UTILS_H
#define CIPHER_UTILS_H
#include <stdint.h>
typedef uint32_t word;
typedef uint8_t byte;

struct FileInfo {
		byte* filePointer;
		long lenght;
}; 



struct FileInfo getFileBytes(char* filePath);

byte* getBytesBlocks(struct FileInfo fileInfo, int numbOfBytePerBlock);

#endif

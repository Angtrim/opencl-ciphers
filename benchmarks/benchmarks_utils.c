#include "benchmarks_utils.h"
#include <stdint.h>
#include <stdio.h>

// to save ciphertext to file (uint8_t)
void buildFileOfZeroes(char* outFileName,long lenght){
	uint8_t* zeroes = (uint8_t*)malloc(lenght*sizeof(uint8_t));
	// fill with zeroes
	for(int i = 0; i <lenght; i++){
		 zeroes[i] = 0;	
	}
	FILE* fp = fopen(outFileName, "wb");
	if (!fp) {
	fprintf(stderr, "Failed to load file.\n");
	exit(1);
	}
	fwrite(zeroes, sizeof(char), lenght, fp);
	fclose(fp);
	free(zeroes);
}
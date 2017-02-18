#include "benchmarks_utils.h"
#include <stdint.h>
#include <stdio.h>
#include <time.h>
#include <string.h>


// to save ciphertext to file (uint8_t)
void buildFileOfZeroes(char* outFileName,long lenght){
	uint8_t* zeroes = (uint8_t*)malloc(lenght*sizeof(uint8_t));
	// fill with zeroes
	for(int i = 0; i <lenght; i++){
		zeroes[i] = 'A';	
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


void saveDataToFile(char* nameCiph,struct BenchInfo* infos,int numInfos){
	time_t rawtime;
	struct tm * timeinfo;

	time ( &rawtime );
	timeinfo = localtime ( &rawtime );

	char outFileName[100];
	strcpy(outFileName, "./bench_out/");
	strcat(outFileName, nameCiph);
	strcat(outFileName, "_");
	strcat(outFileName, asctime (timeinfo));
	strcat(outFileName, ".dat");


	FILE* fp = fopen(outFileName, "w");
	if (!fp) {
		fprintf(stderr, "Failed to load file.\n");
		exit(1);
	}



	fprintf(fp, "%s %s\n","# CIPHER: ",nameCiph);
	fprintf(fp, "%s %ld\n","# FILESIZE (bytes): ",infos[0].fileSize);
	fprintf(fp, "%s %s\n","# DEVICE: ",nameCiph);

 fprintf(fp, "\n\n%s \t  %s \n","# LOCAL_SIZE","TOTAL_TIME (ns)");

	for(int i = 0;i< numInfos;i++){
			fprintf(fp, "%d %ld\n",infos[i].localSize, infos[i].totalTime);
	}
	
	fclose(fp);

}

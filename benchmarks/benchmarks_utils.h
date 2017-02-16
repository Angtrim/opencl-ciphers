#ifndef BENCHMARKS_UTILS_H
#define BENCHMARKS_UTILS_H


static struct BenchInfo {
		long totalTime;
		int localSize;
		long fileSize;
}; 

void buildFileOfZeroes(char* outFileName,long lenght);



#endif

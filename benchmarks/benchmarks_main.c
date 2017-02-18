#include "benchmarks_main.h"


int mainBench(){
	int localSize[] = {1,2};
	printf("\nSTARTING BENCHMARKS");
	benchAes128Multiple(100000,localSize,2,1);
	benchAes192Multiple(100000,localSize,2,1);
	benchAes256Multiple(100000,localSize,2,1);
	benchDesMultiple(100000,localSize,2,1);
	benchDes2Multiple(100000,localSize,2,1);
	benchDes3Multiple(100000,localSize,2,1);
	benchCam128Multiple(100000,localSize,2,1);
	benchCam192Multiple(100000,localSize,2,1);
	benchCam256Multiple(100000,localSize,2,1);
	benchMisty1CtrMultiple(100000,localSize,2,1);
	benchSeedCtrMultiple(100000, localSize, 2,1);
	benchClefia128Multiple(100000, localSize, 2,0);
	benchClefia192Multiple(100000, localSize, 2,0);
	benchClefia256Multiple(100000, localSize, 2,0);
	benchHightMultiple(1000000, localSize, 2, 0);
	benchPresentSpeedMultiple(1000000, localSize, 2,0);
	benchPresentMemoryMultiple(100000, localSize, 2,0);
	
}

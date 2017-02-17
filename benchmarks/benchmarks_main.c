#include "benchmarks_main.h"


int mainBench(){
	int localSize[] = {2,4,20,80};
 benchAes128Multiple(100000,localSize,4,0);
 benchDesMultiple(100000,localSize,4,0);
 benchDes2Multiple(100000,localSize,4,0);
 benchDes3Multiple(100000,localSize,4,0);
 benchCam128Multiple(100000,localSize,4,0);
 benchCam192Multiple(100000,localSize,4,0);
 benchCam256Multiple(100000,localSize,4,0);
 benchMisty1CtrMultiple(100000000,localSize,4,0);
 benchSeedCtrMultiple(100000000, localSize, 4,0);
}

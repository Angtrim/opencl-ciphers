#include "benchmarks_main.h"


int mainBench(){
	int localSize[] = {2,4,20,80};
 	benchAes128CtrMultiple(100000000,localSize,4,0);
 	benchMisty1CtrMultiple(100000000,localSize,4,0);
 	benchSeedCtrMultiple(100000000, localSize, 4,0);
}

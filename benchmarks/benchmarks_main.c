#include "benchmarks_main.h"


int mainBench(){
	int localSize[] = {2,4,20,80};
 benchAes128Multiple(100000000,localSize,4,1);

}
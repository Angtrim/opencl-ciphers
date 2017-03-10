#include "benchmarks_main.h"


int mainBench(cl_device_id* device_id){
	int localSize[] = {1,2};
	int localSizeClefia[] = {1};
	printf("\nSTARTING BENCHMARKS");
	benchAes128Multiple(100000,localSize,2,device_id);
	benchAes192Multiple(100000,localSize,2,device_id);
	benchAes256Multiple(100000,localSize,2,device_id);
	benchDesMultiple(100000,localSize,2,device_id);
	benchDes2Multiple(100000,localSize,2,device_id);
	benchDes3Multiple(100000,localSize,2,device_id);
	benchCam128Multiple(100000,localSize,2,device_id);
	benchCam192Multiple(100000,localSize,2,device_id);
	benchCam256Multiple(100000,localSize,2,device_id);
	benchMisty1CtrMultiple(100000,localSize,2,device_id);
	benchSeedCtrMultiple(100000, localSize, 2,device_id);
	benchClefia128Multiple(100000, localSizeClefia, 2,device_id);
	benchClefia192Multiple(100000, localSizeClefia, 2,device_id);
	benchClefia256Multiple(100000, localSizeClefia, 2,device_id);
	benchHightMultiple(1000000, localSize, 2, device_id);
	benchPresentSpeedMultiple(1000000, localSize, 2,device_id);
	benchPresentMemoryMultiple(100000, localSize, 2,device_id);
	
}

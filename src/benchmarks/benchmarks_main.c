#include "benchmarks_main.h"
#include <stdio.h>

void mainBench(cl_device_id* device_id){
	int localSize[] = {1,2,4,8,16,32,64,128,256,512};
	int fileSize[] = {1000000,5000000,10000000,50000000,100000000,500000000,1000000000};
	printf("\nSTARTING BENCHMARKS\n\n");


	for(int i = 0;i< sizeof(fileSize) ;i++){
		benchAes128Multiple(fileSize[i],localSize, sizeof(localSize),device_id);
		benchAes128Multiple(fileSize[i],localSize, sizeof(localSize),device_id);
		benchAes192Multiple(fileSize[i],localSize, sizeof(localSize),device_id);
		benchAes256Multiple(fileSize[i],localSize, sizeof(localSize),device_id);
		benchClefia128Multiple(fileSize[i], localSize, sizeof(localSize),device_id);
		benchClefia192Multiple(fileSize[i], localSize, sizeof(localSize),device_id);
		benchClefia256Multiple(fileSize[i], localSize, sizeof(localSize),device_id);
		benchDesMultiple(fileSize[i],localSize, sizeof(localSize),device_id);
		benchDes2Multiple(fileSize[i],localSize, sizeof(localSize),device_id);
		benchDes3Multiple(fileSize[i],localSize, sizeof(localSize),device_id);
		benchCam128Multiple(fileSize[i],localSize, sizeof(localSize),device_id);
		benchCam192Multiple(fileSize[i],localSize, sizeof(localSize),device_id);
		benchCam256Multiple(fileSize[i],localSize, sizeof(localSize),device_id);
		benchMisty1CtrMultiple(fileSize[i],localSize, sizeof(localSize),device_id);
		benchSeedCtrMultiple(fileSize[i], localSize, sizeof(localSize),device_id);
		benchPresentSpeedMultiple(fileSize[i], localSize, sizeof(localSize),device_id); 
		benchPresentMemoryMultiple(fileSize[i], localSize, sizeof(localSize),device_id); 
		benchHightMultiple(fileSize[i], localSize, sizeof(localSize), device_id);	
	}
}

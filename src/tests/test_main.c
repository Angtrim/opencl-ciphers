#include "test_aes.h"
#include "test_misty1.h"
#include "test_des.h"
#include "test_camellia.h"
#include "test_clefia.h"
#include "test_hight.h"
#include "test_present.h"
#include "test_seed.h"
#include "../cipher_utils.h"
#include "../benchmarks/benchmarks_main.h"


int main(){
		getSystemInfo();
	cl_uint ret_num_devices;
	cl_uint ret_num_platforms;
	cl_platform_id platform_id[32];;
	cl_device_id device_id[32];
	// Get Platform and Device Info 
	cl_int ret = clGetPlatformIDs(1, &platform_id, &ret_num_platforms);
	// allocate memory, get list of platforms
	cl_platform_id *platforms = (cl_platform_id *) malloc(ret_num_platforms*sizeof(platform_id));
	clGetPlatformIDs(ret_num_platforms, platforms, NULL);
	// iterate over platforms
	for (cl_uint i = 0; i < ret_num_platforms; ++i){
		ret = clGetDeviceIDs(platforms[i], CL_DEVICE_TYPE_ALL, 1, device_id, &ret_num_devices);
		  if(ret != CL_SUCCESS){
   		printf("Failed to find device : %d\n",ret);
   	}
	}
	free(platforms);
printf("device : %d\n",device_id);



		int aesResult = testAESAll(device_id);
		int misty1Result = testMisty1All(device_id);
		int desResult = testDESAll(device_id);
		int cameliaResult = testCamelliaAll(device_id);
		int clefiaResult = testClefiaAll(device_id);
		int seedResult = testSeedAll(device_id);
		int presentResult = testPresentAll(device_id);
		int hightResult = testHightAll(device_id);
		int endResult = aesResult && misty1Result && desResult && cameliaResult && clefiaResult && seedResult && presentResult && hightResult;

		if(endResult != 0){
			printf("\nALL TEST PASSED SUCCESSEFULLY\n");
		}else{
			printf("\nSOME TEST FAILED\n");
		}

		mainBench(device_id);



}
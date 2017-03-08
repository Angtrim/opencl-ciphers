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
		int aesResult = testAESAll();
		int misty1Result = testMisty1All();
		int desResult = testDESAll();
		int cameliaResult = testCamelliaAll();
		int clefiaResult = testClefiaAll();
		int seedResult = testSeedAll();
		int presentResult = testPresentAll();
		int hightResult = testHightAll();
		int endResult = aesResult && misty1Result && desResult && cameliaResult && clefiaResult && seedResult && presentResult && hightResult;

		if(endResult != 0){
			printf("\nALL TEST PASSED SUCCESSEFULLY\n");
		}else{
			printf("\nSOME TEST FAILED\n");
		}



		mainBench();



}
#include "test_aes.h"
#include "test_misty1.h"
#include "test_des.h"
#include "test_camelia.h"
#include "test_clefia.h"
#include "test2.h"
#include "../benchmarks/benchmarks_main.h"


int main(){
		int aesResult = testAESAll();
		int misty1Result = testMisty1All();
		int desResult = testDESAll();
		int cameliaResult = testCameliaAll();
		int clefiaResult = testClefiaAll();
		int seedResult = testSeedAll();
		int presentResult = testPresentAll();
		int endResult = aesResult && misty1Result && desResult && cameliaResult && clefiaResult && seedResult && presentResult;

		if(endResult != 0){
			printf("\nALL TEST PASSED SUCCESSEFULLY\n");
		}else{
			printf("\nSOME TEST FAILED ");
		}



		mainBench();

	 //	testFra();

}
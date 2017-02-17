#include "test_aes.h"
#include "test_misty1.h"
#include "test2.h"
#include "../benchmarks/benchmarks_main.h"


int main(){
		int aesResult = testAESAll();

		int misty1Result = testMisty1All();

		int seedResult = testSeedAll();

		int presentResult = testPresentAll();

		int endResult = aesResult&&misty1Result&&seedResult&&presentResult;

		if(endResult != 0){
			printf("\nALL TEST PASSED SUCCESSEFULLY\n");
		}



		//mainBench();
		testFra();
}
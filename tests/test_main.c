#include "test_aes.h"
#include "test_misty1.h"
#include "test2.h"
#include "../benchmarks/benchmarks_main.h"


int main(){
		int aesResult = testAESAll();

		int misty1Result = testMisty1All();

		int endResult = aesResult&&misty1Result;

		if(endResult != 0){
			printf("\nALL TEST PASSED SUCCESSEFULLY\n");
		}



		mainBench();
		//testFra();
}
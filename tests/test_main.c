#include "test_aes.h"
#include "test_misty1.h"
#include "test_des.h"
#include "test_camelia.h"
#include "test2.h"
#include "../benchmarks/benchmarks_main.h"


int main(){
		int aesResult = testAESAll();
		int misty1Result = testMisty1All();
		int desResult = testDESAll();
		int cameliaResult = testCameliaAll();

		int endResult = aesResult && misty1Result && desResult && cameliaResult;

		if(endResult != 0){
			printf("ALL TEST PASSED SUCCESSEFULLY\n");
		}else{
			printf("SOME TESTS FAILED, CHECK LOG\n");
		}



		//mainBench();
		//testFra();
}
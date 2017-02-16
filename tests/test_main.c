#include "test_aes.h"
#include "test2.h"


int main(){
		int aesResult = testAESAll();


		int endResult = aesResult;

		if(endResult != 0){
			printf("ALL TEST PASSED SUCCESSEFULLY\n");
		}

		testFra();
}
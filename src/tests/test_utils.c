#include "test_utils.h"
#include <stdio.h>

#define DEBUG_LOG 1

void logIt(char* message){
	if(DEBUG_LOG){
		printf("%s\n", message);
	}
}
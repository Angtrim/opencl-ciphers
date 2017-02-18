
#include "present_expansion.h"

const uint64_t sBox4_S80[] = {
	0xc000000000000000,0x5000000000000000,0x6000000000000000,0xb000000000000000,
	0x9000000000000000,0x0000000000000000,0xa000000000000000,0xd000000000000000,
	0x3000000000000000,0xe000000000000000,0xf000000000000000,0x8000000000000000,
	0x4000000000000000,0x7000000000000000,0x1000000000000000,0x2000000000000000
};

const uint16_t sBox4_M80[] = {12,5,6,11,9,0,10,13,3,14,15,8,4,7,1,2};

void present_memory_expandKey(uint64_t *key, uint64_t *subkey) {
	//Input value Key
	uint64_t keyhigh=key[0];
	uint64_t keylow=key[1] & 0xffff;
	//Variables for pLayer
	uint64_t temp;
	
  	#pragma unroll
	for(int round=0;round<32;round++){
		subkey[round] = keyhigh;
		temp = keyhigh;
		keyhigh <<= 61;
		keyhigh |= (keylow<<45);
		keyhigh |= (temp>>19);
		keylow = (temp>>3)&0xFFFF;

		temp = keyhigh>>60;
		keyhigh &=	0x0FFFFFFFFFFFFFFF;
		temp = sBox4_M80[temp];
		keyhigh |= temp<<60;

		keylow ^= ( ( (round+1) & 0x01 ) << 15 );
		keyhigh ^= ( (round+1) >> 1 );
	}
}

void present_speed_expandKey(uint64_t *key, uint64_t *subkey) {
	//  Input values
	uint64_t keyhigh = key[0];
	uint64_t keylow = key[1] & 0xffff;
	//Variables key scheduling
	uint64_t temp;

  	#pragma unroll
	for(int round=0;round<32;round++){
		subkey[round] = keyhigh;
		temp = keyhigh;
		keyhigh <<= 61;
		keyhigh |= (keylow<<45);
		keyhigh |= (temp>>19);
		keylow = (temp>>3)&0xFFFF;

		temp = keyhigh>>60;
		keyhigh &=	0x0FFFFFFFFFFFFFFF;
		temp = sBox4_S80[temp];
		keyhigh |= temp;

		keylow ^= ( ( (round+1) & 0x01 ) << 15 );
		keyhigh ^= ( (round+1) >> 1 );
	}
}

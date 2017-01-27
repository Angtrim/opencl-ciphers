#include "aes_cipher.h"
#include <stdio.h>
int main(){

	byte* output;
	uint32_t key[16] = {0x6ede8bab, 0xdad63ffa, 0x796c767d, 0x92e52e38};

 aes128Encrypt("./nullbytes.txt",key, output,1);

}
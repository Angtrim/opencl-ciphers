#include "aes_cipher.h"
#include <stdio.h>
int main(){

 byte* output;
 uint32_t key[16] = {0x6ede8bab, 0xdad63ffa, 0x796c767d, 0x92e52e38};

 output = aesCtr128Encrypt("./aes_plaintext.txt",key, "./aes_ciphertext.txt",1);
 printf("Ciphertext:\n");
 for(int i=0; i < 32; i++){
 	printf("%02x:", output[i]);
 }
 
 output = aes128Encrypt("./aes_plaintext.txt",key, "./aes_ciphertext.txt",1);
 for(int i=0; i < 32; i++){
 	printf("%02x:", output[i]);
 }

printf("\n");
}

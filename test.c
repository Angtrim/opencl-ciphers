//#include "aes_cipher.h"
//#include "camellia_cipher.h"
//#include "aes_cipher.h"
#include <stdio.h>
#include <inttypes.h>
#include <stdlib.h>
int main(){

 //TODO: different test cases
/*
 uint64_t* output;
 uint64_t Key1[2] = {0x0123456789abcdef, 0xfedcba9876543210};
 uint64_t Ciphertext1[4] = {0x6767313854966973, 0x0857065648eabe43, 0x6767313854966973, 0x0857065648eabe43};
 uint64_t Ciphertext2[2] = {0xb4993401b3e996f8, 0x4ee5cee7d79b09b9};
 uint64_t Key2[3] = {0x0123456789abcdef, 0xfedcba9876543210,
                     0x0011223344556677};
 uint64_t Ciphertext3[2] = {0x9acc237dff16d76c, 0x20ef7c919e3a7509};
 uint64_t Key3[4] = {0x0123456789abcdef, 0xfedcba9876543210,
                     0x0011223344556677, 0x8899aabbccddeeff};
 output = camellia128Encrypt("./camellia_plaintext.txt", Key1, "./camellia_ciphertext.txt",1);
 //uint32_t key[32] = {0x63375233, 0xca899f06, 0x2f868c91, 0x8eb2ccc1, 0x36eb5001, 0x58c75cbb, 0x2ee60020, 0x3286d86f};
	
 //output = aes256Encrypt("./aes_plaintext.txt",key, "./aes_ciphertext.txt",1);
 printf("Ciphertext 128:\n");
 for(int i=0; i < 4; i++){
 	printf("%016llx\n", output[i]);
 }

 if (output[0] == Ciphertext1[0] && output[1] == Ciphertext1[1] && output[2] == Ciphertext1[2] && output[3] == Ciphertext1[3]) {
   printf("ok\n");
 } else {
   printf("no\n");
}
free(output);
uint64_t* output1;

output1 = camellia192Encrypt("./aes_plaintext.txt", Key2, "./aes_ciphertext.txt",1);
printf("Ciphertext 192:\n");
 for(int i=0; i < 2; i++){
 	printf("%016llx\n", output1[i]);
 }

 if (output1[0] == Ciphertext2[0] && output1[1] == Ciphertext2[1]) {
   printf("ok\n");
 } else {
   printf("no\n");
}
free(output1);
uint64_t* output2;

output2 = camellia256Encrypt("./aes_plaintext.txt", Key3, "./aes_ciphertext.txt",1);
printf("Ciphertext 256:\n");
 for(int i=0; i < 2; i++){
 	printf("%016llx\n", output2[i]);
 }

 if (output2[0] == Ciphertext3[0] && output2[1] == Ciphertext3[1]) {
   printf("ok\n");
 } else {
   printf("no\n");
}

 free(output2);
 //output = aes128Encrypt("./aes_plaintext.txt",key, "./aes_ciphertext.txt",1);
 /*for(int i=0; i < 32; i++){
 	printf("%02x:", output[i]);
 }*/

 //printf("\n");
 return 0;
}

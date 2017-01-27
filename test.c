#include "aes_cipher.h"
#include "ctr_test_vectors.h"



/* 
   This function load the plaintext divided in blocks 
   from ctr_test_vect.h
*/
// void loadPlaintextBlocks(byte plaintext[NUM_BLOCKS][BLOCK_SIZE]){
// 	#pragma unroll 
// 	for(int k = 0; k < NUM_BLOCKS; k++){
// 		for(int i = 0; i < BLOCK_SIZE; i++){
// 			plaintext[k][i] = plain[k][i]; 
// 		}
// 	}
// }

/*  
    This function load the plaintext divided in blocks 
    from ctr_test_vect.h
*/
// void loadCiphertextBlocks(byte plaintext[NUM_BLOCKS][BLOCK_SIZE]){

//  #pragma unroll 
//  for(int k = 0; k < NUM_BLOCKS; k++){
//   for(int i = 0; i < BLOCK_SIZE; i++){
//    plaintext[k][i] = cipher[k][i]; 
//   }
//  }
// }


int mains(){
   // long num_blocks = 1000;
   
   // byte plaintext[num_blocks][BLOCK_SIZE];
   // byte output[num_blocks][BLOCK_SIZE];
   // byte right[num_blocks][BLOCK_SIZE];
   // loadPlaintextBlocks(plaintext);
   // loadCiphertextBlocks(right);
   // aesCtrEncrypt(plaintext,key,output);
   
   // for(int k = 0; k < NUM_BLOCKS; k++) {
   //   for(int i = 0; i < BLOCK_SIZE; i++) {
   //     printf("%02x:", output[k][i]);
   //   }
   //   printf("\n");
   // }
   // return 0;
   // // check if right
	return 0;
}

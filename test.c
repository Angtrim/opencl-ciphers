#include "aes_cipher.h"
#include "ctr_test_vectors.h"


/*
 * This function load the size in blocks of bytes 
 * of the plaintext generated from the makefile
 */ 
long getPlainNumBlocks(){
	/*FILE *fileptr;
	char *buffer;
	long filelen;

	fileptr = fopen("nullbytes.txt", "rb");  // Open the file in binary mode
	fseek(fileptr, 0, SEEK_END);          // Jump to the end of the file
	filelen = ftell(fileptr);             // Get the current byte offset in the file
	fclose(fileptr); // Close the file*/
}

/* 
   This function load the plaintext divided in blocks 
   from ctr_test_vect.h
*/
void loadPlaintextBlocks(byte plaintext[NUM_BLOCKS][BLOCK_SIZE]){
 	
	
	return 0;
	#pragma unroll 
	for(int k = 0; k < NUM_BLOCKS; k++){
		for(int i = 0; i < BLOCK_SIZE; i++){
		plaintext[k][i] = plain[k][i]; 
		}
	}
}

/*  
    This function load the plaintext divided in blocks 
    from ctr_test_vect.h
*/
void loadCiphertextBlocks(byte plaintext[NUM_BLOCKS][BLOCK_SIZE]){

 #pragma unroll 
 for(int k = 0; k < NUM_BLOCKS; k++){
  for(int i = 0; i < BLOCK_SIZE; i++){
   plaintext[k][i] = cipher[k][i]; 
  }
 }
}


int main(){
   long num_blocks = 1000;
   
   byte plaintext[num_blocks][BLOCK_SIZE];
   byte output[num_blocks][BLOCK_SIZE];
   byte right[num_blocks][BLOCK_SIZE];
   loadPlaintextBlocks(plaintext);
   loadCiphertextBlocks(right);
   aesCtrEncrypt(num_blocks,plaintext,key,output);
   
   for(int k = 0; k < NUM_BLOCKS; k++) {
     for(int i = 0; i < BLOCK_SIZE; i++) {
       printf("%02x:", output[k][i]);
     }
     printf("\n");
   }
   return 0;
   // check if right
}

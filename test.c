// //#include "aes_cipher.h"
// //#include "camellia_cipher.h"
// //#include "aes_cipher.h"
// #include <stdio.h>
// #include <inttypes.h>
// #include <stdlib.h>

// #include "cipher_utils.h"

// //includes for testing normal encryption
// #include "aes_test_vect.h"

// #define MAX_LOCAL_SIZE 64

// #define CPU_DEVICE "CPU"
// #define GPU_DEVICE "GPU"

// // file names for plaintexts of different dimensions
// char* fileNames[6] = {"ctr_plaintext1", "ctr_plaintext2", "ctr_plaintext3", "ctr_plaintext4", "ctr_plaintext5", "ctr_plaintext6" };
// long plainDimensions[6];
// // different local item sizes: number of work-item per work-group
// size_t localSizes[5] = {1, 2, 4, 8, 16};

// // to save ciphertext to file (uint8_t)
// static void writeOutputToFile(char* outFileName,uint8_t* output, long lenght){
// 	FILE* fp = fopen(outFileName, "wb");
// 	if (!fp) {
// 	fprintf(stderr, "Failed to load file.\n");
// 	exit(1);
// 	}
// 	printf("\nwriting %d bytes..\n", lenght);
// 	for(int i = 0; i <16; i++){
// 		printf("%x", output[i]);	
// 	}
// 	fwrite(output, sizeof(char), lenght, fp);
// 	fclose(fp);
// }

// // to save ciphertext to file (uin64_t)
// static void writeOutputToFileUint64(char* outFileName,char* output, long lenght){
// 	FILE* fp = fopen(outFileName, "wb");
// 	if (!fp) {
// 	fprintf(stderr, "Failed to load file.\n");
// 	exit(1);
// 	}
// 	fwrite(output, sizeof(char), lenght, fp);
// 	fclose(fp);
// }

// int main1(){

//  uint8_t* aesCiphertext = (uint8_t*)malloc((16000+1)*sizeof(uint8_t));
//  uint8_t* aesPlaintext;

//  /* -- START NORMAL ENCRYPTION TESTS --*/

//  printf("\n+++NORMAL ENCRYPTION TESTS: START +++\n");

//  /* AES TEST */
 
//  printf("\n################\n");
//  printf("\n## AES CIPHER ##\n");
//  printf("\n################\n");

//  printf("\n||NORMAL ENCRYPTION||\n");
 
//  printf("\n-------------");
//  printf("\n-- AES 128 --\n");
//  printf("-------------\n");
//  printf("\n___CPU TEST___\n");
//  aes128Encrypt("aes_plaintext", aes128Key, aesCiphertext, 2, CPU_DEVICE);
//  if (memcmp(aesCiphertext, aes128Ciphertext, 16000) == 0) {
//  	printf("\n--->[Test for AES 128 is CORRECT]\n");
//  } else {
//  	printf("\n--->[Test for AES 128 is WRONG!]\n");
//  }

//  printf("\n___GPU TEST___\n");
//  aes128Encrypt("aes_plaintext", aes128Key, aesCiphertext, 2, GPU_DEVICE);
//  if (memcmp(aesCiphertext, aes128Ciphertext, 16000) == 0) {
//  	printf("\n--->[Test for AES 128 is CORRECT]\n");
//  } else {
//  	printf("\n--->[Test for AES 128 is WRONG!]\n");
//  }

//  printf("-------------");
//  printf("\n-- AES 192 --\n");
//  printf("-------------\n");
//  printf("\n___CPU TEST___\n");
//  aes192Encrypt("aes_plaintext", aes192Key, aesCiphertext, 2, CPU_DEVICE);
//  if (memcmp(aesCiphertext, aes192Ciphertext, 16000) == 0) {
//  	printf("\n--->[Test for AES 192 is CORRECT]\n");
//  } else {
//  	printf("\n--->[Test for AES 192 is WRONG!]\n");
//  }

//  printf("\n___GPU TEST___\n");
//  aes192Encrypt("aes_plaintext", aes192Key, aesCiphertext, 2, GPU_DEVICE);
//  if (memcmp(aesCiphertext, aes192Ciphertext, 16000) == 0) {
//  	printf("\n--->[Test for AES 192 is CORRECT]\n");
//  } else {
//  	printf("\n--->[Test for AES 192 is WRONG!]\n");
//  }

//  printf("-------------");
//  printf("\n-- AES 256 --\n");
//  printf("-------------\n");
//  printf("\n___CPU TEST___\n");
//  aes256Encrypt("aes_plaintext", aes256Key, aesCiphertext, 2, CPU_DEVICE);
//  if (memcmp(aesCiphertext, aes256Ciphertext, 16000) == 0) {
//  	printf("\n--->[Test for AES 256 is CORRECT]\n");
//  } else {
//  	printf("\n--->[Test for AES 256 is WRONG!]\n");
//  }

//  printf("\n___GPU TEST___\n");
//  aes256Encrypt("aes_plaintext", aes256Key, aesCiphertext, 2, GPU_DEVICE);
//  if (memcmp(aesCiphertext, aes256Ciphertext, 16000) == 0) {
//  	printf("\n--->[Test for AES 256 is CORRECT]\n");
//  } else {
//  	printf("\n--->[Test for AES 256 is WRONG!]\n");
//  }
//  free(aesCiphertext);

//  /* END AES TEST */

//  printf("\n+++NORMAL ENCRYPTION TESTS: END +++\n");

//  /* -- END NORMAL ENCRYPTION TESTS -- */

//  /* -- CTR ENCRYPTION TESTS --*/

//  printf("\n+++CTR ENCRYPTION TESTS: START +++\n");

//  printf("\nLoading plaintexts...\n");
//  for(int i = 0; i < 6; i++){
// 	// get file len in bytes
// 	plainDimensions[i] = getByteLenght(fileNames[i]);
//  }
 
//  /* Testing for different dimensions of plaintext */
//  for(int i = 5; i < 6; i++){

// 	aesCiphertext = (uint8_t*)malloc((plainDimensions[i])*sizeof(uint8_t));
// 	aesPlaintext = (uint8_t*)malloc((plainDimensions[i])*sizeof(uint8_t));

// 	struct FileInfo fileInfo = getFileBytes(fileNames[i]);

// 	/* Tesing for different sizes of work-items per work-group */
// 	for(size_t j = 8; j < 1024; j=j*2){

// 		printf("\nTesting on: %s\n", fileNames[i]);

// 		printf("\nLocal work size: %u (work-items per work-group)\n", j);

// 		printf("\n||CTR ENCRYPTION||\n");
	 	
// 		/* AES CTR TEST */

// 		printf("\n####################\n");
// 		printf("\n## AES CTR CIPHER ##\n");
// 		printf("\n####################\n");

// 		printf("\n-------------");
// 		printf("\n-- AES CTR 128 --\n");
// 		printf("-------------\n");
// 		printf("\n___CPU TEST___\n");
	
// 		printf("\nEncrypting..\n");
// 		aes128CtrEncrypt(fileNames[i], aes128Key, aesCiphertext, j, CPU_DEVICE);

// 		printf("Writing ciphertext on file..\n");	
// 		writeOutputToFile("aes_ciphertext", aesCiphertext, plainDimensions[i]);

// 		printf("\nDecrypting back ciphertext..\n");
// 		aes128CtrDecrypt("aes_ciphertext", aes128Key, aesPlaintext, j, CPU_DEVICE);
	
// 		if (memcmp(aesPlaintext, fileInfo.filePointer, fileInfo.lenght) == 0) {
// 		printf("\n--->[Test for AES CTR 128 is CORRECT]\n");
// 		} else {
// 		printf("\n--->[Test for AES CTR 128 is WRONG!]\n");
// 		}
	
// 		printf("\n___GPU TEST___\n");
	
// 		printf("\nEncrypting..\n");
// 		aes128CtrEncrypt(fileNames[i], aes128Key, aesCiphertext, j, GPU_DEVICE);
		
// 		printf("Writing ciphertext on file..\n");	
// 		writeOutputToFile("aes_ciphertext", aesCiphertext, plainDimensions[i]);

// 		printf("\nDecrypting back ciphertext..\n");
// 		aes128CtrDecrypt("aes_ciphertext", aes128Key, aesPlaintext, j, GPU_DEVICE);
	
// 		if (memcmp(aesPlaintext, fileInfo.filePointer, fileInfo.lenght) == 0) {
// 		printf("\n--->[Test for AES CTR 128 is CORRECT]\n");
// 		} else {
// 		printf("\n--->[Test for AES CTR 128 is WRONG!]\n");
// 		}
	
// 		printf("\n-------------");
// 		printf("\n-- AES CTR 192 --\n");
// 		printf("-------------\n");
// 		printf("\n___CPU TEST___\n");
	
// 		printf("\nEncrypting..\n");
// 		aes192CtrEncrypt(fileNames[i], aes192Key, aesCiphertext, j, CPU_DEVICE);
		
// 		printf("Writing ciphertext on file..\n");	
// 		writeOutputToFile("aes_ciphertext", aesCiphertext, plainDimensions[i]);

// 		printf("\nDecrypting back ciphertext..\n");
// 		aes192CtrDecrypt("aes_ciphertext", aes192Key, aesPlaintext, j, CPU_DEVICE);
	
// 		if (memcmp(aesPlaintext, fileInfo.filePointer, fileInfo.lenght) == 0) {
// 		printf("\n--->[Test for AES CTR 192 is CORRECT]\n");
// 		} else {
// 		printf("\n--->[Test for AES CTR 192 is WRONG!]\n");
// 		}
	
// 		printf("\n___GPU TEST___\n");
	
// 		printf("\nEncrypting..\n");
// 		aes192CtrEncrypt(fileNames[i], aes192Key, aesCiphertext, j, GPU_DEVICE);
		
// 		printf("Writing ciphertext on file..\n");	
// 		writeOutputToFile("aes_ciphertext", aesCiphertext, plainDimensions[i]);

// 		printf("\nDecrypting back ciphertext..\n");
// 		aes192CtrDecrypt("aes_ciphertext", aes192Key, aesPlaintext, j, GPU_DEVICE);
	
// 		if (memcmp(aesPlaintext, fileInfo.filePointer, fileInfo.lenght) == 0) {
// 		printf("\n--->[Test for AES CTR 192 is CORRECT]\n");
// 		} else {
// 		printf("\n--->[Test for AES CTR 192 is WRONG!]\n");
// 		}

// 		printf("\n-------------");
// 		printf("\n-- AES CTR 256 --\n");
// 		printf("-------------\n");
// 		printf("\n___CPU TEST___\n");
	
// 		printf("\nEncrypting..\n");
// 		aes256CtrEncrypt(fileNames[i], aes256Key, aesCiphertext, j, CPU_DEVICE);
		
// 		printf("Writing ciphertext on file..\n");	
// 		writeOutputToFile("aes_ciphertext", aesCiphertext, plainDimensions[i]);

// 		printf("\nDecrypting back ciphertext..\n");
// 		aes256CtrDecrypt("aes_ciphertext", aes256Key, aesPlaintext, j, CPU_DEVICE);
	
// 		if (memcmp(aesPlaintext, fileInfo.filePointer, fileInfo.lenght) == 0) {
// 		printf("\n--->[Test for AES CTR 256 is CORRECT]\n");
// 		} else {
// 		printf("\n--->[Test for AES CTR 256 is WRONG!]\n");
// 		}
	
// 		printf("\n___GPU TEST___\n");
	
// 		printf("\nEncrypting..\n");
// 		aes256CtrEncrypt(fileNames[i], aes256Key, aesCiphertext, j, GPU_DEVICE);
		
// 		printf("Writing ciphertext on file..\n");	
// 		writeOutputToFile("aes_ciphertext", aesCiphertext, plainDimensions[i]);

// 		printf("\nDecrypting back ciphertext..\n");
// 		aes256CtrDecrypt("aes_ciphertext", aes256Key, aesPlaintext, j, GPU_DEVICE);
	
// 		if (memcmp(aesPlaintext, fileInfo.filePointer, fileInfo.lenght) == 0) {
// 		printf("\n--->[Test for AES CTR 256 is CORRECT]\n");
// 		} else {
// 		printf("\n--->[Test for AES CTR 256 is WRONG!]\n");
// 		}
// 	 }
	
// 	free(aesCiphertext);
// 	free(aesPlaintext);
// 	free(fileInfo.filePointer);
//  }

 
 

//  /* END AES CTR TEST */
 
//  /* -- END CTR ENCRYPTION TESTS -- */

//  //TODO: different test cases
// /*
//  uint64_t* output;
//  uint32_t Key[4] = {0x00000000, 0x00000000, 0x00000000, 0x00000000};
//  uint64_t Ciphertext[2] = { 0x5EBAC6E0054E1668ul, 0x19AFF1CC6D346CDBul};

//  output = seed_old_Encrypt("seed_plaintext", Key, "ciphertext.txt", 1, GPU_DEVICE);
//  for(int i=0; i < 2; i++){
//  	printf("%016llx\n", output[i]);
//  }

//  if (output[0] == Ciphertext[0] && output[1] == Ciphertext[1]) {
//    printf("ok\n");
//  } else {
//    printf("no\n");
//  }

//  free(output);

//  output = seed_Encrypt("seed_plaintext", Key, "ciphertext.txt", 1, CPU_DEVICE);
//  for(int i=0; i < 2; i++){
//  	printf("%016llx\n", output[i]);
//  }

//  if (output[0] == Ciphertext[0] && output[1] == Ciphertext[1]) {
//    printf("ok\n");
//  } else {
//    printf("no\n");
//  }

//  free(output);

//  uint64_t Ciphertext2[2] = {0x6767313854966973, 0x0857065648eabe43};
//  uint64_t Key2[2] = {0x0123456789abcdef, 0xfedcba9876543210};
 
//  output = camellia128Encrypt("camellia_plaintext", Key2, "ciphertext.txt", 1, GPU_DEVICE);
//  for(int i=0; i < 2; i++){
//  	printf("%016llx\n", output[i]);
//  }

//  if (output[0] == Ciphertext2[0] && output[1] == Ciphertext2[1]) {
//    printf("ok\n");
//  } else {
//    printf("no\n");
//  }

//  free(output);

//  output = camellia128Encrypt("camellia_plaintext", Key2, "ciphertext.txt", 1, CPU_DEVICE);
//  for(int i=0; i < 2; i++){
//  	printf("%016llx\n", output[i]);
//  }

//  if (output[0] == Ciphertext2[0] && output[1] == Ciphertext2[1]) {
//    printf("ok\n");
//  } else {
//    printf("no\n");
//  }

//  free(output);

//  uint64_t Key3[2] = {0, 0};
//  uint64_t ptx = 0; 
//  uint64_t correct = 0x5579c1387b228445;

//  output = present_memory_encrypt("present_plaintext", Key3, "ciphertext.txt", 1, GPU_DEVICE);
//  for(int i=0; i < 1; i++){
//  	printf("%016llx\n", output[i]);
//  }

//  if (output[0] == correct) {
//    printf("ok\n");
//  } else {
//    printf("no\n");
//  }
 
//  free(output);

//  output = present_memory_CtrEncrypt("present_plaintext", Key3, "ciphertext.txt", 1, GPU_DEVICE);
//  for(int i=0; i < 1; i++){
//  	printf("%016llx\n", output[i]);
//  }

//  if (output[0] == correct) {
//    printf("ok\n");
//  } else {
//    printf("no\n");
//  }

//  free(output);

//  uint8_t K[16] = {
//     0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77,
//     0x88, 0x99, 0xaa, 0xbb, 0xcc, 0xdd, 0xee, 0xff
//   };

//   uint64_t P[2] = {
//     0x0123456789abcdefUL,
//     0xfedcba9876543210UL
//   };

//   uint64_t C[2] = {
//     0x8b1da5f56ab3d07cUL,
//     0x04b68240b13be95dUL
//   };

//  output = misty1Encrypt("misty_plaintext", K, "ciphertext.txt" ,1, GPU_DEVICE);

//  for(int i=0; i < 2; i++){
//  	printf("%016llx\n", output[i]);
//  }
 
//  if (output[0] == C[0] && output[1] == C[1]) {
//    printf("ok\n");
//  } else {
//    printf("no\n");
//  }
 
 
// /*
//  uint64_t* output;
//  uint64_t Key1[2] = {0x0123456789abcdef, 0xfedcba9876543210};
//  uint64_t Ciphertext1[4] = {0x6767313854966973, 0x0857065648eabe43, 0x6767313854966973, 0x0857065648eabe43};
//  uint64_t Ciphertext2[2] = {0xb4993401b3e996f8, 0x4ee5cee7d79b09b9};
//  uint64_t Key2[3] = {0x0123456789abcdef, 0xfedcba9876543210,
//                      0x0011223344556677};
//  uint64_t Ciphertext3[2] = {0x9acc237dff16d76c, 0x20ef7c919e3a7509};
//  uint64_t Key3[4] = {0x0123456789abcdef, 0xfedcba9876543210,
//                      0x0011223344556677, 0x8899aabbccddeeff};
//  output = camellia128Encrypt("./camellia_plaintext.txt", Key1, "./camellia_ciphertext.txt",1);
//  //uint32_t key[32] = {0x63375233, 0xca899f06, 0x2f868c91, 0x8eb2ccc1, 0x36eb5001, 0x58c75cbb, 0x2ee60020, 0x3286d86f};
	
//  //output = aes256Encrypt("./aes_plaintext.txt",key, "./aes_ciphertext.txt",1);
//  printf("Ciphertext 128:\n");
//  for(int i=0; i < 4; i++){
//  	printf("%016llx\n", output[i]);
//  }

//  if (output[0] == Ciphertext1[0] && output[1] == Ciphertext1[1] && output[2] == Ciphertext1[2] && output[3] == Ciphertext1[3]) {
//    printf("ok\n");
//  } else {
//    printf("no\n");
// }
// free(output);
// uint64_t* output1;

// output1 = camellia192Encrypt("./aes_plaintext.txt", Key2, "./aes_ciphertext.txt",1);
// printf("Ciphertext 192:\n");
//  for(int i=0; i < 2; i++){
//  	printf("%016llx\n", output1[i]);
//  }

//  if (output1[0] == Ciphertext2[0] && output1[1] == Ciphertext2[1]) {
//    printf("ok\n");
//  } else {
//    printf("no\n");
// }
// free(output1);
// uint64_t* output2;

// output2 = camellia256Encrypt("./aes_plaintext.txt", Key3, "./aes_ciphertext.txt",1);
// printf("Ciphertext 256:\n");
//  for(int i=0; i < 2; i++){
//  	printf("%016llx\n", output2[i]);
//  }

//  if (output2[0] == Ciphertext3[0] && output2[1] == Ciphertext3[1]) {
//    printf("ok\n");
//  } else {
//    printf("no\n");
// }

//  free(output2);
//  //output = aes128Encrypt("./aes_plaintext.txt",key, "./aes_ciphertext.txt",1);
//  /*for(int i=0; i < 32; i++){
//  	printf("%02x:", output[i]);
//  }*/

//  //printf("\n");
//  return 0;
// }

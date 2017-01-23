#include "benchmark.h"
#include <stdint.h>
#include <assert.h>

#ifdef __APPLE__
#include <OpenCL/opencl.h>
#else
#include <CL/cl.h>
#endif

//#define __CTR_MODE__
#define WORK_GROUP_SIZE 1
#define __LITTLE_ENDIAN__

#define MEM_SIZE (128)
#define MAX_SOURCE_SIZE (0x100000)

#define BLOCK_SIZE (128 / 8)
#define AES128

#if defined(AES128)
#  define KEYSIZE 128
#  define NAME "AES 128"
#elif defined(AES192)
#  define KEYSIZE 192
#  define NAME "AES 192"
#elif defined(AES256)
#  define KEYSIZE 256
#  define NAME "AES 256"
#else
#  error "Define AES128, AES192 or AES256!"
#endif

#if KEYSIZE == 128
#  define Nk 4
#  define Nb 4
#  define Nr 10
#  define AES_128
#elif KEYSIZE == 192
#  define Nk 6
#  define Nb 4
#  define Nr 12
#  define AES_192
#elif KEYSIZE == 256
#  define Nk 8
#  define Nb 4
#  define Nr 14
#  define AES_256
#else
#  error "Wrong keysize, must be 128, 192 or 256"
#endif

// The following definitions are not essential but they allow us to keep the
// implementation as close to the FIPS document as possible.

typedef uint32_t word;
typedef uint8_t byte;

#define makeword(a, b, c, d) \
  (((word)(a) << 24) | ((word)(b) << 16) | ((word)(c) << 8) | (d))

////////////////////////////////////////////////////////////////////////////////

const byte SBox[0x100] __attribute__((sbox)) = {
  0x63, 0x7c, 0x77, 0x7b, 0xf2, 0x6b, 0x6f, 0xc5,
  0x30, 0x01, 0x67, 0x2b, 0xfe, 0xd7, 0xab, 0x76,
  0xca, 0x82, 0xc9, 0x7d, 0xfa, 0x59, 0x47, 0xf0,
  0xad, 0xd4, 0xa2, 0xaf, 0x9c, 0xa4, 0x72, 0xc0,
  0xb7, 0xfd, 0x93, 0x26, 0x36, 0x3f, 0xf7, 0xcc,
  0x34, 0xa5, 0xe5, 0xf1, 0x71, 0xd8, 0x31, 0x15,
  0x04, 0xc7, 0x23, 0xc3, 0x18, 0x96, 0x05, 0x9a,
  0x07, 0x12, 0x80, 0xe2, 0xeb, 0x27, 0xb2, 0x75,
  0x09, 0x83, 0x2c, 0x1a, 0x1b, 0x6e, 0x5a, 0xa0,
  0x52, 0x3b, 0xd6, 0xb3, 0x29, 0xe3, 0x2f, 0x84,
  0x53, 0xd1, 0x00, 0xed, 0x20, 0xfc, 0xb1, 0x5b,
  0x6a, 0xcb, 0xbe, 0x39, 0x4a, 0x4c, 0x58, 0xcf,
  0xd0, 0xef, 0xaa, 0xfb, 0x43, 0x4d, 0x33, 0x85,
  0x45, 0xf9, 0x02, 0x7f, 0x50, 0x3c, 0x9f, 0xa8,
  0x51, 0xa3, 0x40, 0x8f, 0x92, 0x9d, 0x38, 0xf5,
  0xbc, 0xb6, 0xda, 0x21, 0x10, 0xff, 0xf3, 0xd2,
  0xcd, 0x0c, 0x13, 0xec, 0x5f, 0x97, 0x44, 0x17,
  0xc4, 0xa7, 0x7e, 0x3d, 0x64, 0x5d, 0x19, 0x73,
  0x60, 0x81, 0x4f, 0xdc, 0x22, 0x2a, 0x90, 0x88,
  0x46, 0xee, 0xb8, 0x14, 0xde, 0x5e, 0x0b, 0xdb,
  0xe0, 0x32, 0x3a, 0x0a, 0x49, 0x06, 0x24, 0x5c,
  0xc2, 0xd3, 0xac, 0x62, 0x91, 0x95, 0xe4, 0x79,
  0xe7, 0xc8, 0x37, 0x6d, 0x8d, 0xd5, 0x4e, 0xa9,
  0x6c, 0x56, 0xf4, 0xea, 0x65, 0x7a, 0xae, 0x08,
  0xba, 0x78, 0x25, 0x2e, 0x1c, 0xa6, 0xb4, 0xc6,
  0xe8, 0xdd, 0x74, 0x1f, 0x4b, 0xbd, 0x8b, 0x8a,
  0x70, 0x3e, 0xb5, 0x66, 0x48, 0x03, 0xf6, 0x0e,
  0x61, 0x35, 0x57, 0xb9, 0x86, 0xc1, 0x1d, 0x9e,
  0xe1, 0xf8, 0x98, 0x11, 0x69, 0xd9, 0x8e, 0x94,
  0x9b, 0x1e, 0x87, 0xe9, 0xce, 0x55, 0x28, 0xdf,
  0x8c, 0xa1, 0x89, 0x0d, 0xbf, 0xe6, 0x42, 0x68,
  0x41, 0x99, 0x2d, 0x0f, 0xb0, 0x54, 0xbb, 0x16
};

const byte Rcon[0x100] = {
  0x8d, 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40,
  0x80, 0x1b, 0x36, 0x6c, 0xd8, 0xab, 0x4d, 0x9a,
  0x2f, 0x5e, 0xbc, 0x63, 0xc6, 0x97, 0x35, 0x6a,
  0xd4, 0xb3, 0x7d, 0xfa, 0xef, 0xc5, 0x91, 0x39,
  0x72, 0xe4, 0xd3, 0xbd, 0x61, 0xc2, 0x9f, 0x25,
  0x4a, 0x94, 0x33, 0x66, 0xcc, 0x83, 0x1d, 0x3a,
  0x74, 0xe8, 0xcb, 0x8d, 0x01, 0x02, 0x04, 0x08,
  0x10, 0x20, 0x40, 0x80, 0x1b, 0x36, 0x6c, 0xd8,
  0xab, 0x4d, 0x9a, 0x2f, 0x5e, 0xbc, 0x63, 0xc6,
  0x97, 0x35, 0x6a, 0xd4, 0xb3, 0x7d, 0xfa, 0xef,
  0xc5, 0x91, 0x39, 0x72, 0xe4, 0xd3, 0xbd, 0x61,
  0xc2, 0x9f, 0x25, 0x4a, 0x94, 0x33, 0x66, 0xcc,
  0x83, 0x1d, 0x3a, 0x74, 0xe8, 0xcb, 0x8d, 0x01,
  0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x1b,
  0x36, 0x6c, 0xd8, 0xab, 0x4d, 0x9a, 0x2f, 0x5e,
  0xbc, 0x63, 0xc6, 0x97, 0x35, 0x6a, 0xd4, 0xb3,
  0x7d, 0xfa, 0xef, 0xc5, 0x91, 0x39, 0x72, 0xe4,
  0xd3, 0xbd, 0x61, 0xc2, 0x9f, 0x25, 0x4a, 0x94,
  0x33, 0x66, 0xcc, 0x83, 0x1d, 0x3a, 0x74, 0xe8,
  0xcb, 0x8d, 0x01, 0x02, 0x04, 0x08, 0x10, 0x20,
  0x40, 0x80, 0x1b, 0x36, 0x6c, 0xd8, 0xab, 0x4d,
  0x9a, 0x2f, 0x5e, 0xbc, 0x63, 0xc6, 0x97, 0x35,
  0x6a, 0xd4, 0xb3, 0x7d, 0xfa, 0xef, 0xc5, 0x91,
  0x39, 0x72, 0xe4, 0xd3, 0xbd, 0x61, 0xc2, 0x9f,
  0x25, 0x4a, 0x94, 0x33, 0x66, 0xcc, 0x83, 0x1d,
  0x3a, 0x74, 0xe8, 0xcb, 0x8d, 0x01, 0x02, 0x04,
  0x08, 0x10, 0x20, 0x40, 0x80, 0x1b, 0x36, 0x6c,
  0xd8, 0xab, 0x4d, 0x9a, 0x2f, 0x5e, 0xbc, 0x63,
  0xc6, 0x97, 0x35, 0x6a, 0xd4, 0xb3, 0x7d, 0xfa,
  0xef, 0xc5, 0x91, 0x39, 0x72, 0xe4, 0xd3, 0xbd,
  0x61, 0xc2, 0x9f, 0x25, 0x4a, 0x94, 0x33, 0x66,
  0xcc, 0x83, 0x1d, 0x3a, 0x74, 0xe8, 0xcb, 0x8d
};

//This function adds two string pointers together
char* stradd(const char* a, const char* b){
	size_t len = strlen(a) + strlen(b);
	char *ret = (char*)malloc(len * sizeof(char) + 1);
	*ret = '\0';
	return strcat(strcat(ret, a) ,b);
} 

// No table mult because protection is heavy

// N is MSB, 0 is LSB
#define byte(x, n) (((x) >> 8*(n)) & 0xff)

#define RotWord(x) (((x) << 8) | ((x) >> (32 - 8)))

#define SubWord(x) \
  makeword(SBox[byte(x, 3)], \
           SBox[byte(x, 2)], \
           SBox[byte(x, 1)], \
           SBox[byte(x, 0)])

// Combined operation for efficiency
#define SubWord_Rcon(x, i) \
  makeword(SBox[byte(x, 3)] ^ Rcon[i/Nk], \
           SBox[byte(x, 2)], \
           SBox[byte(x, 1)], \
           SBox[byte(x, 0)])

#define byteswap(x) \
  makeword(byte(x, 0), byte(x, 1), byte(x, 2), byte(x, 3))


void KeyExpansion(word key[Nk], word w[Nb*(Nr+1)]) {
	#pragma unroll
	for (int i = 0; i < Nk; ++i) {
	w[i] = key[i];
	}

	#pragma unroll
	for (int i = Nk; i < Nb * (Nr + 1); ++i) {
	word temp = w[i-1];

	if (i % Nk == 0) {
	  temp = SubWord_Rcon(RotWord(temp), i);
	} else if (Nk > 6 && i % Nk == 4) {
	  temp = SubWord(temp);
	}

	w[i] = w[i-Nk] ^ temp;
	}
	
	#ifdef __LITTLE_ENDIAN__
	#  warning "Little endian platform detected"
	  // Flip key on little endian platforms
	  #pragma unroll
	  for (int i = 0; i < Nb*(Nr+1); ++i) {
	    w[i] = byteswap(w[i]);
	  }
	#endif
}

void aes_expand(word key[Nk] __attribute__((key)),
                        byte in[4*Nb] __attribute__((plain)),
                        byte out[4*Nb]) {
	  word w[Nb*(Nr+1)];
	  BEGIN_KEYSCHED;
	  KeyExpansion(key, w);
	  END_KEYSCHED;
}

#include "ctr_test_vectors.h"

// This function load the plaintext divided in blocks 
// from ctr_test_vect.h
void loadBlocks(byte plaintext[NUM_BLOCKS][BLOCK_SIZE]){
	
	#pragma unroll 
	for(int k = 0; k < NUM_BLOCKS; k++){
		for(int i = 0; i < BLOCK_SIZE; i++){
			plaintext[k][i] = plain[k][i]; 
		}
	}
}

#include <stdio.h>
#include <string.h>
#include <stdbool.h>

int main() {
	//opencl parameters initialization
	//to run the kernel
	cl_device_id device_id = NULL;
	cl_context context = NULL;
	cl_command_queue command_queue = NULL;

	cl_mem out = NULL;
	cl_mem in = NULL;
	cl_mem exKey = NULL;

	cl_program program = NULL;
	cl_kernel kernel = NULL;
	cl_platform_id platform_id = NULL;
	cl_uint ret_num_devices;
	cl_uint ret_num_platforms;
	cl_int ret;
	
	// plaintext divided in blocks of dim BLOCK_SIZE
	byte plaintext[NUM_BLOCKS][BLOCK_SIZE];
	
	loadBlocks(plaintext);
	
	// number of work-items equal to the number of blocks
	size_t global_item_size = NUM_BLOCKS;
	
	#ifdef __CTR_MODE__
		// number of work-item per work-group
		size_t local_item_size = WORK_GROUP_SIZE;
	#else
		// number of work-item per work-group
		size_t local_item_size = 1;
	#endif
	
	
	byte output[NUM_BLOCKS][BLOCK_SIZE];
	
	//key expansion is performed on cpu
    int exKeyDim = Nb*(Nr+1);
	word w[exKeyDim];
	BEGIN_KEYSCHED;
	KeyExpansion(key, w);
	END_KEYSCHED;

	FILE *fp;
	char fileName[] = "./aes.cl";

	//to pass the constant parameter Nb and mode of operation flag
	char *append_str = "";
	#ifdef __CTR_MODE__
	append_str = stradd(append_str, "#define __CTR_MODE__\n");
	#else
	#endif
	char temp1[10];
	// pass Nb and Nr to opencl file
	append_str = stradd(append_str, "#define Nb");
	sprintf(temp1, " %d\n", Nb); // puts string into buffer
	append_str = stradd(append_str, temp1);
	append_str = stradd(append_str, "#define Nr");
	sprintf(temp1, " %d\n", Nr); // puts string into buffer
	append_str = stradd(append_str, temp1);
	// pass NUM_BLOCKS and BLOCK_SIZE to opencl file
	append_str = stradd(append_str, "#define NUM_BLOCKS");
	sprintf(temp1, " %d\n", NUM_BLOCKS); // puts string into buffer
	append_str = stradd(append_str, temp1);
	append_str = stradd(append_str, "#define BLOCK_SIZE");
	sprintf(temp1, " %d\n", BLOCK_SIZE); // puts string into buffer
	append_str = stradd(append_str, temp1);
	printf("%s\n", append_str);
	
	char *source_str;
	size_t source_size;
	 
	/* Load the source code containing the kernel*/
	fp = fopen(fileName, "r");
	if (!fp) {
	fprintf(stderr, "Failed to load kernel.\n");
	exit(1);
	}
	source_str = (char*)malloc(MAX_SOURCE_SIZE);
	fread(source_str, 1, MAX_SOURCE_SIZE, fp);
	fclose(fp);

	append_str = stradd(append_str, source_str);
	source_size = strlen(append_str);

	/* Get Platform and Device Info */
	ret = clGetPlatformIDs(1, &platform_id, &ret_num_platforms);
	ret = clGetDeviceIDs(platform_id, CL_DEVICE_TYPE_GPU, 1, &device_id, &ret_num_devices);
	 
	/* Create OpenCL context */
	context = clCreateContext(NULL, 1, &device_id, NULL, NULL, &ret);
	 
	/* Create Command Queue */
	command_queue = clCreateCommandQueue(context, device_id, 0, &ret);
	 
	/* Create Memory Buffers */
	in = clCreateBuffer(context, CL_MEM_READ_WRITE, NUM_BLOCKS * BLOCK_SIZE * sizeof(byte), NULL, &ret);
	exKey = clCreateBuffer(context, CL_MEM_READ_WRITE, exKeyDim * sizeof(word), NULL, &ret); 
	out = clCreateBuffer(context, CL_MEM_READ_WRITE, NUM_BLOCKS * BLOCK_SIZE * sizeof(byte), NULL, &ret);

	/* Copy input data to Memory Buffer */
	ret = clEnqueueWriteBuffer(command_queue, in, CL_TRUE, 0, NUM_BLOCKS * BLOCK_SIZE * sizeof(byte), plaintext, 0, NULL, NULL);
	ret = clEnqueueWriteBuffer(command_queue, exKey, CL_TRUE, 0, exKeyDim * sizeof(word), w, 0, NULL, NULL);

	/* Create Kernel Program from the source */
	program = clCreateProgramWithSource(context, 1, (const char **)&append_str,
	(const size_t *)&source_size, &ret);
	 
	/* Build Kernel Program */
	ret = clBuildProgram(program, 1, &device_id, NULL, NULL, NULL);
	if(ret != CL_SUCCESS){
	 printf("\nBuild Error = %i", ret);
			// Determine the size of the log
			size_t log_size;
			clGetProgramBuildInfo(program, device_id, CL_PROGRAM_BUILD_LOG, 0, NULL, &log_size);

			// Allocate memory for the log
			char *log = (char *) malloc(log_size);

			// Get the log
			clGetProgramBuildInfo(program, device_id, CL_PROGRAM_BUILD_LOG, log_size, log, NULL);

			// Print the log
	printf("%s\n", log);
	}
	 
	/* Create OpenCL Kernel */
	kernel = clCreateKernel(program, "aesCipher", &ret);
	 
	/* Set OpenCL Kernel Parameters */
	ret = clSetKernelArg(kernel, 0, sizeof(cl_mem), (void *)&in);
	ret = clSetKernelArg(kernel, 1, sizeof(cl_mem), (void *)&exKey);
	ret = clSetKernelArg(kernel, 2, sizeof(cl_mem), (void *)&out);
	 
	/* Execute OpenCL Kernel instances */
	ret = clEnqueueNDRangeKernel(command_queue, kernel, 1, NULL, &global_item_size, &local_item_size, 0, NULL,NULL);
	 
	/* Copy results from the memory buffer */
	ret = clEnqueueReadBuffer(command_queue, out, CL_TRUE, 0,
	NUM_BLOCKS * BLOCK_SIZE * sizeof(byte),output, 0, NULL, NULL);
	 
	/* Finalization */
	ret = clFlush(command_queue);
	ret = clFinish(command_queue);
	ret = clReleaseKernel(kernel);
	ret = clReleaseProgram(program);
	ret = clReleaseMemObject(in);
	ret = clReleaseMemObject(exKey);
	ret = clReleaseMemObject(out);
	ret = clReleaseCommandQueue(command_queue);
	ret = clReleaseContext(context);
	 
	free(source_str);

    #ifdef __CTR_MODE__ 
		/* -- testing enctryption for the cipher in ctr mode -- */
		for(int k = 0; k < NUM_BLOCKS; k++){
			if (memcmp(output[k], cipher[k], BLOCK_SIZE) == 0) {
			/* Display Result */
			for(int i = 0; i < BLOCK_SIZE; i++){
				printf("%02x:", output[k][i]);
			}
			PASS;
			} else {
			FAIL;
			}
		}
    #else
		/* -- testing encryption for the cipher not in ctr mode -- */
		for(int k = 0; k < NUM_BLOCKS; k++){
			if (memcmp(output[k], cipher[k], BLOCK_SIZE) == 0) {
			/* Display Result */
			for(int i = 0; i < BLOCK_SIZE; i++){
				printf("%02x:", output[k][i]);
			}
			PASS;
			} else {
			FAIL;
			}
		}
    #endif
		// testing
		for(int k = 0; k < NUM_BLOCKS; k++){
			if (memcmp(output[k], cipher[k], BLOCK_SIZE) == 0) {
			/* Display Result */
			for(int i = 0; i < BLOCK_SIZE; i++){
				printf("%02x:", output[k][i]);
			}
			PASS;
			} else {
			FAIL;
			}
		}
	
	//}

	#if 0
	  byte output[BLOCK_SIZE];
	  bool passed = true;

	  for (int i = 0; i < NUM_CIPHER; ++i) {
		aes_expand_encrypt(key, plain[i], output);
		if (memcmp(output, cipher[i], BLOCK_SIZE) != 0) {
		  passed = false;
		  break;
		}
	  }

	  if (passed) {
		printf("OK\n");
	  } else {
		printf("NO\n");
	  }
	#endif

	return 0;
}

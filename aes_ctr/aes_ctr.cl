
#define BLOCK_SIZE (128 / 8)

__constant uchar sbox[256] = {
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

//nonce added to the counter in ctr mode
__constant uchar nonce[8] = {
  0xf1, 0x42, 0x61, 0xbb, 0xfc, 0x34, 0xc5, 0xe9
};

__kernel void subBytes(__local uchar* s){

  #pragma unroll
  for (int i = 0; i < 4*Nb; ++i) {
    s[i] = sbox[s[i]];
  }
  /*s[0] = sbox[s[0]];s[1] = sbox[s[1]];s[2] = sbox[s[2]];
  s[3] = sbox[s[3]];s[4] = sbox[s[4]];s[5] = sbox[s[5]];
  s[6] = sbox[s[6]];s[7] = sbox[s[7]];s[8] = sbox[s[8]];
  s[9] = sbox[s[9]];s[10] = sbox[s[10]];s[11] = sbox[s[11]];
  s[12] = sbox[s[12]];s[13] = sbox[s[13]];s[14] = sbox[s[14]];
  s[15] = sbox[s[15]];*/
}

__kernel void mixColumns(__local uchar* arr){
  
  #pragma unroll
  for (int i = 0; i < 4; ++i) {

    __local uchar *r = arr + 4*i;

    uchar a[4];
    uchar b[4];

    uchar h;

    #pragma unroll
    for(int c = 0; c < 4; ++c) {
      a[c] = r[c];
      h = (uchar)((signed char)r[c] >> 7);
      b[c] = r[c] << 1;
      b[c] ^= 0x1B & h;
    }

    r[0] = b[0] ^ a[3] ^ a[2] ^ b[1] ^ a[1];
    r[1] = b[1] ^ a[0] ^ a[3] ^ b[2] ^ a[2];
    r[2] = b[2] ^ a[1] ^ a[0] ^ b[3] ^ a[3];
    r[3] = b[3] ^ a[2] ^ a[1] ^ b[0] ^ a[0];
  }
}

__kernel void shiftRows(__local uchar* s){
  
  uchar t;
  t = s[ 1]; s[ 1] = s[ 5]; s[ 5] = s[ 9]; s[ 9] = s[13]; s[13] = t;
  t = s[ 2]; s[ 2] = s[10]; s[10] = t; t = s[ 6]; s[ 6] = s[14]; s[14] = t;
  t = s[ 7]; s[ 7] = s[ 3]; s[ 3] = s[15]; s[15] = s[11]; s[11] = t;
}

__kernel void addRoundKey(__local uchar* state,__local uint* w, int i){
  
  __local uint* s = (__local uint*) state;

  #pragma unroll
  for (int j = 0; j < Nb; ++j) {
    s[j] ^= w[i + j];
  }
} 

__kernel void encrypt(__local uchar state[BLOCK_SIZE], __local uint *w, __local uchar out[BLOCK_SIZE]){
  
  addRoundKey(state, w, 0);

  #pragma unroll
  for (int round = 1; round < Nr; ++round) {
    subBytes(state);
    shiftRows(state); 
    mixColumns(state); 
    addRoundKey(state, w, round*Nb);
  }

  subBytes(state);
  shiftRows(state);
  addRoundKey(state, w, Nr*Nb);

  #pragma unroll
  for (int i = 0; i < 4*Nb; ++i) {
    out[i] = state[i];
  }
}

__kernel void aesCipher(__global uchar* in, __global uint *w, __global uchar* out){

  __local int gid;
  gid = get_global_id(0); 

  __local int local_gid;
  local_gid = get_local_id(0);

  __local int localSize;
  localSize = get_local_size(0);

  /*if(gid == 0){
	for(int i = 0; i < 16000; i++){
	printf("%x", in[i]);
  }
  }*/
  

  //printf("gid: %d", gid);

  __local uchar state[LOCAL_SIZE][4*Nb]; 
  __local uint _w[Nb*(Nr+1)];
  
  
  #pragma unroll
  for (int i = 0; i < 4*Nb; ++i) {
    int offset = gid * BLOCK_SIZE + i;
    state[local_gid][i] = in[offset];
    
  }

  #pragma unroll
  for (int i = 0; i < Nb*(Nr+1); ++i) {
    _w[i] = w[i];
  }  

  /* call encrypt and get output */
  __local uchar outCipher[BLOCK_SIZE];
  encrypt(state[local_gid], _w, outCipher);
  
  #pragma unroll
  for(int i = 0; i < BLOCK_SIZE; i++) {
    int offset = gid * BLOCK_SIZE + i;
    out[offset] = outCipher[i];
  } 
}



__kernel void aesCipherCtr(__global uchar* in, __global uint *w, __global uchar* out){

  __local int gid;
  gid = get_global_id(0);

  
  	printf("gid: %d", gid);
  
   

  /* Create input for aesCipher */
  __local uchar counter[16];
  __local uint _w[Nb*(Nr+1)];
   
  /* Initialize local variable for key */
  #pragma unroll
  for (int i = 0; i < Nb*(Nr+1); ++i) {
    _w[i] = w[i];
  }  

  /* -- initialize counter -- */
  #pragma unroll
  for(int k = 0; k < 8; k++){
    counter[k] = nonce[k];
  }

  __local uchar countBytes[8]; 
  //initialize counter
  countBytes[0] = (char)0;
  countBytes[1] = (char)0;
  countBytes[2] = (char)0;
  countBytes[3] = (char)0;
  countBytes[4] = (char)0;
  countBytes[5] = (char)0;
  countBytes[6] = (char)0;
  countBytes[7] = (char)0;
  int n = 8, c = gid;

  /* increment the counter by gid */
  do {
    --n;
    c += countBytes[n];
    countBytes[n] = (char)c;
    c >>= 8;
  } while (n);
  #pragma unroll
  for(int k = 8; k < 16; k++){
    counter[k] = countBytes[k-8];
  }  

  /* call encrypt and get output */
  __local uchar outCipher[BLOCK_SIZE];
  encrypt(counter,_w,outCipher);


  /* final xor */
  #pragma unroll
  for (int i = 0; i < 4*Nb; ++i) {
    int offset = gid * BLOCK_SIZE + i;
    out[offset] = outCipher[i] ^ in[offset];
  }
}

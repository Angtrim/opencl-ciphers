/*
 * Expanded DES S-boxes
 */
__constant uint SB1[64] = {
    0x01010400, 0x00000000, 0x00010000, 0x01010404, 0x01010004, 0x00010404,
    0x00000004, 0x00010000, 0x00000400, 0x01010400, 0x01010404, 0x00000400,
    0x01000404, 0x01010004, 0x01000000, 0x00000004, 0x00000404, 0x01000400,
    0x01000400, 0x00010400, 0x00010400, 0x01010000, 0x01010000, 0x01000404,
    0x00010004, 0x01000004, 0x01000004, 0x00010004, 0x00000000, 0x00000404,
    0x00010404, 0x01000000, 0x00010000, 0x01010404, 0x00000004, 0x01010000,
    0x01010400, 0x01000000, 0x01000000, 0x00000400, 0x01010004, 0x00010000,
    0x00010400, 0x01000004, 0x00000400, 0x00000004, 0x01000404, 0x00010404,
    0x01010404, 0x00010004, 0x01010000, 0x01000404, 0x01000004, 0x00000404,
    0x00010404, 0x01010400, 0x00000404, 0x01000400, 0x01000400, 0x00000000,
    0x00010004, 0x00010400, 0x00000000, 0x01010004};

__constant uint SB2[64] = {
    0x80108020, 0x80008000, 0x00008000, 0x00108020, 0x00100000, 0x00000020,
    0x80100020, 0x80008020, 0x80000020, 0x80108020, 0x80108000, 0x80000000,
    0x80008000, 0x00100000, 0x00000020, 0x80100020, 0x00108000, 0x00100020,
    0x80008020, 0x00000000, 0x80000000, 0x00008000, 0x00108020, 0x80100000,
    0x00100020, 0x80000020, 0x00000000, 0x00108000, 0x00008020, 0x80108000,
    0x80100000, 0x00008020, 0x00000000, 0x00108020, 0x80100020, 0x00100000,
    0x80008020, 0x80100000, 0x80108000, 0x00008000, 0x80100000, 0x80008000,
    0x00000020, 0x80108020, 0x00108020, 0x00000020, 0x00008000, 0x80000000,
    0x00008020, 0x80108000, 0x00100000, 0x80000020, 0x00100020, 0x80008020,
    0x80000020, 0x00100020, 0x00108000, 0x00000000, 0x80008000, 0x00008020,
    0x80000000, 0x80100020, 0x80108020, 0x00108000};

__constant uint SB3[64] = {
    0x00000208, 0x08020200, 0x00000000, 0x08020008, 0x08000200, 0x00000000,
    0x00020208, 0x08000200, 0x00020008, 0x08000008, 0x08000008, 0x00020000,
    0x08020208, 0x00020008, 0x08020000, 0x00000208, 0x08000000, 0x00000008,
    0x08020200, 0x00000200, 0x00020200, 0x08020000, 0x08020008, 0x00020208,
    0x08000208, 0x00020200, 0x00020000, 0x08000208, 0x00000008, 0x08020208,
    0x00000200, 0x08000000, 0x08020200, 0x08000000, 0x00020008, 0x00000208,
    0x00020000, 0x08020200, 0x08000200, 0x00000000, 0x00000200, 0x00020008,
    0x08020208, 0x08000200, 0x08000008, 0x00000200, 0x00000000, 0x08020008,
    0x08000208, 0x00020000, 0x08000000, 0x08020208, 0x00000008, 0x00020208,
    0x00020200, 0x08000008, 0x08020000, 0x08000208, 0x00000208, 0x08020000,
    0x00020208, 0x00000008, 0x08020008, 0x00020200};

__constant uint SB4[64] = {
    0x00802001, 0x00002081, 0x00002081, 0x00000080, 0x00802080, 0x00800081,
    0x00800001, 0x00002001, 0x00000000, 0x00802000, 0x00802000, 0x00802081,
    0x00000081, 0x00000000, 0x00800080, 0x00800001, 0x00000001, 0x00002000,
    0x00800000, 0x00802001, 0x00000080, 0x00800000, 0x00002001, 0x00002080,
    0x00800081, 0x00000001, 0x00002080, 0x00800080, 0x00002000, 0x00802080,
    0x00802081, 0x00000081, 0x00800080, 0x00800001, 0x00802000, 0x00802081,
    0x00000081, 0x00000000, 0x00000000, 0x00802000, 0x00002080, 0x00800080,
    0x00800081, 0x00000001, 0x00802001, 0x00002081, 0x00002081, 0x00000080,
    0x00802081, 0x00000081, 0x00000001, 0x00002000, 0x00800001, 0x00002001,
    0x00802080, 0x00800081, 0x00002001, 0x00002080, 0x00800000, 0x00802001,
    0x00000080, 0x00800000, 0x00002000, 0x00802080};

__constant uint SB5[64] = {
    0x00000100, 0x02080100, 0x02080000, 0x42000100, 0x00080000, 0x00000100,
    0x40000000, 0x02080000, 0x40080100, 0x00080000, 0x02000100, 0x40080100,
    0x42000100, 0x42080000, 0x00080100, 0x40000000, 0x02000000, 0x40080000,
    0x40080000, 0x00000000, 0x40000100, 0x42080100, 0x42080100, 0x02000100,
    0x42080000, 0x40000100, 0x00000000, 0x42000000, 0x02080100, 0x02000000,
    0x42000000, 0x00080100, 0x00080000, 0x42000100, 0x00000100, 0x02000000,
    0x40000000, 0x02080000, 0x42000100, 0x40080100, 0x02000100, 0x40000000,
    0x42080000, 0x02080100, 0x40080100, 0x00000100, 0x02000000, 0x42080000,
    0x42080100, 0x00080100, 0x42000000, 0x42080100, 0x02080000, 0x00000000,
    0x40080000, 0x42000000, 0x00080100, 0x02000100, 0x40000100, 0x00080000,
    0x00000000, 0x40080000, 0x02080100, 0x40000100};

__constant uint SB6[64] = {
    0x20000010, 0x20400000, 0x00004000, 0x20404010, 0x20400000, 0x00000010,
    0x20404010, 0x00400000, 0x20004000, 0x00404010, 0x00400000, 0x20000010,
    0x00400010, 0x20004000, 0x20000000, 0x00004010, 0x00000000, 0x00400010,
    0x20004010, 0x00004000, 0x00404000, 0x20004010, 0x00000010, 0x20400010,
    0x20400010, 0x00000000, 0x00404010, 0x20404000, 0x00004010, 0x00404000,
    0x20404000, 0x20000000, 0x20004000, 0x00000010, 0x20400010, 0x00404000,
    0x20404010, 0x00400000, 0x00004010, 0x20000010, 0x00400000, 0x20004000,
    0x20000000, 0x00004010, 0x20000010, 0x20404010, 0x00404000, 0x20400000,
    0x00404010, 0x20404000, 0x00000000, 0x20400010, 0x00000010, 0x00004000,
    0x20400000, 0x00404010, 0x00004000, 0x00400010, 0x20004010, 0x00000000,
    0x20404000, 0x20000000, 0x00400010, 0x20004010};

__constant uint SB7[64] = {
    0x00200000, 0x04200002, 0x04000802, 0x00000000, 0x00000800, 0x04000802,
    0x00200802, 0x04200800, 0x04200802, 0x00200000, 0x00000000, 0x04000002,
    0x00000002, 0x04000000, 0x04200002, 0x00000802, 0x04000800, 0x00200802,
    0x00200002, 0x04000800, 0x04000002, 0x04200000, 0x04200800, 0x00200002,
    0x04200000, 0x00000800, 0x00000802, 0x04200802, 0x00200800, 0x00000002,
    0x04000000, 0x00200800, 0x04000000, 0x00200800, 0x00200000, 0x04000802,
    0x04000802, 0x04200002, 0x04200002, 0x00000002, 0x00200002, 0x04000000,
    0x04000800, 0x00200000, 0x04200800, 0x00000802, 0x00200802, 0x04200800,
    0x00000802, 0x04000002, 0x04200802, 0x04200000, 0x00200800, 0x00000000,
    0x00000002, 0x04200802, 0x00000000, 0x00200802, 0x04200000, 0x00000800,
    0x04000002, 0x04000800, 0x00000800, 0x00200002};

__constant uint SB8[64] = {
    0x10001040, 0x00001000, 0x00040000, 0x10041040, 0x10000000, 0x10001040,
    0x00000040, 0x10000000, 0x00040040, 0x10040000, 0x10041040, 0x00041000,
    0x10041000, 0x00041040, 0x00001000, 0x00000040, 0x10040000, 0x10000040,
    0x10001000, 0x00001040, 0x00041000, 0x00040040, 0x10040040, 0x10041000,
    0x00001040, 0x00000000, 0x00000000, 0x10040040, 0x10000040, 0x10001000,
    0x00041040, 0x00040000, 0x00041040, 0x00040000, 0x10041000, 0x00001000,
    0x00000040, 0x10040040, 0x00001000, 0x00041040, 0x10001000, 0x00000040,
    0x10000040, 0x10040000, 0x10040040, 0x10000000, 0x00040000, 0x10001040,
    0x00000000, 0x10041040, 0x00040040, 0x10000040, 0x10040000, 0x10001000,
    0x10001040, 0x00000000, 0x10041040, 0x00041000, 0x00041000, 0x00001040,
    0x00001040, 0x00040040, 0x10000000, 0x10041000};

typedef struct {
  uint esk[32]; /*!< DES encryption subkeys */
  uint dsk[32]; /*!< DES decryption subkeys */
} des_context;

typedef struct {
  uint esk[96]; /*!< Triple-DES encryption subkeys */
  uint dsk[96]; /*!< Triple-DES decryption subkeys */
} des3_context;


/*
 * 32-bit integer manipulation macros (big endian)
 */
#ifndef GET_UINT32_BE
#define GET_UINT32_BE(n, b, i)                                                 \
  {                                                                            \
    (n) = ((uint)(b)[(i)] << 24) | ((uint)(b)[(i) + 1] << 16) |        \
          ((uint)(b)[(i) + 2] << 8) | ((uint)(b)[(i) + 3]);            \
  }
#endif
#ifndef PUT_UINT32_BE
#define PUT_UINT32_BE(n, b, i)                                                 \
  {                                                                            \
    (b)[(i)] = (uchar)((n) >> 24);                                           \
    (b)[(i) + 1] = (uchar)((n) >> 16);                                       \
    (b)[(i) + 2] = (uchar)((n) >> 8);                                        \
    (b)[(i) + 3] = (uchar)((n));                                             \
  }
#endif

/*
 * Initial Permutation macro
 */
#define DES_IP(X, Y)                                                           \
  {                                                                            \
    T = ((X >> 4) ^ Y) & 0x0F0F0F0F;                                           \
    Y ^= T;                                                                    \
    X ^= (T << 4);                                                             \
    T = ((X >> 16) ^ Y) & 0x0000FFFF;                                          \
    Y ^= T;                                                                    \
    X ^= (T << 16);                                                            \
    T = ((Y >> 2) ^ X) & 0x33333333;                                           \
    X ^= T;                                                                    \
    Y ^= (T << 2);                                                             \
    T = ((Y >> 8) ^ X) & 0x00FF00FF;                                           \
    X ^= T;                                                                    \
    Y ^= (T << 8);                                                             \
    Y = ((Y << 1) | (Y >> 31)) & 0xFFFFFFFF;                                   \
    T = (X ^ Y) & 0xAAAAAAAA;                                                  \
    Y ^= T;                                                                    \
    X ^= T;                                                                    \
    X = ((X << 1) | (X >> 31)) & 0xFFFFFFFF;                                   \
  }

/*
 * Final Permutation macro
 */
#define DES_FP(X, Y)                                                           \
  {                                                                            \
    X = ((X << 31) | (X >> 1)) & 0xFFFFFFFF;                                   \
    T = (X ^ Y) & 0xAAAAAAAA;                                                  \
    X ^= T;                                                                    \
    Y ^= T;                                                                    \
    Y = ((Y << 31) | (Y >> 1)) & 0xFFFFFFFF;                                   \
    T = ((Y >> 8) ^ X) & 0x00FF00FF;                                           \
    X ^= T;                                                                    \
    Y ^= (T << 8);                                                             \
    T = ((Y >> 2) ^ X) & 0x33333333;                                           \
    X ^= T;                                                                    \
    Y ^= (T << 2);                                                             \
    T = ((X >> 16) ^ Y) & 0x0000FFFF;                                          \
    Y ^= T;                                                                    \
    X ^= (T << 16);                                                            \
    T = ((X >> 4) ^ Y) & 0x0F0F0F0F;                                           \
    Y ^= T;                                                                    \
    X ^= (T << 4);                                                             \
  }

/*
 * DES round macro
 */
#define DES_ROUND(X, Y)                                                        \
  _Pragma("cipher round") {                                                    \
    T = *SK++ ^ X;                                                             \
    Y ^= SB8[(T)&0x3F] ^ SB6[(T >> 8) & 0x3F] ^ SB4[(T >> 16) & 0x3F] ^        \
         SB2[(T >> 24) & 0x3F];                                                \
                                                                               \
    T = *SK++ ^ ((X << 28) | (X >> 4));                                        \
    Y ^= SB7[(T)&0x3F] ^ SB5[(T >> 8) & 0x3F] ^ SB3[(T >> 16) & 0x3F] ^        \
         SB1[(T >> 24) & 0x3F];                                                \
  }

__kernel void des_crypt(__local uint SK[32], __local uchar input[8], __local uchar output[8]){
  
  __local uint X, Y, T;

  GET_UINT32_BE(X, input, 0);
  GET_UINT32_BE(Y, input, 4);

  DES_IP(X, Y);

  DES_ROUND(Y, X);
  DES_ROUND(X, Y);
  DES_ROUND(Y, X);
  DES_ROUND(X, Y);
  DES_ROUND(Y, X);
  DES_ROUND(X, Y);
  DES_ROUND(Y, X);
  DES_ROUND(X, Y);
  DES_ROUND(Y, X);
  DES_ROUND(X, Y);
  DES_ROUND(Y, X);
  DES_ROUND(X, Y);
  DES_ROUND(Y, X);
  DES_ROUND(X, Y);
  DES_ROUND(Y, X);
  DES_ROUND(X, Y);

  DES_FP(Y, X);

  PUT_UINT32_BE(Y, output, 0);
  PUT_UINT32_BE(X, output, 4);
}

__kernel void des3_crypt(__local uint SK[96], __local uchar input[8], __local uchar output[8]) {
  
  __local uint X, Y, T;

  GET_UINT32_BE(X, input, 0);
  GET_UINT32_BE(Y, input, 4);

  DES_IP(X, Y);

  DES_ROUND(Y, X);
  DES_ROUND(X, Y);
  DES_ROUND(Y, X);
  DES_ROUND(X, Y);
  DES_ROUND(Y, X);
  DES_ROUND(X, Y);
  DES_ROUND(Y, X);
  DES_ROUND(X, Y);
  DES_ROUND(Y, X);
  DES_ROUND(X, Y);
  DES_ROUND(Y, X);
  DES_ROUND(X, Y);
  DES_ROUND(Y, X);
  DES_ROUND(X, Y);
  DES_ROUND(Y, X);
  DES_ROUND(X, Y);

  DES_ROUND(X, Y);
  DES_ROUND(Y, X);
  DES_ROUND(X, Y);
  DES_ROUND(Y, X);
  DES_ROUND(X, Y);
  DES_ROUND(Y, X);
  DES_ROUND(X, Y);
  DES_ROUND(Y, X);
  DES_ROUND(X, Y);
  DES_ROUND(Y, X);
  DES_ROUND(X, Y);
  DES_ROUND(Y, X);
  DES_ROUND(X, Y);
  DES_ROUND(Y, X);
  DES_ROUND(X, Y);
  DES_ROUND(Y, X);

  DES_ROUND(Y, X);
  DES_ROUND(X, Y);
  DES_ROUND(Y, X);
  DES_ROUND(X, Y);
  DES_ROUND(Y, X);
  DES_ROUND(X, Y);
  DES_ROUND(Y, X);
  DES_ROUND(X, Y);
  DES_ROUND(Y, X);
  DES_ROUND(X, Y);
  DES_ROUND(Y, X);
  DES_ROUND(X, Y);
  DES_ROUND(Y, X);
  DES_ROUND(X, Y);
  DES_ROUND(Y, X);
  DES_ROUND(X, Y);

  DES_FP(Y, X);

  PUT_UINT32_BE(Y, output, 0);
  PUT_UINT32_BE(X, output, 4);
}

__kernel void desCipher(__global uint SK[32], __global uchar* input, __global uchar* output){
  
  __local int gid;
  gid = get_global_id(0);

  __local uchar _input[8];
  #pragma unroll
  for (int i = 0; i < 8; ++i) {
    int offset = gid * 8 + i;
    _input[i] = input[offset];
  }

  __local uint _SK[32];
  #pragma unroll
  for(int i = 0; i < 32; i++){
    _SK[i] = SK[i];
  }

  __local uchar outCipher[8];
  /* encryption */
  des_crypt(_SK, _input, outCipher);

  #pragma unroll
  for(int i = 0; i < 8; i++) {
    int offset = gid * 8 + i;
    output[offset] = outCipher[i];
  } 
}

__kernel void des3Cipher(__global uint SK[96], __global uchar* input, __global uchar* output){
  
  __local int gid;
  gid = get_global_id(0);

  __local uchar _input[8];
  #pragma unroll
  for (int i = 0; i < 8; ++i) {
    int offset = gid * 8 + i;
    _input[i] = input[offset];
  }

  __local uint _SK[96];
  #pragma unroll
  for(int i = 0; i < 96; i++){
    _SK[i] = SK[i];
  }

  __local uchar outCipher[8];
  /* encryption */
  des3_crypt(_SK, _input, outCipher);

  #pragma unroll
  for(int i = 0; i < 8; i++) {
    int offset = gid * 8 + i;
    output[offset] = outCipher[i];
  } 
}

__kernel void desCtrCipher(__global uint SK[32], __global uchar* input, __global uchar* output){
  
  __local int gid;
  gid = get_global_id(0);

  /* initialize counter */
  __local uchar counter[8]; 
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

  __local uint _SK[32];
  #pragma unroll
  for(int i = 0; i < 32; i++){
    _SK[i] = SK[i];
  }

  __local uchar outCipher[8];
  /* encryption */
  des_crypt(_SK, counter, outCipher);

  #pragma unroll
  for(int i = 0; i < 8; i++) {
    int offset = gid * 8 + i;
    output[offset] = outCipher[i] ^ input[offset];
  } 
}

__kernel void des3CtrCipher(__global uint SK[32], __global uchar* input, __global uchar* output){
  
  __local int gid;
  gid = get_global_id(0);

  /* initialize counter */
  __local uchar counter[8]; 
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


  __local uint _SK[32];
  #pragma unroll
  for(int i = 0; i < 32; i++){
    _SK[i] = SK[i];
  }

  __local uchar outCipher[8];
  /* encryption */
  des_crypt(_SK, counter, outCipher);

  #pragma unroll
  for(int i = 0; i < 8; i++) {
    int offset = gid * 8 + i;
    output[offset] = outCipher[i] ^ input[offset];
  } 
}


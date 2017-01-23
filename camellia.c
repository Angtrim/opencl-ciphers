#include "benchmark.h"

#ifdef __APPLE__
#include <OpenCL/opencl.h>
#else
#include <CL/cl.h>
#endif

#define MAX_SOURCE_SIZE (0x100000)

//#define DUMP_ROUNDS
#define CAMELLIA128

#if defined(CAMELLIA128)
#  define KEYLEN 128
#  define NAME "Camellia 128"
#elif defined(CAMELLIA192)
#  define KEYLEN 192
#  define NAME "Camellia 192"
#elif defined(CAMELLIA256)
#  define KEYLEN 256
#  define NAME "Camellia 256"
#else
#  error "Define CAMELLIA128, CAMELLIA192 or CAMELLIA256"
#endif

#if 0
1.1.  Camellia

   Camellia was jointly developed by Nippon Telegraph and Telephone
   Corporation and Mitsubishi Electric Corporation in 2000
   [CamelliaSpec].  Camellia specifies the 128-bit block size and 128-,
   192-, and 256-bit key sizes, the same interface as the Advanced
   Encryption Standard (AES).  Camellia is characterized by its
   suitability for both software and hardware implementations as well as
   its high level of security.  From a practical viewpoint, it is
   designed to enable flexibility in software and hardware
   implementations on 32-bit processors widely used over the Internet
   and many applications, 8-bit processors used in smart cards,
   cryptographic hardware, embedded systems, and so on [CamelliaTech].
   Moreover, its key setup time is excellent, and its key agility is
   superior to that of AES.
#endif

#include <stdint.h>

#define MASK8   0xff
#define MASK32  0xffffffffu

// ===---- 2.4.  Components of Camellia ----===

// SBOX1, SBOX2, SBOX3, and SBOX4 are lookup tables with 8-bit input/
// output data.  SBOX2, SBOX3, and SBOX4 are defined using SBOX1 as
// follows:
//
//     SBOX2[x] = SBOX1[x] <<< 1;
//     SBOX3[x] = SBOX1[x] <<< 7;
//     SBOX4[x] = SBOX1[x <<< 1];

const uint8_t SBOX1[0x100] __attribute__((sbox)) = {
112, 130,  44, 236, 179,  39, 192, 229, 228, 133,  87,  53, 234,  12, 174,  65,
 35, 239, 107, 147,  69,  25, 165,  33, 237,  14,  79,  78,  29, 101, 146, 189,
134, 184, 175, 143, 124, 235,  31, 206,  62,  48, 220,  95,  94, 197,  11,  26,
166, 225,  57, 202, 213,  71,  93,  61, 217,   1,  90, 214,  81,  86, 108,  77,
139,  13, 154, 102, 251, 204, 176,  45, 116,  18,  43,  32, 240, 177, 132, 153,
223,  76, 203, 194,  52, 126, 118,   5, 109, 183, 169,  49, 209,  23,   4, 215,
 20,  88,  58,  97, 222,  27,  17,  28,  50,  15, 156,  22,  83,  24, 242,  34,
254,  68, 207, 178, 195, 181, 122, 145,  36,   8, 232, 168,  96, 252, 105,  80,
170, 208, 160, 125, 161, 137,  98, 151,  84,  91,  30, 149, 224, 255, 100, 210,
 16, 196,   0,  72, 163, 247, 117, 219, 138,   3, 230, 218,   9,  63, 221, 148,
135,  92, 131,   2, 205,  74, 144,  51, 115, 103, 246, 243, 157, 127, 191, 226,
 82, 155, 216,  38, 200,  55, 198,  59, 129, 150, 111,  75,  19, 190,  99,  46,
233, 121, 167, 140, 159, 110, 188, 142,  41, 245, 249, 182,  47, 253, 180,  89,
120, 152,   6, 106, 231,  70, 113, 186, 212,  37, 171,  66, 136, 162, 141, 250,
114,   7, 185,  85, 248, 238, 172,  10,  54,  73,  42, 104,  60,  56, 241, 164,
 64,  40, 211, 123, 187, 201,  67, 193,  21, 227, 173, 244, 119, 199, 128, 158
};

const uint8_t SBOX2[0x100] __attribute__((sbox)) = {
224,   5,  88, 217, 103,  78, 129, 203, 201,  11, 174, 106, 213,  24,  93, 130,
 70, 223, 214,  39, 138,  50,  75,  66, 219,  28, 158, 156,  58, 202,  37, 123,
 13, 113,  95,  31, 248, 215,  62, 157, 124,  96, 185, 190, 188, 139,  22,  52,
 77, 195, 114, 149, 171, 142, 186, 122, 179,   2, 180, 173, 162, 172, 216, 154,
 23,  26,  53, 204, 247, 153,  97,  90, 232,  36,  86,  64, 225,  99,   9,  51,
191, 152, 151, 133, 104, 252, 236,  10, 218, 111,  83,  98, 163,  46,   8, 175,
 40, 176, 116, 194, 189,  54,  34,  56, 100,  30,  57,  44, 166,  48, 229,  68,
253, 136, 159, 101, 135, 107, 244,  35,  72,  16, 209,  81, 192, 249, 210, 160,
 85, 161,  65, 250,  67,  19, 196,  47, 168, 182,  60,  43, 193, 255, 200, 165,
 32, 137,   0, 144,  71, 239, 234, 183,  21,   6, 205, 181,  18, 126, 187,  41,
 15, 184,   7,   4, 155, 148,  33, 102, 230, 206, 237, 231,  59, 254, 127, 197,
164,  55, 177,  76, 145, 110, 141, 118,   3,  45, 222, 150,  38, 125, 198,  92,
211, 242,  79,  25,  63, 220, 121,  29,  82, 235, 243, 109,  94, 251, 105, 178,
240,  49,  12, 212, 207, 140, 226, 117, 169,  74,  87, 132,  17,  69,  27, 245,
228,  14, 115, 170, 241, 221,  89,  20, 108, 146,  84, 208, 120, 112, 227,  73,
128,  80, 167, 246, 119, 147, 134, 131,  42, 199,  91, 233, 238, 143,   1,  61
};

const uint8_t SBOX3[0x100] __attribute__((sbox)) = {
 56,  65,  22, 118, 217, 147,  96, 242, 114, 194, 171, 154, 117,   6,  87, 160,
145, 247, 181, 201, 162, 140, 210, 144, 246,   7, 167,  39, 142, 178,  73, 222,
 67,  92, 215, 199,  62, 245, 143, 103,  31,  24, 110, 175,  47, 226, 133,  13,
 83, 240, 156, 101, 234, 163, 174, 158, 236, 128,  45, 107, 168,  43,  54, 166,
197, 134,  77,  51, 253, 102,  88, 150,  58,   9, 149,  16, 120, 216,  66, 204,
239,  38, 229,  97,  26,  63,  59, 130, 182, 219, 212, 152, 232, 139,   2, 235,
 10,  44,  29, 176, 111, 141, 136,  14,  25, 135,  78,  11, 169,  12, 121,  17,
127,  34, 231,  89, 225, 218,  61, 200,  18,   4, 116,  84,  48, 126, 180,  40,
 85, 104,  80, 190, 208, 196,  49, 203,  42, 173,  15, 202, 112, 255,  50, 105,
  8,  98,   0,  36, 209, 251, 186, 237,  69, 129, 115, 109, 132, 159, 238,  74,
195,  46, 193,   1, 230,  37,  72, 153, 185, 179, 123, 249, 206, 191, 223, 113,
 41, 205, 108,  19, 100, 155,  99, 157, 192,  75, 183, 165, 137,  95, 177,  23,
244, 188, 211,  70, 207,  55,  94,  71, 148, 250, 252,  91, 151, 254,  90, 172,
 60,  76,   3,  53, 243,  35, 184,  93, 106, 146, 213,  33,  68,  81, 198, 125,
 57, 131, 220, 170, 124, 119,  86,   5,  27, 164,  21,  52,  30,  28, 248,  82,
 32,  20, 233, 189, 221, 228, 161, 224, 138, 241, 214, 122, 187, 227,  64,  79
};

const uint8_t SBOX4[0x100] __attribute__((sbox)) = {
112,  44, 179, 192, 228,  87, 234, 174,  35, 107,  69, 165, 237,  79,  29, 146,
134, 175, 124,  31,  62, 220,  94,  11, 166,  57, 213,  93, 217,  90,  81, 108,
139, 154, 251, 176, 116,  43, 240, 132, 223, 203,  52, 118, 109, 169, 209,   4,
 20,  58, 222,  17,  50, 156,  83, 242, 254, 207, 195, 122,  36, 232,  96, 105,
170, 160, 161,  98,  84,  30, 224, 100,  16,   0, 163, 117, 138, 230,   9, 221,
135, 131, 205, 144, 115, 246, 157, 191,  82, 216, 200, 198, 129, 111,  19,  99,
233, 167, 159, 188,  41, 249,  47, 180, 120,   6, 231, 113, 212, 171, 136, 141,
114, 185, 248, 172,  54,  42,  60, 241,  64, 211, 187,  67,  21, 173, 119, 128,
130, 236,  39, 229, 133,  53,  12,  65, 239, 147,  25,  33,  14,  78, 101, 189,
184, 143, 235, 206,  48,  95, 197,  26, 225, 202,  71,  61,   1, 214,  86,  77,
 13, 102, 204,  45,  18,  32, 177, 153,  76, 194, 126,   5, 183,  49,  23, 215,
 88,  97,  27,  28,  15,  22,  24,  34,  68, 178, 181, 145,   8, 168, 252,  80,
208, 125, 137, 151,  91, 149, 255, 210, 196,  72, 247, 219,   3, 218,  63, 148,
 92,   2,  74,  51, 103, 243, 127, 226, 155,  38,  55,  59, 150,  75, 190,  46,
121, 140, 110, 142, 245, 182, 253,  89, 152, 106,  70, 186,  37,  66, 162, 250,
  7,  85, 238,  10,  73, 104,  56, 164,  40, 123, 201, 193, 227, 244, 199, 158
};

char* stradd(const char* a, const char* b){
    size_t len = strlen(a) + strlen(b);
    char *ret = (char*)malloc(len * sizeof(char) + 1);
    *ret = '\0';
    return strcat(strcat(ret, a) ,b);
} //This function adds two string pointers together

// ===---- 2.4.1.  F-function ----===

// F-function takes two parameters.  One is 64-bit input data F_IN.  The
// other is 64-bit subkey KE.  F-function returns 64-bit data F_OUT.
//
inline __attribute__((always_inline))
uint64_t F(uint64_t F_IN, uint64_t KE) {
  uint64_t x;

  uint8_t t1, t2, t3, t4, t5, t6, t7, t8;
  uint8_t y1, y2, y3, y4, y5, y6, y7, y8;

  x  = F_IN ^ KE;

  t1 =  x >> 56;
  t2 = (x >> 48) & MASK8;
  t3 = (x >> 40) & MASK8;
  t4 = (x >> 32) & MASK8;
  t5 = (x >> 24) & MASK8;
  t6 = (x >> 16) & MASK8;
  t7 = (x >>  8) & MASK8;
  t8 =  x        & MASK8;

  t1 = SBOX1[t1];
  t2 = SBOX2[t2];
  t3 = SBOX3[t3];
  t4 = SBOX4[t4];
  t5 = SBOX2[t5];
  t6 = SBOX3[t6];
  t7 = SBOX4[t7];
  t8 = SBOX1[t8];

  y1 = t1 ^ t3 ^ t4 ^ t6 ^ t7 ^ t8;
  y2 = t1 ^ t2 ^ t4 ^ t5 ^ t7 ^ t8;
  y3 = t1 ^ t2 ^ t3 ^ t5 ^ t6 ^ t8;
  y4 = t2 ^ t3 ^ t4 ^ t5 ^ t6 ^ t7;
  y5 = t1 ^ t2 ^ t6 ^ t7 ^ t8;
  y6 = t2 ^ t3 ^ t5 ^ t7 ^ t8;
  y7 = t3 ^ t4 ^ t5 ^ t6 ^ t8;
  y8 = t1 ^ t4 ^ t5 ^ t6 ^ t7;

  uint64_t F_OUT = ((uint64_t)y1 << 56)
                 | ((uint64_t)y2 << 48)
                 | ((uint64_t)y3 << 40)
                 | ((uint64_t)y4 << 32)
                 | ((uint64_t)y5 << 24)
                 | ((uint64_t)y6 << 16)
                 | ((uint64_t)y7 <<  8)
                 | y8;
  return F_OUT;
}

// ===---- 2.4.2.  FL- and FLINV-functions ----===

#define rol32(x, n) (((x) << (n)) | ((x) >> (32 - n)))

// FL-function takes two parameters.  One is 64-bit input data FL_IN.
// The other is 64-bit subkey KE.  FL-function returns 64-bit data FL_OUT.
//
inline __attribute__((always_inline))
uint64_t FL(uint64_t FL_IN, uint64_t KE) {
  uint32_t x1, x2;
  uint32_t k1, k2;

  x1 = FL_IN >> 32;
  x2 = FL_IN & MASK32;
  k1 = KE >> 32;
  k2 = KE & MASK32;
  x2 = x2 ^ rol32((x1 & k1), 1);
  x1 = x1 ^ (x2 | k2);

  uint64_t FL_OUT = ((uint64_t)x1 << 32) | x2;

  return FL_OUT;
}

// FLINV-function is the inverse function of the FL-function.
//
inline __attribute__((always_inline))
uint64_t FLINV(uint64_t FLINV_IN, uint64_t KE) {
  uint32_t y1, y2;
  uint32_t k1, k2;

  y1 = FLINV_IN >> 32;
  y2 = FLINV_IN & MASK32;
  k1 = KE >> 32;
  k2 = KE & MASK32;
  y1 = y1 ^ (y2 | k2);
  y2 = y2 ^ rol32((y1 & k1), 1);

  uint64_t FLINV_OUT = ((uint64_t)y1 << 32) | y2;

  return FLINV_OUT;
}

#undef rol32

 #if 0
2.1.  Terminology

   The following operators are used in this document to describe the
   algorithm.

      &    bitwise AND operation.
      |    bitwise OR operation.
      ^    bitwise exclusive-OR operation.
      <<   logical left shift operation.
      >>   logical right shift operation.
      <<<  left rotation operation.
      ~y   bitwise complement of y.
      0x   hexadecimal representation.

   Note that the logical left shift operation is done with the infinite
   data width.
#endif

// ===---- 2.2.  Key Scheduling Part ----===

inline __attribute__((always_inline))
uint64_t rol128lo(uint64_t xhi, uint64_t xlo, unsigned n) {
  if (n > 64) {
    return n? (xhi << (n - 64)) | (xlo >> (128 - n)) : xhi;
  } else {
    return n? (xlo << n) | (xhi >> (64 - n)) : xlo;
  }
}

#define rol128hi(xhi, xlo, n) rol128lo(xlo, xhi, n)

// generate 64-bit subkeys kw1, ..., kw4, k1, ..., k18, ke1, ..., ke4
//
inline __attribute__((always_inline))
void camellia_expandkey(uint64_t *Ko, uint64_t *kw, uint64_t *k, uint64_t *ke) {
  uint64_t K[4];

  K[0] = Ko[0]; K[1] = Ko[1];

#if KEYLEN == 128
  K[2] = 0; K[3] = 0;
#elif KEYLEN == 192
  K[2] = Ko[2]; K[3] = ~Ko[2];
#elif KEYLEN == 256
  K[2] = Ko[2]; K[3] = Ko[3];
#endif

  uint64_t KLhi = K[0];
  uint64_t KLlo = K[1];
  uint64_t KRhi = K[2];
  uint64_t KRlo = K[3];

  const uint64_t Sigma1 = 0xA09E667F3BCC908Bul;
  const uint64_t Sigma2 = 0xB67AE8584CAA73B2ul;
  const uint64_t Sigma3 = 0xC6EF372FE94F82BEul;
  const uint64_t Sigma4 = 0x54FF53A5F1D36F1Cul;

#if KEYLEN > 128
  const uint64_t Sigma5 = 0x10E527FADE682D1Dul;
  const uint64_t Sigma6 = 0xB05688C2B3E6C1FDul;
#endif

  uint64_t D1, D2;

  D1 = KLhi ^ KRhi;
  D2 = KLlo ^ KRlo;
  D2 = D2 ^ F(D1, Sigma1);
  D1 = D1 ^ F(D2, Sigma2);
  D1 = D1 ^ KLhi;
  D2 = D2 ^ KLlo;
  D2 = D2 ^ F(D1, Sigma3);
  D1 = D1 ^ F(D2, Sigma4);

  uint64_t KAhi = D1;
  uint64_t KAlo = D2;

#if KEYLEN > 128
  D1 = KAhi ^ KRhi;
  D2 = KAlo ^ KRlo;
  D2 = D2 ^ F(D1, Sigma5);
  D1 = D1 ^ F(D2, Sigma6);

  uint64_t KBhi = D1;
  uint64_t KBlo = D2;
#endif

  // 64-bit subkeys are generated by rotating KL, KR, KA, and KB and
  // taking the left- or right-half of them.

#if KEYLEN == 128
  kw[0] = rol128hi(KLhi, KLlo,   0);
  kw[1] = rol128lo(KLhi, KLlo,   0);

  k[0]  = rol128hi(KAhi, KAlo,   0);
  k[1]  = rol128lo(KAhi, KAlo,   0);
  k[2]  = rol128hi(KLhi, KLlo,  15);
  k[3]  = rol128lo(KLhi, KLlo,  15);
  k[4]  = rol128hi(KAhi, KAlo,  15);
  k[5]  = rol128lo(KAhi, KAlo,  15);

  ke[0] = rol128hi(KAhi, KAlo,  30);
  ke[1] = rol128lo(KAhi, KAlo,  30);

  k[6]  = rol128hi(KLhi, KLlo,  45);
  k[7]  = rol128lo(KLhi, KLlo,  45);
  k[8]  = rol128hi(KAhi, KAlo,  45);
  k[9]  = rol128lo(KLhi, KLlo,  60);
  k[10] = rol128hi(KAhi, KAlo,  60);
  k[11] = rol128lo(KAhi, KAlo,  60);

  ke[2] = rol128hi(KLhi, KLlo,  77);
  ke[3] = rol128lo(KLhi, KLlo,  77);

  k[12] = rol128hi(KLhi, KLlo,  94);
  k[13] = rol128lo(KLhi, KLlo,  94);
  k[14] = rol128hi(KAhi, KAlo,  94);
  k[15] = rol128lo(KAhi, KAlo,  94);
  k[16] = rol128hi(KLhi, KLlo, 111);
  k[17] = rol128lo(KLhi, KLlo, 111);

  kw[2] = rol128hi(KAhi, KAlo, 111);
  kw[3] = rol128lo(KAhi, KAlo, 111);

#else // 192 or 256

  kw[0] = rol128hi(KLhi, KLlo,   0);
  kw[1] = rol128lo(KLhi, KLlo,   0);

  k[0]  = rol128hi(KBhi, KBlo,   0);
  k[1]  = rol128lo(KBhi, KBlo,   0);
  k[2]  = rol128hi(KRhi, KRlo,  15);
  k[3]  = rol128lo(KRhi, KRlo,  15);
  k[4]  = rol128hi(KAhi, KAlo,  15);
  k[5]  = rol128lo(KAhi, KAlo,  15);

  ke[0] = rol128hi(KRhi, KRlo,  30);
  ke[1] = rol128lo(KRhi, KRlo,  30);

  k[6]  = rol128hi(KBhi, KBlo,  30);
  k[7]  = rol128lo(KBhi, KBlo,  30);
  k[8]  = rol128hi(KLhi, KLlo,  45);
  k[9]  = rol128lo(KLhi, KLlo, 45);
  k[10] = rol128hi(KAhi, KAlo,  45);
  k[11] = rol128lo(KAhi, KAlo,  45);

  ke[2] = rol128hi(KLhi, KLlo,  60);
  ke[3] = rol128lo(KLhi, KLlo,  60);

  k[12] = rol128hi(KRhi, KRlo,  60);
  k[13] = rol128lo(KRhi, KRlo,  60);
  k[14] = rol128hi(KBhi, KBlo,  60);
  k[15] = rol128lo(KBhi, KBlo,  60);
  k[16] = rol128hi(KLhi, KLlo,  77);
  k[17] = rol128lo(KLhi, KLlo,  77);

  ke[4] = rol128hi(KAhi, KAlo,  77);
  ke[5] = rol128lo(KAhi, KAlo,  77);

  k[18] = rol128hi(KRhi, KRlo,  94);
  k[19] = rol128lo(KRhi, KRlo,  94);
  k[20] = rol128hi(KAhi, KAlo,  94);
  k[21] = rol128lo(KAhi, KAlo,  94);
  k[22] = rol128hi(KLhi, KLlo, 111);
  k[23] = rol128lo(KLhi, KLlo, 111);

  kw[2] = rol128hi(KBhi, KBlo, 111);
  kw[3] = rol128lo(KBhi, KBlo, 111);
#endif
}

#undef rol128

// ===---- 2.3.1.  Encryption for 128-bit keys ----===

#define round(name) _Pragma("cipher round")

#if KEYLEN == 128

#ifdef DUMP_ROUNDS
#  include <stdio.h>
#  define dump(what, D1, D2, k) \
     printf(#what "\tX = %016lx K=%016lx Y=%016lx\n", D1, k, D2);
#else
#  define dump(...)
#endif

inline __attribute__((always_inline))
void camellia_encrypt(uint64_t *C, uint64_t *M, uint64_t *kw, uint64_t *k, uint64_t *ke) {
  uint64_t D1 = M[0];
  uint64_t D2 = M[1];

  round(pre whitening) {
    D1 = D1 ^ kw[0];
    D2 = D2 ^ kw[1];
  }

  round( 1) { D2 = D2 ^ F(D1, k[0]); }
  round( 2) { D1 = D1 ^ F(D2, k[1]); }
  round( 3) { D2 = D2 ^ F(D1, k[2]); }
  round( 4) { D1 = D1 ^ F(D2, k[3]); }
  round( 5) { D2 = D2 ^ F(D1, k[4]); }
  round( 6) { D1 = D1 ^ F(D2, k[5]); }

  round(FL) { D1 = FL(D1, ke[0]); }
  round(FLINV) { D2 = FLINV(D2, ke[1]); }

  round( 7) { D2 = D2 ^ F(D1, k[6]); }
  round( 8) { D1 = D1 ^ F(D2, k[7]); }
  round( 9) { D2 = D2 ^ F(D1, k[8]); }
  round(10) { D1 = D1 ^ F(D2, k[9]); }
  round(11) { D2 = D2 ^ F(D1, k[10]); }
  round(12) { D1 = D1 ^ F(D2, k[11]); }

  round(FL) { D1 = FL(D1, ke[2]); }
  round(FLINV) { D2 = FLINV(D2, ke[3]); }

  round(13) { D2 = D2 ^ F(D1, k[12]); }
  round(14) { D1 = D1 ^ F(D2, k[13]); }
  round(15) { D2 = D2 ^ F(D1, k[14]); }
  round(16) { D1 = D1 ^ F(D2, k[15]); }
  round(17) { D2 = D2 ^ F(D1, k[16]); }
  round(19) { D1 = D1 ^ F(D2, k[17]); }

  round(post whitening) {
    D2 = D2 ^ kw[2];           // Postwhitening
    D1 = D1 ^ kw[3];
  }

  C[0] = D2;
  C[1] = D1;
}

#else

// ===---- 2.3.2.  Encryption for 192- and 256-bit keys ----===

inline __attribute__((always_inline))
void camellia_encrypt(uint64_t *C, uint64_t *M, uint64_t *kw, uint64_t *k, uint64_t *ke) {
  uint64_t D1 = M[0];
  uint64_t D2 = M[1];

  round(pre whitening) {
    D1 = D1 ^ kw[0];
    D2 = D2 ^ kw[1];
  }

  round( 1) { D2 = D2 ^ F(D1, k[0]); }
  round( 2) { D1 = D1 ^ F(D2, k[1]); }
  round( 3) { D2 = D2 ^ F(D1, k[2]); }
  round( 4) { D1 = D1 ^ F(D2, k[3]); }
  round( 5) { D2 = D2 ^ F(D1, k[4]); }
  round( 6) { D1 = D1 ^ F(D2, k[5]); }

  round(FL) { D1 = FL(D1, ke[0]); }
  round(FLINV) { D2 = FLINV(D2, ke[1]); }

  round( 7) { D2 = D2 ^ F(D1, k[6]); }
  round( 8) { D1 = D1 ^ F(D2, k[7]); }
  round( 9) { D2 = D2 ^ F(D1, k[8]); }
  round(10) { D1 = D1 ^ F(D2, k[9]); }
  round(11) { D2 = D2 ^ F(D1, k[10]); }
  round(12) { D1 = D1 ^ F(D2, k[11]); }

  round(FL) { D1 = FL(D1, ke[2]); }
  round(FLINV) { D2 = FLINV(D2, ke[3]); }

  round(13) { D2 = D2 ^ F(D1, k[12]); }
  round(14) { D1 = D1 ^ F(D2, k[13]); }
  round(15) { D2 = D2 ^ F(D1, k[14]); }
  round(16) { D1 = D1 ^ F(D2, k[15]); }
  round(17) { D2 = D2 ^ F(D1, k[16]); }
  round(18) { D1 = D1 ^ F(D2, k[17]); }

  round(FL) { D1 = FL(D1, ke[4]); }
  round(FLINV) { D2 = FLINV(D2, ke[5]); }

  round(19) { D2 = D2 ^ F(D1, k[18]); }
  round(20) { D1 = D1 ^ F(D2, k[19]); }
  round(21) { D2 = D2 ^ F(D1, k[20]); }
  round(22) { D1 = D1 ^ F(D2, k[21]); }
  round(23) { D2 = D2 ^ F(D1, k[22]); }
  round(24) { D1 = D1 ^ F(D2, k[23]); }

  round(post whitening) {
    D2 = D2 ^ kw[2];
    D1 = D1 ^ kw[3];
  }

  C[0] = D2;
  C[1] = D1;
}

#endif

void camellia_expand(uint64_t *K __attribute__((key)),uint64_t *M __attribute__((plain)),        uint64_t *C, uint64_t *kw, uint64_t *k, uint64_t *ke){

  //BEGIN_KEYSCHED;
  camellia_expandkey(K, kw, k, ke);
  //END_KEYSCHED;
}

#include <stdio.h>

int main() {
cl_device_id device_id = NULL;
cl_context context = NULL;
cl_command_queue command_queue = NULL;

cl_mem _C = NULL;
cl_mem _M = NULL;
cl_mem _kw = NULL;
cl_mem _k = NULL;
cl_mem _ke = NULL;


cl_program program = NULL;
cl_kernel kernel = NULL;
cl_platform_id platform_id = NULL;
cl_uint ret_num_devices;
cl_uint ret_num_platforms;
cl_int ret;


  // ===---- Appendix A.  Example Data of Camellia ----===
#if KEYLEN == 128
  uint64_t Plaintext[2] = {0x0123456789abcdef, 0xfedcba9876543210};
  uint64_t Ciphertext[2] = {0x6767313854966973, 0x0857065648eabe43};
  uint64_t Key[2] = {0x0123456789abcdef, 0xfedcba9876543210};
#elif KEYLEN == 192
  uint64_t Plaintext[2] = {0x0123456789abcdef, 0xfedcba9876543210};
  uint64_t Ciphertext[2] = {0xb4993401b3e996f8, 0x4ee5cee7d79b09b9};
  uint64_t Key[3] = {0x0123456789abcdef, 0xfedcba9876543210,
                     0x0011223344556677};
#elif KEYLEN == 256
  uint64_t Plaintext[2] = {0x0123456789abcdef, 0xfedcba9876543210};
  uint64_t Ciphertext[2] = {0x9acc237dff16d76c, 0x20ef7c919e3a7509};
  uint64_t Key[4] = {0x0123456789abcdef, 0xfedcba9876543210,
                     0x0011223344556677, 0x8899aabbccddeeff};
#else
#  error "Invalid KEYLEN"
#endif

int k_dim;
int ke_dim;

#if KEYLEN > 128
  k_dim = 24;
  ke_dim = 6;
  uint64_t k[k_dim];
  uint64_t ke[ke_dim];
#else
  k_dim = 18;
  ke_dim = 4;
  uint64_t k[k_dim];
  uint64_t ke[ke_dim];
#endif
  int kw_dim = 4;
  uint64_t kw[kw_dim];

  uint64_t Mine[2];

//DO_BENCHMARK(NAME) {
camellia_expand(Key, Plaintext, Mine, kw, k, ke);

//encryption
//BEGIN_ENCRYPT(M);
//camellia_encrypt(C, M, kw, k, ke);
//END_ENCRYPT(C);

FILE *fp;
char fileName[] = "./camellia.cl";

//to pass the constant parameter Nb
char *append_str = "#define KEYLEN 128\n";
//append_str = stradd(append_str, "4");
//append_str = stradd(append_str, "\n");

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
ret = clGetDeviceIDs(platform_id, CL_DEVICE_TYPE_DEFAULT, 1, &device_id, &ret_num_devices);
 
/* Create OpenCL context */
context = clCreateContext(NULL, 1, &device_id, NULL, NULL, &ret);
 
/* Create Command Queue */
command_queue = clCreateCommandQueue(context, device_id, 0, &ret);
 
/* Create Memory Buffers */
//out = clCreateBuffer(context, CL_MEM_READ_WRITE,BLOCK_SIZE * sizeof(byte), NULL, &ret);
_C = clCreateBuffer(context, CL_MEM_READ_WRITE, 2 * sizeof(uint64_t), NULL, &ret);
_M = clCreateBuffer(context, CL_MEM_READ_WRITE, 2 * sizeof(uint64_t), NULL, &ret);
_kw = clCreateBuffer(context, CL_MEM_READ_WRITE, kw_dim * sizeof(uint64_t), NULL, &ret); 
_k = clCreateBuffer(context, CL_MEM_READ_WRITE, k_dim * sizeof(uint64_t), NULL, &ret);
_ke = clCreateBuffer(context, CL_MEM_READ_WRITE, ke_dim * sizeof(uint64_t), NULL, &ret);

/* Copy input data to Memory Buffer */
ret = clEnqueueWriteBuffer(command_queue, _M, CL_TRUE, 0, 2 * sizeof(uint64_t), Plaintext, 0, NULL, NULL);
ret = clEnqueueWriteBuffer(command_queue, _kw, CL_TRUE, 0, kw_dim * sizeof(uint64_t), kw, 0, NULL, NULL);
ret = clEnqueueWriteBuffer(command_queue, _k, CL_TRUE, 0, k_dim * sizeof(uint64_t), k, 0, NULL, NULL);
ret = clEnqueueWriteBuffer(command_queue, _ke, CL_TRUE, 0, ke_dim * sizeof(uint64_t), ke, 0, NULL, NULL);

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
kernel = clCreateKernel(program, "camellia_encrypt", &ret);
 
/* Set OpenCL Kernel Parameters */
ret = clSetKernelArg(kernel, 0, sizeof(cl_mem), (void *)&_C);
ret = clSetKernelArg(kernel, 1, sizeof(cl_mem), (void *)&_M);
ret = clSetKernelArg(kernel, 2, sizeof(cl_mem), (void *)&_kw);
ret = clSetKernelArg(kernel, 3, sizeof(cl_mem), (void *)&_k);
ret = clSetKernelArg(kernel, 4, sizeof(cl_mem), (void *)&_ke);
 
/* Execute OpenCL Kernel */
ret = clEnqueueTask(command_queue, kernel, 0, NULL,NULL);
 
/* Copy results from the memory buffer */
ret = clEnqueueReadBuffer(command_queue, _C, CL_TRUE, 0,
2 * sizeof(uint64_t),Mine, 0, NULL, NULL);
 
/* Finalization */
ret = clFlush(command_queue);
ret = clFinish(command_queue);
ret = clReleaseKernel(kernel);
ret = clReleaseProgram(program);
ret = clReleaseMemObject(_C);
ret = clReleaseMemObject(_M);
ret = clReleaseMemObject(_k);
ret = clReleaseMemObject(_kw);
ret = clReleaseMemObject(_ke);
ret = clReleaseCommandQueue(command_queue);
ret = clReleaseContext(context);
 
free(source_str);

if (Mine[0] == Ciphertext[0] && Mine[1] == Ciphertext[1]) {
      PASS;
} else {
      FAIL;
}
//}

return 0;
}

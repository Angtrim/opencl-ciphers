
#define rol8(x, n) (((x) << (n)) | ((x) >> (8 - n)))

#define concat64(a7, a6, a5, a4, a3, a2, a1, a0) \
  ( ((ulong)(a7) << (8*7)) \
  | ((ulong)(a6) << (8*6)) \
  | ((ulong)(a5) << (8*5)) \
  | ((ulong)(a4) << (8*4)) \
  | ((ulong)(a3) << (8*3)) \
  | ((ulong)(a2) << (8*2)) \
  | ((ulong)(a1) << (8*1)) \
  | ((ulong)(a0) << (8*0)) \
  )

#define byte(a, n) ((a >> (8*n)) & 0xff)

void F0(__private uchar x, __private uchar F0_OUT) {
  F0_OUT = rol8(x, 1) ^ rol8(x, 2) ^ rol8(x, 7);
}

void F1(__private uchar x, __private uchar F1_OUT) {
  F1_OUT = rol8(x, 3) ^ rol8(x, 4) ^ rol8(x, 6);
}

void hight_encrypt(__private ulong P, __global uchar *WK, __global uchar *SK, __private ulong C) {
  __private uchar X0, X1, X2, X3, X4, X5, X6, X7;
  __private uchar F0_OUT, F1_OUT;

  //cipher round
  X0 = byte(P, 0) + WK[0];
  printf("X0: %x\n", X0);
  X2 = byte(P, 2) ^ WK[1];
  X4 = byte(P, 4) + WK[2];
  X6 = byte(P, 6) ^ WK[3];
  X1 = byte(P, 1);
  X3 = byte(P, 3);
  X5 = byte(P, 5);
  X7 = byte(P, 7);

  #pragma unroll
  for (int i = 0; i < 31; ++i) {
    //cipher round
    F0(X6, F0_OUT);
    uchar Xn0 = X7 ^ (F0_OUT + SK[4*i + 3]);
    uchar Xn1 = X0;
    F1(X0, F1_OUT);
    uchar Xn2 = X1 + (F1_OUT ^ SK[4*i]);
    uchar Xn3 = X2;
    F0(X2, F0_OUT);
    uchar Xn4 = X3 ^ (F0_OUT + SK[4*i + 1]);
    uchar Xn5 = X4;
    F1(X4, F1_OUT);
    uchar Xn6 = X5 + (F1_OUT ^ SK[4*i + 2]);
    uchar Xn7 = X6;

    X0 = Xn0; X1 = Xn1; X2 = Xn2; X3 = Xn3;
    X4 = Xn4; X5 = Xn5; X6 = Xn6; X7 = Xn7;
  }

  // Last iteration (32th)
  //cipher round
  uchar Xn0 = X0;
  F1(X0, F1_OUT);
  uchar Xn1 = X1 + (F1_OUT ^ SK[124]);
  uchar Xn2 = X2;
  F0(X2, F0_OUT);
  uchar Xn3 = X3 ^ (F0_OUT + SK[125]);
  uchar Xn4 = X4;
  F1(X4, F1_OUT);
  uchar Xn5 = X5 + (F1_OUT ^ SK[126]);
  uchar Xn6 = X6;
  F0(X6, F0_OUT);
  uchar Xn7 = X7 ^ (F0_OUT + SK[127]);

  X0 = Xn0; X1 = Xn1; X2 = Xn2; X3 = Xn3;
  X4 = Xn4; X5 = Xn5; X6 = Xn6; X7 = Xn7;

  //cipher round
  uchar C0 = X0 + WK[4];
  uchar C1 = X1;
  uchar C2 = X2 ^ WK[5];
  uchar C3 = X3;
  uchar C4 = X4 + WK[6];
  uchar C5 = X5;
  uchar C6 = X6 ^ WK[7];
  uchar C7 = X7;

  C = concat64(C7, C6, C5, C4, C3, C2, C1, C0);
}

__kernel void hightCipher(__global ulong* P, __global uchar *WK, __global uchar *SK, __global ulong* C){

  __private int gid;
  gid = get_global_id(0);

  printf("SK: \n");
  for(int i = 0; i < 128; i++){
  	printf("%x", SK[i]);
  }
  printf("\nWK: \n");
  for(int i = 0; i < 4; i++){
  	printf("%x", WK[i]);
  }

  __private ulong _P;
  _P = P[gid];

  printf("aaaa");
  printf("%16llx\n", _P);

  __local ulong outCipher;
  
  /* encryption */
  hight_encrypt(_P, WK, SK, outCipher);

  printf("%16llx\n", outCipher);

  C[gid] = outCipher;
}

__kernel void hightCtrCipher(__global ulong* P, __global uchar *WK, __global uchar *SK, __global ulong* C){
  
  __private int gid;
  gid = get_global_id(0);

  /* initialize counter */
  __private ulong counter; 
  /* increment the counter by gid */
  counter = (ulong)gid;
  
  __local ulong outCipher;
  
  /* encryption */
  hight_encrypt(counter, WK, SK, outCipher);

  C[gid] = outCipher ^ P[gid];
}

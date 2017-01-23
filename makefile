apple: *.c
	gcc -o *.c -framework opencl -I aes_ctr
ubuntu: *.c
	gcc -I /usr/local/cuda-7.0/include/ -I aes_ctr -L /usr/local/cuda-7.0/lib64/  -o test *.c -lOpenCL


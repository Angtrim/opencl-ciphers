SOURCES := $(shell find . -name '*.c')

apple: *.c
	gcc -o *.c -framework opencl -I aes_ctr
ubuntu: *.c
	cc -I /usr/local/cuda-8.0/include/ -I aes_ctr -L /usr/local/cuda-8.0/lib64/  -o test $(SOURCES) -lOpenCL


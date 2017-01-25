SOURCES := $(shell find . -name '*.c')
BLOCKSIZE := 1
COUNT := 128 

apple: *.c
	gcc -o *.c -framework opencl -I aes_ctr
ubuntu: *.c
	dd if=/dev/zero of=nullbytes.txt bs=$(BLOCKSIZE) count=$(COUNT)
	cc -I /usr/local/cuda-8.0/include/ -I aes_ctr -L /usr/local/cuda-8.0/lib64/  -o test $(SOURCES) -lOpenCL



SOURCES := $(shell find . -name '*.c')
COMPILE_LINUX := cc -I /usr/local/cuda-8.0/include/ -I aes_ctr -L /usr/local/cuda-8.0/lib64/  -ow test $(SOURCES) -lOpenCL
COMPILE_APPLE := gcc -o $(SOURCES) -framework opencl -I aes_ctr


create_plain:
	dd if=/dev/zero of=nullbytes.txt bs=$(BLOCKSIZE) count=$(COUNT)

apple: *.c
	$(COMPILE_APPLE)

ubuntu: *.c
	$(COMPILE_LINUX)	




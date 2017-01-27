SOURCES := $(shell find . -name '*.c')
BLOCKSIZE := 1
COUNT := 128 
COMPILE_LINUX := cc -I /usr/local/cuda-8.0/include/ -I aes_ctr -L /usr/local/cuda-8.0/lib64/  -o test $(SOURCES) -lOpenCL
COMPILE_APPLE := gcc -o *.c -framework opencl -I aes_ctr
PLATFORM := 

test_linux: $(eval PLATFORM=linux) create_plain test_aes

test_apple: create_plain apple
	./test

create_plain:
	dd if=/dev/zero of=nullbytes.txt bs=$(BLOCKSIZE) count=$(COUNT)

apple: *.c
	$(COMPILE_APPLE)

ubuntu: *.c
	$(COMPILE_LINUX)	

#test list
test_aes: aes128 aes192 aes256

aes128:
ifeq ($(PLATFORM),linux)
	$(COMPILE_LINUX) -DAES128
	./test
endif

aes192:
ifeq ($(PLATFORM),linux)
	$(COMPILE_LINUX) -DAES192
	./test
endif

aes256:
ifeq ($(PLATFORM),linux)
	$(COMPILE_LINUX) -DAES256
	./test
endif


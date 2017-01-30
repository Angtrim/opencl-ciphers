SOURCES := $(shell find . -name '*.c')
COMPILE_LINUX := cc -I /usr/local/cuda-8.0/include/ -L /usr/local/cuda-8.0/lib64/ -o test prova.c  $(SOURCES) -lOpenCL
COMPILE_APPLE := gcc -o test $(SOURCES) -framework opencl -I aes_ctr


create_plain:
	#echo -n -e '\x53\x9c\x7a\x6e\x4c\x11\x35\xba\xe1\xa4\x8e\x7e\xb1\xe7\x57\x15' > nullbytes.txt
	#dd if=/dev/zero of=camellia_plaintext.txt bs=16 count=2

apple: $(SOURCES)
	$(COMPILE_APPLE)

ubuntu: create_plain $(SOURCES)
	$(COMPILE_LINUX)	




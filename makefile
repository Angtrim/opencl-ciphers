SOURCES := $(shell find . -name '*.c')

ifdef OPENCL_INC
  CL_CFLAGS = -I $(OPENCL_INC)
endif

ifdef OPENCL_LIB
  CL_LDFLAGS = -L $(OPENCL_LIB)
endif

COMPILE_LINUX := cc $(CL_CFLAGS) $(CL_LDFLAGS) -o test $(SOURCES) -lOpenCL 
COMPILE_APPLE := gcc -o test $(SOURCES) -framework opencl -I aes_ctr

create_plain:
	#echo -n -e '\x53\x9c\x7a\x6e\x4c\x11\x35\xba\xe1\xa4\x8e\x7e\xb1\xe7\x57\x15' > nullbytes.txt
	#dd if=/dev/zero of=camellia_plaintext.txt bs=16 count=2

apple: $(SOURCES)
	$(COMPILE_APPLE)

ubuntu: create_plain $(SOURCES)
	$(COMPILE_LINUX)	




SOURCES := $(shell find . -name '*.c')

ifdef OPENCL_INC
  CL_CFLAGS = -I $(OPENCL_INC)
endif

ifdef OPENCL_LIB
  CL_LDFLAGS = -L $(OPENCL_LIB)
endif

COMPILE_LINUX := cc $(CL_CFLAGS) $(CL_LDFLAGS) -o test $(SOURCES) -lOpenCL 
COMPILE_APPLE := gcc -o test $(SOURCES) -framework opencl -I aes_ctr

ctr_plaintext*:
	
	COUNT=65536; for i in $$(seq 1 6); do dd if=/dev/zero bs=16 count=$$COUNT of=ctr_plaintext$$i; COUNT=$$(($$COUNT*4)); done
	

apple: ctr_plaintext* $(SOURCES)
	
	$(COMPILE_APPLE)

ubuntu: $(SOURCES)
	
	$(COMPILE_LINUX)	




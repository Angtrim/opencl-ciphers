SOURCES := $(shell find . -name '*.c')

ifdef OPENCL_INC
  CL_CFLAGS = -I $(OPENCL_INC)
endif

ifdef OPENCL_LIB
  CL_LDFLAGS = -L $(OPENCL_LIB)
endif

COMPILE_LINUX := cc $(CL_CFLAGS) $(CL_LDFLAGS) -o test $(SOURCES) -lOpenCL 
COMPILE_APPLE := gcc -o test $(SOURCES) -framework opencl 


apple: $(SOURCES)
	
	$(COMPILE_APPLE)

ubuntu: $(SOURCES)
	
	$(COMPILE_LINUX)	




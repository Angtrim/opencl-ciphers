#ifndef TESTAES_H
#define TESTAES_H
#ifdef __APPLE__
#include <OpenCL/opencl.h>
#else
#include <CL/cl.h>
#endif

int testAESAll(cl_device_id* device_id);

#endif

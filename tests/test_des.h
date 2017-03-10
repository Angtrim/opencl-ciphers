#ifndef TESTDES_H
#define TESTDES_H
#ifdef __APPLE__
#include <OpenCL/opencl.h>
#else
#include <CL/cl.h>
#endif
int testDESAll(cl_device_id* device_id);

#endif

#ifndef TESTCLEFIA_H
#define TESTCLEFIA_H
#ifdef __APPLE__
#include <OpenCL/opencl.h>
#else
#include <CL/cl.h>
#endif
int testClefiaAll(cl_device_id* device_id);

#endif

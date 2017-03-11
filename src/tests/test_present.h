#ifndef TESTPRESENT_H
#define TESTPRESENT_H
#ifdef __APPLE__
#include <OpenCL/opencl.h>
#else
#include <CL/cl.h>
#endif
int testPresentAll(cl_device_id* device_id);

#endif
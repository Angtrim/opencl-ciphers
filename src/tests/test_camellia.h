#ifndef TESTCAMELIA_H
#define TESTCAMELIA_H
#ifdef __APPLE__
#include <OpenCL/opencl.h>
#else
#include <CL/cl.h>
#endif

int testCamelliaAll(cl_device_id* device_id);

#endif

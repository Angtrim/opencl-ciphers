#ifndef TESTHIGHT_H
#define TESTHIGHT_H
#ifdef __APPLE__
#include <OpenCL/opencl.h>
#else
#include <CL/cl.h>
#endif
int testHightAll(cl_device_id* device_id);

#endif

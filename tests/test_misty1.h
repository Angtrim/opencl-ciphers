#ifndef TESTMISTY1_H
#define TESTMISTY1_H
#ifdef __APPLE__
#include <OpenCL/opencl.h>
#else
#include <CL/cl.h>
#endif
int testMisty1All(cl_device_id* device_id);

#endif

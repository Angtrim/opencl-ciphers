#ifndef TESTSEED_H
#define TESTSEED_H
#ifdef __APPLE__
#include <OpenCL/opencl.h>
#else
#include <CL/cl.h>
#endif
int testSeedAll(cl_device_id* device_id);

#endif
#ifndef TEST_UTILS_CIPHER_H
#define TEST_UTILS_CIPHER_H
#ifdef __APPLE__
#include <OpenCL/opencl.h>
#else
#include <CL/cl.h>
#endif
void logIt(char* message);

#endif
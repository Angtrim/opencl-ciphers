#ifndef BENCHMARKS_H
#define BENCHMARKS_H
#include "benchmarks_utils.h"

#include "benchmarks_aes.h"
#include "benchmarks_des.h"
#include "benchmarks_camellia.h"
#include "benchmarks_misty1.h"
#include "benchmarks_seed.h"
#include "benchmarks_present.h"
#include "benchmarks_hight.h"
#include "benchmarks_clefia.h"

int mainBench(cl_device_id* device_id);

#endif

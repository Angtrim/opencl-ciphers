#ifndef MISTY1_EXPANSION_H
#define MISTY1_EXPANSION_H

#include <stdint.h>
typedef uint32_t word;
typedef uint8_t byte;

#define KEY_SIZE 16
#define EX_KEY_SIZE 32

void misty1_expandkey(uint16_t *EK, uint8_t *K);

#endif



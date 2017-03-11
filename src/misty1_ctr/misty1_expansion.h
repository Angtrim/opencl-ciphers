#ifndef MISTY1_EXPANSION_H
#define MISTY1_EXPANSION_H

#include <stdint.h>
#include "../cipher_utils.h"


#define KEY_SIZE 16
#define EX_KEY_SIZE 32

void misty1_expandkey(uint16_t *EK, uint8_t *K);

#endif



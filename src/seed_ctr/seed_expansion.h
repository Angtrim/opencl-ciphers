#ifndef SEED_EXPANSION_H
#define SEED_EXPANSION_H

#include <stdint.h>

void seed_old_expandkey(uint32_t *Key, uint32_t* K);
void seed_expandkey(uint32_t *Key, uint32_t* K);
#endif

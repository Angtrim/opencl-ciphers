#ifndef CAMELLIA_EXPANSION_H
#define CAMELLIA_EXPANSION_H

#include <stdint.h>

void camellia_128_expandkey(uint64_t *Ko, uint64_t *kw, uint64_t *k, uint64_t *ke);
void camellia_192_expandkey(uint64_t *Ko, uint64_t *kw, uint64_t *k, uint64_t *ke);
void camellia_256_expandkey(uint64_t *Ko, uint64_t *kw, uint64_t *k, uint64_t *ke);

#endif

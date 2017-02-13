#ifndef CLEFIA_EXPANSION_H
#define CLEFIA_EXPANSION_H

#include <stdint.h>

void Clefia128KeySet(unsigned char *rk, const unsigned char *skey);
void Clefia192KeySet(unsigned char *rk, const unsigned char *skey);
void Clefia256KeySet(unsigned char *rk, const unsigned char *skey);

#endif


#ifndef DES_EXPANSION_H
#define DES_EXPANSION_H

#include <stdint.h>
typedef uint32_t word;
typedef uint8_t byte;


typedef struct {
  uint32_t esk[32]; /*!< DES encryption subkeys */
  uint32_t dsk[32]; /*!< DES decryption subkeys */
} des_context;

typedef struct {
  uint32_t esk[96]; /*!< Triple-DES encryption subkeys */
  uint32_t dsk[96]; /*!< Triple-DES decryption subkeys */
} des3_context;


void des_expandkey(des_context* K, uint8_t *key);
void tdes2_expandkey(des3_context* K, uint8_t *key);
void tdes3_expandkey(des3_context* K, uint8_t *key);

#endif


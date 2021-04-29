/* (m1,c1) = (5530a0, 156a97) (m2,c2) = (bf2eef,73f787) */
#pragma once
#include "Encryption.h"
#include "Decryption.h"
#include <stdlib.h>


typedef struct matching_keys{
    uint32_t k1;
    uint32_t k2;
}match ;

uint32_t attack(uint32_t m1, uint32_t m2, uint32_t c1, uint32_t c2);

uint32_t findCorrectKey();


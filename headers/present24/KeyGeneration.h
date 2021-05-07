#pragma once
#include <stdlib.h>
#include <stdint.h>

//Generate 11 sub keys by using circular shifts, Sbox on 4 bits and xor.
void subKeyGeneration(uint32_t master_key, uint32_t* sub_keys);
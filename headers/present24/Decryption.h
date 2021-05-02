#pragma once
#include <stdint.h>


typedef struct Decryption{
    uint32_t encrypted;
    uint32_t key;
}Decrypt;

uint32_t revSubstitution(uint32_t state);

uint32_t revPermutation(uint32_t state);

uint32_t decryption(uint32_t message, uint32_t key, uint32_t* sub_keys);
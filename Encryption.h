#pragma once
#include <stdio.h>
#include <stdint.h>

typedef struct Encryption{
    uint32_t message;
    uint32_t encrypted;
    uint32_t *keys;
}Encrypt;

uint32_t substitution(uint32_t state);

uint32_t permutation(uint32_t state);

uint32_t* subKeyGeneration(uint32_t masterKey, uint32_t* keys);

uint32_t encryption(uint32_t message, uint32_t* keys);


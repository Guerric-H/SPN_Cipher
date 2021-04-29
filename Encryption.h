#pragma once
#include <stdio.h>
#include <stdint.h>

typedef struct Encryption{
    uint32_t message;
    uint32_t encrypted;
    uint32_t *subKeys;
}Encrypt;

void initEncryption(Encrypt*);

uint32_t KeyXOR(uint32_t state, uint32_t subKey);

uint32_t Substitution(uint32_t state);

uint32_t Permutation(uint32_t state);

uint32_t* SubKeyGeneration(uint32_t masterKey, uint32_t* subKeys);

uint32_t Encryption(uint32_t message, uint32_t* keys);


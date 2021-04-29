#pragma once
#include <stdio.h>
#include <stdint.h>
//Void in return, wether state is a pointer or not.



typedef struct Decryption{
    uint32_t message;
    uint32_t encrypted;
    uint32_t *keys;
}Decrypt;

uint32_t revSubstitution(uint32_t state);

uint32_t revPermutation(uint32_t state);

uint32_t decryption(uint32_t message, uint32_t* keys);
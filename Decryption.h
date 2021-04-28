#pragma once
#include <stdio.h>
#include <stdint.h>
//Void in return, wether state is a pointer or not.

void RevKeyXOR(uint32_t* state, uint32_t subKey);

void RevSubstitution(uint32_t* state,uint32_t* substitutionTable);

void RevPermutation(uint32_t* state,uint32_t* permutationTable);

uint32_t Decryption(uint32_t message, uint32_t* keys);
#pragma once
#include "KeyGeneration.h"

//Structure that contains the data given by the user for a decryption.
typedef struct Decryption
{
    uint32_t encrypted;
    uint32_t key;
} Decrypt;

//Putting our message (decrypted a number of time or not) in a reverse Sbox and getting back the modified message
uint32_t revSubstitution(uint32_t state);

/*Putting our message (decrypted a number of time or not) and replacing each bit by the content of the reverse permutationTable.
Return the modified message
*/
uint32_t revPermutation(uint32_t state);

/*Global Function generate subKey from the @param key, XOR the encrypted with the 11th key.
XOR and apply functions above 10 times
More details in Decryption.c
*/
uint32_t decryption(uint32_t message, uint32_t key, uint32_t *sub_keys);
#pragma once
#include <stdint.h>

//Structure that contains the data given by the user for an encryption.
typedef struct Encryption{
    uint32_t message;
    uint32_t key;
}Encrypt;

//Putting our message (encrypted a number of time or not) in a Sbox and getting back the modified message
uint32_t substitution(uint32_t state);
//Putting our message (encrypted a number of time or not) and replacing each bit by the content of the permutationTable.
//Return the modified message
uint32_t permutation(uint32_t state);

//Global function, generate subKey from the @param key and apply functions above and a XOR 10 times. 
//Finally XOR a last time with the 11th subkey.
//More details in Encryption.c
uint32_t encryption(uint32_t message, uint32_t key);


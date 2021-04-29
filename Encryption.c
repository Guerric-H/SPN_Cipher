#include "Encryption.h"

uint32_t KeyXOR(uint32_t state, uint32_t subKey) { 
    return state ^ subKey; 
}

uint32_t Substitution(uint32_t state) {
    uint16_t static sbox[16] = {12, 5, 6, 11, 9, 0, 10, 13, 3, 14, 15, 8, 4, 7, 1, 2};

    uint32_t res = 0, tmp = 0;
    for (uint32_t i = 0; i < 6; i++) {
        res = res << 4;
        tmp = (state >> (5 - i) * 4) & 0xf ;
        res |= sbox[tmp]; 
    }
    return res;
}

uint32_t Permutation(uint32_t state) {
    uint32_t static permutationTable[24] = {0, 6, 12, 18, 1, 7, 13, 19, 2, 8, 14, 20, 
                                            3, 9, 15, 21, 4, 10, 16, 22, 5, 11, 17, 23} ;
    uint32_t res = 0, tmp = 0;
    for (uint32_t i = 0; i < 24; i++) {
        tmp = ((state >> i) & 1);
        res |= (tmp << permutationTable[i]);
    }
    return res;
}

uint32_t* SubKeyGeneration(uint32_t masterKey, uint32_t* subKeys) {
    uint16_t static sbox[16] = {12, 5, 6, 11, 9, 0, 10, 13, 3, 14, 15, 8, 4, 7, 1, 2};
    uint64_t firstCopy = masterKey, tmp; 
    uint16_t secondCopy = 0;

    firstCopy = (firstCopy << 40);

    for (uint32_t i = 1; i <= 11 ; i++) {
        subKeys[i-1] = (firstCopy & 0xffffff);

        tmp = secondCopy; 
        tmp = (firstCopy << 61) | (tmp << 45) | (firstCopy >> 19);
        secondCopy = (firstCopy >> 3) & 0xffff;
        firstCopy  = tmp;
        
        tmp = sbox[tmp >> 60];                    
        firstCopy = (tmp << 60) | (firstCopy << 4) >> 4;
        
        firstCopy  = (((firstCopy << 60) >> 60) ^ (i >> 1)) | (firstCopy >> 4) << 4;
        secondCopy = (secondCopy ^ (i << 15) ) | (secondCopy & 0x7fff);
    }
    return subKeys;
}

uint32_t Encryption(uint32_t message, uint32_t* subKeys) {
    for (uint32_t i = 0; i < 10 ; i++) {
        message = KeyXOR(message, subKeys[i]);
        message = Substitution(message);
        message = Permutation(message);
    }
    message = KeyXOR(message, subKeys[10]);
    printf ("%06x\n", message);
    return message;
}
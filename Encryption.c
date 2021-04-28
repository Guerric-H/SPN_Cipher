#include "Encryption.h"

uint32_t KeyXOR(uint32_t state, uint32_t subKey){ 
    return state ^ subKey; 
}


uint32_t Substitution(uint32_t state){

    uint32_t static sbox[16] = {12, 5, 6, 11, 9, 0, 10, 13, 3, 14, 15, 8, 4, 7, 1, 2};

    uint32_t res = 0, tmp = 0;
    for (uint32_t i = 0; i < 6; i++) {
        res << 4;
        tmp = (state >> (5 - i) * 4) & 15 ;
        res |= sbox[tmp]; 
    }
    return res;
}


uint32_t Permutation(uint32_t state, uint32_t* permutationTable) {

    uint32_t static permutationTable[24] = {0, 6, 12, 18, 1, 7, 13, 19, 2, 8, 14, 20, 
                                            3, 9, 15, 21, 4, 10, 16, 22, 5, 11, 17, 23} ;

    uint32_t res = 0, tmp = 0;
    for (uint32_t i = 0; i < 24; i++) {
        tmp = (i >> state ) & 1;
        res |= tmp << permutationTable[i];
    }
    return res;
}

uint32_t* SubKeyGeneration(uint32_t masterKey, uint32_t* subKeys) {
    uint64_t firstCopy = (masterKey << 40);
    uint16_t secondCopy = 0;
    for (uint32_t i; i < 11 ; i++) {
        subKeys[i] = (firstCopy & 33554431); // 33554431 -> (pow(2, 25) - 1) écriture alternative : 3 >> ~0
        firstCopy = (secondCopy & 8) ;
    }
}
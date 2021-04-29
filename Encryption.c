#include "Encryption.h"


uint32_t substitution(uint32_t state) {
    uint16_t static sbox[16] = {12, 5, 6, 11, 9, 0, 10, 13, 3, 14, 15, 8, 4, 7, 1, 2};

    uint32_t res = 0, tmp = 0;
    for (uint32_t i = 0; i < 6; i++) {
        res = res << 4;
        tmp = (state >> (5 - i) * 4) & 15 ;
        res |= sbox[tmp]; 
    }
    return res;
}

uint32_t permutation(uint32_t state) {
    uint32_t static p_table[24] = {0, 6, 12, 18, 1, 7, 13, 19, 2, 8, 14, 20, 
                                            3, 9, 15, 21, 4, 10, 16, 22, 5, 11, 17, 23} ;
    uint32_t res = 0, tmp = 0;
    for (uint32_t i = 0; i < 24; i++) {
        tmp = ((state >> i) & 1);
        res |= (tmp << p_table[i]);
    }
    return res;
}

uint32_t* subKeyGeneration(uint32_t masterKey, uint32_t* subKeys) {
    uint16_t static sbox[16] = {12, 5, 6, 11, 9, 0, 10, 13, 3, 14, 15, 8, 4, 7, 1, 2};
    uint64_t first_copy = masterKey, tmp; 
    uint16_t second_copy = 0;

    first_copy = (first_copy << 40);

    for (uint32_t i = 1; i <= 11 ; i++) {
        subKeys[i-1] = first_copy & 0xffffff;

        tmp = second_copy;

        tmp = (first_copy << 61) | (tmp << 45) | (first_copy >> 19);
        second_copy = first_copy << 45 >> 48 ;
        first_copy  = tmp;
        
        tmp = sbox[tmp >> 60];                    
        first_copy = (tmp << 60) | (first_copy & 0xfffffffffffffff);

        first_copy  = ((first_copy & 0xf) ^ (i >> 1)) | (first_copy & 0xfffffffffffffff);
        second_copy = (second_copy ^ (i << 15) ) | (second_copy & 0x7fff);
    }
    return subKeys;
}

uint32_t encryption(uint32_t message, uint32_t* keys) {
   
    for (int i = 0; i < 10 ; i++) {
        message = message ^ keys[i];
        message = substitution(message);
        message = permutation(message);
    }
    message = message ^ keys[10];
    return message;
}
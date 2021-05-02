#include "Encryption.h"
#include "KeyGeneration.h"
#include<stdlib.h>

uint32_t substitution(uint32_t state) {
    uint16_t static sbox[16] = {12, 5, 6, 11, 9, 0, 10, 13, 3, 14, 15, 8, 4, 7, 1, 2};

    uint32_t res = 0, tmp = 0;
    for (uint32_t i = 0; i < 6; i++) {
        res = res << 4;
        tmp = (state >> (5 - i) * 4) & 0xf ;
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

uint32_t encryption(uint32_t message, uint32_t key, uint32_t* sub_keys) {
   
    sub_keys = subKeyGeneration(key);
    
    for (int i = 0; i < 10 ; i++) {
        message = message ^ sub_keys[i];
        message = substitution(message);
        message = permutation(message);
    }
    message = message ^ sub_keys[10];
    
    free(sub_keys);
    return message;
}
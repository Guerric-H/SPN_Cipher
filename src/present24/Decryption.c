#include "Decryption.h"
#include "KeyGeneration.h"
#include<stdlib.h>

uint32_t revSubstitution(uint32_t state){
    uint32_t static rev_sbox[16] = {5, 14, 15, 8, 12, 1, 2, 13,
                                     11, 4, 6, 3, 0, 7, 9, 10};
    uint32_t res = 0, tmp = 0;
    for (uint32_t i = 0; i < 6; i++) {
        res = res << 4;
        tmp = (state >> (5 - i) * 4) & 15 ;
        res |= rev_sbox[tmp]; 
    }
    return res;
}

uint32_t revPermutation(uint32_t state) {
    uint32_t static rev_p_table[24] = {0, 4, 8, 12, 16, 20, 1, 5, 9, 13, 17, 21,
                                        2, 6, 10, 14, 18, 22, 3, 7, 11, 15, 19, 23};
    uint32_t res = 0, tmp = 0;
    for (int i = 0; i < 24; i++) {
        tmp = ((state >> i) & 1);
        res |= (tmp << rev_p_table[i]);
    }
    return res;
}

uint32_t decryption (uint32_t encrypted, uint32_t key, uint32_t*  sub_keys){
    
    sub_keys = subKeyGeneration(key);

    encrypted = encrypted ^ sub_keys[10];
    for (int i = 9; i >= 0 ; i--) {
        encrypted = revPermutation(encrypted);
        encrypted = revSubstitution(encrypted);
        encrypted = encrypted ^ sub_keys[i];
    }

    free(sub_keys);
    return encrypted ;
}
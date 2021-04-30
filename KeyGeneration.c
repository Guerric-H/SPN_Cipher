#include "KeyGeneration.h"

uint32_t* subKeyGeneration(uint32_t masterKey) {

    uint32_t* sub_keys = malloc(sizeof(uint32_t)*11);

    uint16_t static sbox[16] = {12, 5, 6, 11, 9, 0, 10, 13, 3, 14, 15, 8, 4, 7, 1, 2};
    uint64_t first_copy = masterKey, tmp; 
    uint16_t second_copy = 0;

    first_copy = (first_copy << 40);

    for (uint32_t i = 1; i <= 11 ; i++) {
        sub_keys[i-1] = first_copy & 0xffffff;

        tmp = second_copy;

        tmp = (first_copy << 61) | (tmp << 45) | (first_copy >> 19);
        second_copy = first_copy << 45 >> 48 ;
        first_copy  = tmp;
        
        tmp = sbox[tmp >> 60];                    
        first_copy = (tmp << 60) | (first_copy & 0xfffffffffffffff);

        first_copy  = ((first_copy & 0xf) ^ (i >> 1)) | (first_copy & 0xfffffffffffffff);
        second_copy = (second_copy ^ (i << 15) ) | (second_copy & 0x7fff);
    }
    return sub_keys;
}
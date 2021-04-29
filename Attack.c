#include "Attack.h"

uint32_t attack(uint32_t m1, uint32_t m2, uint32_t c1, uint32_t c2){
    
    uint32_t* hiddeux = malloc(sizeof(uint32_t)*0x1000000);
    uint32_t* hihun = malloc(sizeof(uint32_t)*0x1000000);
    //match* kk = malloc(sizeof(match)*0x1000000);
    uint32_t* keys = malloc(sizeof(uint32_t)*11);
    //uint32_t* match1 = malloc(sizeof(uint32_t)*0x1000);
    int k = 0;

    for (int i = 0 ; i < 0x1000000 ; i++){
        keys = subKeyGeneration(i,keys);
        hihun[i] = encryption(m1,keys);
        hiddeux[i] = decryption(c1,keys);
    }
    /*
    for (int i = 0 ; i < 0x1000000 ; i ++)
        for (int j = 0 ; i < 0x1000000 ; j++)
            if (hihun[i] == hiddeux[j])
                k++;
                */
 return k ;
}
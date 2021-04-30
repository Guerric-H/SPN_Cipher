#include "Attack.h"
#include "Encryption.h"
#include "Decryption.h"

uint32_t attack(uint32_t m1, uint32_t m2, uint32_t c1, uint32_t c2){
    /* 
    uint32_t* hiddeux = malloc(sizeof(uint32_t)*0x1000000);
    uint32_t* hihun = malloc(sizeof(uint32_t)*0x1000000);
    //match* kk = malloc(sizeof(match)*0x1000000);
    uint32_t* keys = malloc(sizeof(uint32_t)*11);
    //uint32_t* match1 = malloc(sizeof(uint32_t)*0x1000); */
    int k = 0;

    /* for (int i = 0 ; i < 0x1000000 ; i++){
        keys = subKeyGeneration(i,keys);
        hihun[i] = encryption(m1,keys);
        hiddeux[i] = decryption(c1,keys);
    }
    
    for (int i = 0 ; i < 0x1000000 ; i ++)
        for (int j = 0 ; i < 0x1000000 ; j++)
            if (hihun[i] == hiddeux[j])
                k++; */
                
    return k ;
}

List* swap(List* list, List trade, int pos_a, int pos_b) {
    trade = list[pos_a];
    list[pos_a] = list[pos_b];
    list[pos_b] = trade;
    return list;
}

int partitioning(List* list, int first, int last) {
    int pos = first;
    List trade;

    for (int i = first; i < last; i++) {
        if (list[i].message <= list[last].message) {
            list = swap(list, trade, pos, i);
            pos++;
        }
    }

    list = swap(list, trade, pos, last);
    return pos;
}

List* quickSort(List* list, int first, int last) {
    if (first < last) {
        int pivot = partitioning (list, first, last);
        list = quickSort(list, first, pivot - 1);
        list = quickSort(list, pivot + 1, last);
    }
    return list;
}

/* List* fillList (List* list, uint32_t clear) {
    for (int i = 0; i < 0x1000000; i++) {
        list[i].key = i;
        list[i].message = Encryption(clear,)
    }
} */
#pragma once

#include <stdint.h>
/* (m1,c1) = (5530a0, 156a97) (m2,c2) = (bf2eef,73f787) */
//Linked Combination structures and functions.

typedef struct KResult {
    uint32_t k1;
    uint32_t k2;
}AttackResult;

typedef struct MKAttack{
    uint32_t m1;
    uint32_t c1;    
    uint32_t m2;
    uint32_t c2;
}AttackInput;


typedef struct CandidateKeys CandidateKeys;
struct CandidateKeys{
    uint32_t k1;
    uint32_t k2;
    CandidateKeys* next;
    CandidateKeys* previous;
};

typedef struct KList{
    CandidateKeys *first;
    int size;
}KeysList;

KeysList* init();

void insert(KeysList* list,uint32_t k1, uint32_t k2); 

void remove_element(KeysList* list, CandidateKeys* element);

void free_list(KeysList* list);

/////////////////////////////////////////////////////////////////
//Combination of key and result to be sorted and compared.

typedef struct KeyMessageCombination {
    uint32_t key;
    uint32_t result;
} Combination;

void swap(Combination* list, Combination trade, int pos_a, int pos_b);

int partitioning(Combination* list, int first, int last);

void quickSort(Combination* list, int first, int last);

void fillLists (Combination* enc_list, Combination* dec_list, AttackInput input, uint32_t* sub_keys);

void dichotomous_search (KeysList* KeysList, Combination* enc_list, Combination dec, uint32_t begin, uint32_t end);

/////////////////////////////////////////////////////////////////
//Englobing functions to attack with 2 couple message and encrypted

AttackResult attack(AttackInput, uint32_t* sub_keys);
AttackResult findCorrectKey(KeysList* candidates, AttackInput input, uint32_t* sub_keys);
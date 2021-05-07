#pragma once
#include <stdio.h>
#include <stddef.h>
#include <time.h>
#include "Encryption.h"
#include "Decryption.h"

//Struct used to return valid keys.
typedef struct KResult
{
    uint32_t k1;
    uint32_t k2;
} AttackResult;

//Struct used to give information for attack.
typedef struct MKAttack
{
    uint32_t m1;
    uint32_t c1;
    uint32_t m2;
    uint32_t c2;
} AttackInput;

//Struct element of the List below, pointer to next and previous ; contains k1, k2
typedef struct CandidateKeys CandidateKeys;
struct CandidateKeys
{
    uint32_t k1;
    uint32_t k2;
    CandidateKeys *next;
    CandidateKeys *previous;
};

//Struct Linked List, pointer to first element. Contain the size of the Linked List
typedef struct KList
{
    CandidateKeys *first;
    int size;
} KeysList;

//Initialize the Linked List
KeysList *init();

//Insert a new element to the list
void insert(KeysList *list, uint32_t k1, uint32_t k2);

//Remove a specific element of the list
void remove_element(KeysList *list, CandidateKeys *element);

//Free the Linked List
void free_list(KeysList *list);

//Combination of key and result to be sorted and compared.
typedef struct KeyMessageCombination
{
    uint32_t key;
    uint32_t result;
} Combination;

void swap(Combination *list, int pos_a, int pos_b);

int partitioning(Combination *list, int first, int last);

void quickSort(Combination *list, int first, int last);

//Fill 2 list with pre-defined size, meant to be used with dynamic array of size 2^24 (NON PORTABLE)
void fillLists(Combination *enc_list, Combination *dec_list, AttackInput input, uint32_t *sub_keys);

void testing_key(KeysList *result, uint32_t k1, uint32_t k2, AttackInput input, uint32_t *sub_keys);

//Dichotomous search global function
size_t dichotomous_verification(KeysList *KeysList, uint32_t *sub_keys, AttackInput input, Combination *enc_list, Combination dec, uint32_t begin, uint32_t end);

//Englobing function to attack with 2 couple message and encrypted
KeysList *attack(AttackInput, uint32_t *sub_keys);

//Remove every key combination that are not valid
AttackResult findCorrectKey(KeysList *candidates, AttackInput input, uint32_t *sub_keys);
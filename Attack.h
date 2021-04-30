/* (m1,c1) = (5530a0, 156a97) (m2,c2) = (bf2eef,73f787) */
#pragma once
#include "Encryption.h"
#include "Decryption.h"
#include <stdlib.h>

typedef struct key_message_combination {
    uint32_t key;
    uint32_t message;
} List;

typedef struct matching_keys{
    uint32_t k1;
    uint32_t k2;
} match;

List* swap(List* list, List trade, int pos_a, int pos_b);

int partitioning(List* list, int first, int last);

List* quickSort(List* list, int first, int last);

List* fillList (List* list, uint32_t message);

uint32_t attack(uint32_t m1, uint32_t m2, uint32_t c1, uint32_t c2);

uint32_t findCorrectKey();


#include "Encryption.h"
#include "Decryption.h"
#include "Attack.h"
#include<time.h>
#include<stdlib.h>
#include<stdio.h>

void swap(Combination* list, Combination trade, int pos_a, int pos_b) {
    trade = list[pos_a];
    list[pos_a] = list[pos_b];
    list[pos_b] = trade;
}

int partitioning(Combination* list, int first, int last) {
    int pos = first;
    Combination trade;

    for (int i = first; i < last; i++) {
        if (list[i].result <= list[last].result) {
            swap(list, trade, pos, i);
            pos++;
        }
    }
    swap(list, trade, pos, last);
    return pos;
}

void quickSort(Combination* list, int first, int last) {

    if (first < last) {
        int pivot = partitioning (list, first, last);
        quickSort(list, first, pivot - 1);
        quickSort(list, pivot + 1, last);
    }
}

void fillLists (Combination* enc_list, Combination* dec_list, AttackInput input, uint32_t* sub_keys) {
    for (int i = 0; i <= 0xffffff; i++) {
        enc_list[i].key = dec_list[i].key = i;
        enc_list[i].result = encryption(input.m1, i,sub_keys);
        dec_list[i].result = decryption(input.c1, i,sub_keys);
    }
}


KeysList* init(){
    
    KeysList* list = malloc(sizeof(KeysList));
    list->first = NULL ;
    list->size = 0 ;
    return list;
}

void insert(KeysList* list, uint32_t key1, uint32_t key2){
    
    CandidateKeys* new_element = malloc(sizeof(CandidateKeys));
    
    new_element->next = list->first;
    new_element->previous = NULL;
    new_element->k1 = key1;
    new_element->k2 = key2;
    
    if(list->first){
        CandidateKeys* tmp = list->first;
        tmp->previous = new_element;
    }
    list->first = new_element;
    list->size++;
}

void remove_element(KeysList* list, CandidateKeys* element){

    if(!element)
        return;

    if(element == list->first){
        list->first = element->next; 
    }

    if(element->previous) {
        CandidateKeys* tmp = element->previous;
        tmp->next = element->next;
    }
    
    if(element->next) {
        CandidateKeys* tmp = element->next;
        tmp->previous = element->previous;
    }
    list->size--;
    element->next = element->previous = NULL ;
    free(element);
}

void free_list(KeysList* list){
    CandidateKeys* current = list->first;
    CandidateKeys* next;

    while(current){
        next = current->next;
        free(current);
        current = next;
    }
    free(list);
}

void dichotomous_search(KeysList* keys, Combination* enc_list, Combination dec, uint32_t begin, uint32_t end) {
    int mid = (begin + end)/2;
    
    if (enc_list[mid].result == dec.result) {    
        int i = 0;
        while ((mid - i) >= 0) {
            if (enc_list[mid - i].result == dec.result) {
                insert (keys, enc_list[mid - i].key, dec.key);
                i++;
            }
            else break;
        }
        i = 1;
        while ((mid + i) <=  0xffffff) {
            if (enc_list[mid + i].result == dec.result) {
                insert (keys, enc_list[mid + i].key, dec.key);
                i++;
            }
            else break;
        }
        return;
    }  
    if(begin >= end)
        return;
    if (enc_list[mid].result > dec.result)
        dichotomous_search(keys, enc_list, dec, begin, mid - 1);
    if (enc_list[mid].result < dec.result)
        dichotomous_search(keys, enc_list, dec, mid + 1, end);
}

AttackResult attack(AttackInput input, uint32_t* sub_keys){
    
    clock_t start = clock();
    double elapsed_time = 0 ;
    Combination* enc_list = malloc(sizeof(Combination)*0x1000000);
    Combination* dec_list = malloc(sizeof(Combination)*0x1000000);
    KeysList* candidate = init();

    clock_t fillListbegin = clock();
    fillLists(enc_list, dec_list, input,sub_keys);
    clock_t fillListend = clock();
    elapsed_time = ((double) (fillListend - fillListbegin)) / CLOCKS_PER_SEC;
    printf("Temps pour chiffrer et déchiffrer avec toutes les clés possibles : %f\n",elapsed_time);

    clock_t Quicksortbegin = clock();
    quickSort(enc_list, 0x0, 0xffffff);
    quickSort(dec_list, 0x0, 0xffffff);
    clock_t Quicksortend = clock();
    elapsed_time = ((double) (Quicksortend - Quicksortbegin)) / CLOCKS_PER_SEC;
    printf("Temps pour trier les listes : %f\n",elapsed_time);

    clock_t dichotomousbegin = clock();
    for(int i = 0; i <= 0xffffff; i++)
        dichotomous_search(candidate, enc_list, dec_list[i], 0, 0xffffff);
    clock_t dichotomoustend = clock();
    elapsed_time = ((double) (dichotomoustend - dichotomousbegin)) / CLOCKS_PER_SEC;
    printf("Temps pour la recherche dichotomique : %f\n",elapsed_time);
    
    clock_t removebegin = clock();
    AttackResult result = findCorrectKey(candidate, input,sub_keys);
    clock_t removeend = clock();
    elapsed_time = ((double) (removeend - removebegin)) / CLOCKS_PER_SEC;
    printf("Temps pour éliminer les mauvaises clés: %f\n",elapsed_time);

    free_list(candidate);
    free(dec_list);
    free(enc_list);

    clock_t end = clock();
    elapsed_time = ((double) (end - start)) / CLOCKS_PER_SEC;
    printf("Temps total: %f\n",elapsed_time);

    return result ;
}

AttackResult findCorrectKey(KeysList* candidates, AttackInput att,uint32_t* sub_keys) {
    
    CandidateKeys* current = candidates->first; 
    CandidateKeys* tmp;
    AttackResult result;
    result.k1 = result.k2 = 0;
        
    while (current) {
        tmp = current->next;
        if (att.c2 != encryption(encryption(att.m2, current->k1,sub_keys), current->k2,sub_keys))
            remove_element(candidates, current);
        else
        {
            result.k1 = current->k1;
            result.k2 = current->k2;
        }
            
        current = tmp;
    }

    if(!candidates->size)
        puts("Aucune combinaison de clés trouvée. Les valeurs ci-dessous sont par défaut.\n");
    return result;
}
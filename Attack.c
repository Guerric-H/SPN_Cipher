#include "Attack.h"
#include "Encryption.h"
#include "Decryption.h"

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

void fillLists (Combination* enc_list, Combination* dec_list, uint32_t m1, uint32_t c1) {
    for (int i = 0; i <= 0xffffff; i++) {
        enc_list[i].key = dec_list[i].key = i;
        enc_list[i].result = encryption(m1, i);
        dec_list[i].result = decryption(c1, i);
    }
}

/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////

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
        //insert(keys,enc_list[mid].result,dec.key);
        
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

/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////

//Englobing function for attack
uint32_t attack(uint32_t m1, uint32_t m2, uint32_t c1, uint32_t c2){

    Combination* enc_list = malloc(sizeof(Combination)*0x1000000);
    Combination* dec_list = malloc(sizeof(Combination)*0x1000000);
    KeysList* candidate = init();

    fillLists(enc_list, dec_list, m1, c1);

    quickSort(enc_list, 0x0, 0xffffff);
    quickSort(dec_list, 0x0, 0xffffff);

    for(int i = 0; i <= 0xffffff; i++)
        dichotomous_search(candidate, enc_list, dec_list[i], 0, 0xffffff);
        
    printf("Nombre de couple de clé candidates : %x\n",candidate->size);
    findCorrectKey(candidate, m2, c2);

 return 0 ;
}

uint32_t findCorrectKey(KeysList* candidates, uint32_t m2, uint32_t c2) {
    
    CandidateKeys* current = candidates->first; 
    CandidateKeys* tmp ;
    if (!current)
        return 0 ;
        
    while (current) {
        tmp = current->next;
        if (c2 != encryption(encryption(m2, current->k1), current->k2))
            remove_element(candidates, current);
        else
            printf("(k1, k2) = (%x,%x)\n", current->k1, current->k2);
        current = tmp;
    }
    return 0;
}
#include "Encryption.h"
#include "Decryption.h"
#include "Attack.h"
#include<time.h>
#include<stdlib.h>
#include<stdio.h>

//(m1,c1) = (ef333a, 250324) (m2,c2) = (fb432f,a71982)

/*Swap 2 elements in a list using a temporary*/
void swap(Combination* list, int pos_a, int pos_b) {
    Combination trade = list[pos_a];
    list[pos_a] = list[pos_b];
    list[pos_b] = trade;
}

/*Partitionning a part of a List using the range [first, last]
    Take a list's part, and for each element in range (except last), compare it with the last one
    If last is >=, swap the element with last and increase the pivot by 1 (pivot is equal to first)
*/
int partitioning(Combination* list, int first, int last) {
    int pos = first;

    for (int i = first; i < last; i++) {
        if (list[i].result <= list[last].result) {
            swap(list, pos, i);
            pos++;
        }
    }
    swap(list, pos, last);
    return pos;
}

/*Quicksort
    Use partitionning to sort the value between first and last
    use quicksort again on new first and last given by partitionning (pivot);
    Repeat until first is no longer inferior to last.
*/
void quickSort(Combination* list, int first, int last) {

    if (first < last) {
        int pivot = partitioning (list, first, last);
        quickSort(list, first, pivot - 1);
        quickSort(list, pivot + 1, last);
    }
}

/*This fill an encryption list and a decryption list, using a message for encryption, and the encrypted (from double SPN) for decryption.
    The number of keys being 2^24 in this subject, 0xffffff = 2^24 - 1. Since we start from 0, we fill the List with the right amount of encryption/decryption.
*/
void fillLists (Combination* enc_list, Combination* dec_list, AttackInput input, uint32_t* sub_keys) {
    for (int i = 0; i <= 0xffffff; i++) {
        enc_list[i].key = dec_list[i].key = i;
        enc_list[i].result = encryption(input.m1, i,sub_keys);
        dec_list[i].result = decryption(input.c1, i,sub_keys);
    }
}

/*Function used to create a simple linked list
    first is set to NULL and will be given as a next when inserting, it will be used as the end of the list
    size is 0, and will increase or decrease depending on removal or insertions.
*/
KeysList* init(){
    
    KeysList* list = malloc(sizeof(KeysList));
    list->first = NULL ;
    list->size = 0 ;
    return list;
}

/*Basic insertion
    Put the new element as the first of the list, set its values with parameters key1 and key2
    If the list is not empty, take the first element and set it's previous to the new one
    Increase size by 1.
*/
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


/*Element removal using the pointer to the element
    if NULL, do nothing.
    if element is first, remove it but take care of keeping a valid pointer to first, by setting it to the 2nd.
    if a previous element exist, set it's new next to the actual element->next.
    if a following element exist, set it's new previous to the actual element->previous.
    decrease the size by 1.
    free the element we removed.

*/
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
   // element->next = element->previous = NULL ;
    free(element);
}

/*Free the whole list, by using free until there is no element remaining.
    Then, free the list.
*/
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

void testing_key(KeysList* result, uint32_t k1, uint32_t k2, AttackInput input, uint32_t* sub_keys){
    if (input.c2 == encryption(encryption(input.m2, k1,sub_keys), k2,sub_keys)){
        insert(result, k1, k2);
    }
}

/*Standard dichotomous search :
    if the two values are matching (we found a key combination), check for adjacent values (as the two list are sorted, we might look at other values that have use different keys but produce the same output)
    if the value of begin is equal or over end, we stop, as there was no match.

    If the decryption value is less than our encryption, make the same research with end = (begin + end / 2) - 1
    If the decryption value is more than our encryption, make the same research with begin = (begin + end / 2) + 1

*/ 
void dichotomous_verification(KeysList* keys,uint32_t* sub_keys, AttackInput input, Combination* enc_list, Combination dec, uint32_t begin, uint32_t end) {
    int mid = (begin + end)/2;
    
    if (enc_list[mid].result == dec.result) {   
        int i = 0;
        while ((mid - i) >= 0) {
            if (enc_list[mid - i].result == dec.result) {
                testing_key(keys, enc_list[mid - i].key, dec.key, input, sub_keys);
                i++;
            }
            else break;
        }
        i = 1;
        while ((mid + i) <=  0xffffff) {
            if (enc_list[mid + i].result == dec.result) {
                testing_key(keys, enc_list[mid + i].key, dec.key, input, sub_keys);
                i++;
            }
            else break;
        }
        return;
    }  
    if(begin >= end)
        return;
    if (enc_list[mid].result > dec.result)
        dichotomous_verification(keys, sub_keys, input, enc_list, dec, begin, mid - 1);
    if (enc_list[mid].result < dec.result)
        dichotomous_verification(keys, sub_keys, input, enc_list, dec, mid + 1, end);
}

/*Global function for the attack
    We use of clock_t to measure the timed needed for each part of the attack.
    
    1) List allocation for Message / Key combination.
    2) Fill both lists
    3) Sort the two lists
    4) Dichotomous Search
    5) Testing Candidate Keys with m2,c2 ; remove every wrong keys.
    6) Returning the valid key combination.

Note : More than 1 combination may exist, however we would need a third (m3,c3) to verify which is the correct one. Thus, we will only return 1 key. 

*/
KeysList* attack(AttackInput input, uint32_t* sub_keys){
    
    clock_t start = clock();
    double elapsed_time = 0 ;
    Combination* enc_list = malloc(sizeof(Combination)*0x1000000);
    Combination* dec_list = malloc(sizeof(Combination)*0x1000000);
    KeysList* result = init();

    clock_t begin = clock();
    fillLists(enc_list, dec_list, input,sub_keys);
    clock_t end = clock();
    elapsed_time = ((double) (end - begin)) / CLOCKS_PER_SEC;
    printf("Temps pour chiffrer et déchiffrer avec toutes les clés possibles : %f\n",elapsed_time);

    begin = clock();
    quickSort(enc_list, 0x0, 0xffffff);
    quickSort(dec_list, 0x0, 0xffffff);
    end = clock();
    elapsed_time = ((double) (end - begin)) / CLOCKS_PER_SEC;
    printf("Temps pour trier les listes : %f\n",elapsed_time);

    begin = clock();
    for(int i = 0; i <= 0xffffff; i++)
        dichotomous_verification(result, sub_keys, input, enc_list, dec_list[i], 0, 0xffffff);
    end = clock();
    elapsed_time = ((double) (end - begin)) / CLOCKS_PER_SEC;
    printf("Temps pour la recherche dichotomique : %f\n",elapsed_time);
    free(dec_list);
    free(enc_list);

    end = clock();
    elapsed_time = ((double) (end - start)) / CLOCKS_PER_SEC;
    printf("Temps total: %f\n",elapsed_time);

    return result ;
}

/*Take the Keys List and test each combination with the second message and encrypted given.
    If the result does not match c2, proceed to remove the key combination.
    To check the match, use encryption on m2 and k1, take the result and encrypt it with k2.

    Even though we return 1 key, if more exist, they can be found in the KeysList when the removal is finished. If we had a third message + encrypted, they would be used again with this function.
    If our list has no member left, the size would be 0 and thus there's no existing combination.
*/

/*
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
*/
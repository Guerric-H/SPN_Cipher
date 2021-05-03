#include "Decryption.h"
#include "KeyGeneration.h"
#include<stdlib.h>

/*The reverse substitution use a reverse Sbox and operate the same as the classic Substitution.

In order to optimize the code, the Sbox is declared Static and is an array. 
This allow to declare this array only one time and use it everytime instead of declaring and deleting it each time we use this function.

Our message is 24 bits long, so the goal is to use a 6 time ForLoop for every 4 bits (6*4 = 24 bits).
Each time, we shift our result by 4 to the right to insert the new value.
We take the 4 most significant bits that are not changed yet, we shift them to the least significant one and clear the other bits
Then, we make a OR operation on our result and the Sbox value of our last changed 4 bits.

tmp is used to modify freely our original message without impacting the state, which make the code more simple. 
*/
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

/*The reverse permutation use a reverse permutation table and operate the same as the classic Permutation.

In order to optimize the code, the permutation table is declared Static and is an array. 
This allow to declare this array only one time and use it everytime instead of declaring and deleting it each time we use this function.

Our message is 24 bits long, so the goal is to use a 24 time ForLoop for every bits.
Each time, we take the i bit (going from least significant to most significant), put it on the least significant position and use & 1 to find if it is a 0 or a 1.
Then, we shift back the result to the position given by our reverse permutation Table and add it to our result.

tmp is used to modify freely our original message without impacting the state, which make the code more simple. 
*/
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

/* Global decryption function

In order to minimize the amount of allocation and free needed, the sub_keys are already malloced and a pointer is given. 
Otherwise, the attack would use a malloc and a free each time we use a decryption.

The SubKeyGeneration is detailed in the KeyGeneration.c file.

This function behave like a mirror compared to encryption, as it does everything the encryption does, but going from the end to the beginning.
First, we use a XOR between our encrypted message and the last generated key.
Then, we loop "backward" from the last keys to the first :
1) reverse permutation
2) reverse substitution
3) encrypted XOR sub_key (from 10th to 1st)
*/
uint32_t decryption (uint32_t encrypted, uint32_t key, uint32_t*  sub_keys){
    
    subKeyGeneration(key, sub_keys);

    encrypted = encrypted ^ sub_keys[10];
    for (int i = 9; i >= 0 ; i--) {
        encrypted = revPermutation(encrypted);
        encrypted = revSubstitution(encrypted);
        encrypted = encrypted ^ sub_keys[i];
    }
    return encrypted ;
}
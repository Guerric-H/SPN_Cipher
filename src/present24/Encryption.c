#include "Encryption.h"
#include "KeyGeneration.h"
#include<stdlib.h>

/*The substitution use a pre-defined Sbox.

In order to optimize the code, the Sbox is declared Static and is an array. 
This allow to declare this array only one time and use it everytime instead of declaring and deleting it each time we use this function.

Our message is 24 bits long, so the goal is to use a 6 time ForLoop for every 4 bits (6*4 = 24 bits).
Each time, we shift our result by 4 to the right to insert the new value.
We take the 4 most significant bits that are not changed yet, we shift them to the least significant one and clear the other bits
Then, we make a OR operation on our result and the Sbox value of our last changed 4 bits.

tmp is used to modify freely our original message without impacting the state, which make the code more simple. 
*/
uint32_t substitution(uint32_t state) {
    uint16_t static sbox[16] = {12, 5, 6, 11, 9, 0, 10, 13, 3, 14, 15, 8, 4, 7, 1, 2};

    uint32_t res = 0, tmp = 0;
    for (uint32_t i = 0; i < 6; i++) {
        res = res << 4;
        tmp = (state >> (5 - i) * 4) & 0xf ;
        res |= sbox[tmp]; 
    }
    return res;
}

/*The  permutation use a pre-defined permutation table.

In order to optimize the code, the permutation table is declared Static and is an array. 
This allow to declare this array only one time and use it everytime instead of declaring and deleting it each time we use this function.

Our message is 24 bits long, so the goal is to use a 24 time ForLoop for every bits.
Each time, we take the i bit (going from least significant to most significant), put it on the least significant position and use & 1 to find if it is a 0 or a 1.
Then, we shift back the result to the position given by our permutation Table and add it to our result.

tmp is used to modify freely our original message without impacting the state, which make the code more simple. 
*/
uint32_t permutation(uint32_t state) {
    uint32_t static p_table[24] = {0, 6, 12, 18, 1, 7, 13, 19, 2, 8, 14, 20, 
                                   3, 9, 15, 21, 4, 10, 16, 22, 5, 11, 17, 23} ;
    uint32_t res = 0, tmp = 0;
    for (uint32_t i = 0; i < 24; i++) {
        tmp = ((state >> i) & 1);
        res |= (tmp << p_table[i]);
    }
    return res;
}

/* Global encryption function

In order to minimize the amount of allocation and free needed, the sub_keys are already malloced and a pointer is given. 
Otherwise, the attack would use a malloc and a free each time we use an encryption.

The SubKeyGeneration is detailed in the KeyGeneration.c file.

We loop 10 times from the first key to the last :
1) message XOR sub_key (from 1st to 10th, 1 by loop)
2) substitution
3) permutation
Finally, we use a XOR between our encrypted message and the last generated key.
*/
uint32_t encryption(uint32_t message, uint32_t key, uint32_t* sub_keys){
   
    subKeyGeneration(key, sub_keys);
    
    for (int i = 0; i < 10 ; i++) {
        message = message ^ sub_keys[i];
        message = substitution(message);
        message = permutation(message);
    }
    message = message ^ sub_keys[10];
    return message;
}
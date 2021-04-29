#include "stdlib.h"
#include "Encryption.h"
//#include "Decryption.h"
//#include "Attack.h"

int main (int argc, char **argv){

    Encrypt test;
    test.message = 0xf955b9;

    /*uint32_t clair = 5592405;
    uint32_t chiffré = Substitution(clair);
    printf("%u\n",chiffré);
    chiffré = Permutation(clair);
    printf("%u\n",chiffré);*/

    test.subKeys = malloc(sizeof(uint32_t) * 11);
    test.subKeys = SubKeyGeneration(0xd1bd2d, test.subKeys);

    test.encrypted = Encryption(test.message, test.subKeys);
    
    return 0;
}

// 010101010101010101010101
// 000000111111000000111111
/* for (int i = 0; i < 11; i++)
        printf ("%06x\n", kikis[i]); */
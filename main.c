#include <stdlib.h>
#include "Encryption.h"
#include "Decryption.h"
#include "Attack.h"

int main (int argc, char **argv){

    Encrypt enc;
    Decrypt dec;
    enc.message = 0xf955b9;
    
    enc.keys = malloc(sizeof(uint32_t) * 11);
    dec.keys = enc.keys = subKeyGeneration(0xd1bd2d, enc.keys);
    
    enc.encrypted = dec.encrypted = encryption(enc.message, enc.keys);
    dec.message = decryption(dec.encrypted, dec.keys);

    printf("Message clair choisi : %x\n", enc.message);
    printf("Chiffrement du message clair : %x\n\n", enc.encrypted);
    printf("Message chiffré choisi : %x\n", dec.encrypted);
    printf("Déchiffrement du message chiffré : %x\n", dec.message);

    List* list = malloc(sizeof(List)*20);
    for (int i=0; i<20; i++) {
        list[i].message = 0x444444 - (((i*3) + 7)%13);
        list[i].key = 0x0;
        printf ("i = %d -> %x\n", i, list[i].message);
    }
    printf("\n");
    list = quickSort(list, 0, 19);
    for (int i=0; i<20; i++)
        printf ("i = %d -> %x\n", i, list[i].message);

    return 0;

    /* uint32_t m1 = 0x5530a0;
    uint32_t c1 = 0x156a97;
    
     int count = attack(m1,0,c1,0);
    printf("%d\n",count); */
}
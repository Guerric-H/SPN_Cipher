#include <stdlib.h>
#include "Encryption.h"
#include "Decryption.h"
#include "Attack.h"

int main (int argc, char **argv){

    Encrypt test;
    Decrypt test_bis;
    
    uint32_t m1 = 0x5530a0;
    uint32_t c1 = 0x156a97;
    
    int count = attack(m1,0,c1,0);
    printf("%d\n",count);
    /*
    //test.keys = malloc(sizeof(uint32_t) * 11);
    //test_bis.keys = test.keys = subKeyGeneration(0xd1bd2d, test.keys);
    
    //test.encrypted = test_bis.encrypted = encryption(test.message, test.keys);
    //test_bis.message = decryption(test_bis.encrypted, test_bis.keys);

    printf("Message clair choisi : %x\n", test.message);
    printf("Chiffrement du message clair : %x\n\n", test.encrypted);
    printf("Message chiffré choisi : %x\n", test_bis.encrypted);
    printf("Déchiffrement du message chiffr : %x\n", test.message);
    */
    return 0;
}
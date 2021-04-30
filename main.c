#include <stdlib.h>
#include <pthread.h>
#include "Encryption.h"
#include "Decryption.h"
#include "Attack.h"


int main (int argc, char **argv){

    attack(0x5530a0, 0xbf2eef, 0x156a97, 0x73f787);
    attack(0xae4def,0x832331,0xd218a6,0x9200d5);
    return 0;
}
    //attack(0x5530a0, 0xbf2eef, 0x156a97, 0x73f787);
    //attack(0xae4def,0x832331,0xd218a6,0x9200d5);
    

    /*
    KeysList* candidate = init();

    for(int i = 0 ; i < 10 ; i++)
    {
        insert(candidate,i,i);
    }
    
    //remove_element(candidate,(candidate->first->next->next));
    
    //printf("%d %d\n",candidate->first->k1, candidate->first->k2);
    insert(candidate,10,10);
    CandidateKeys* current = candidate->first;
    //remove_element(candidate,current);
    for(int i = 0 ; i < 10 ; i++)
    {
        if(current){
            printf("%d   %d\n",current->k1,current->k2);
            
            current = current->next;
        }
    }
    */

    /*Encrypt enc;
    Decrypt dec;
    enc.message = 0xf955b9;

    enc.keys = malloc(sizeof(uint32_t) * 11);
    dec.keys = enc.keys = subKeyGeneration(0xd1bd2d, enc.keys);

    enc.encrypted = dec.encrypted = encryption(enc.message, enc.keys);
    dec.message = decryption(dec.encrypted, dec.keys);

    printf("Message clair choisi : %x\n", enc.message);
    printf("Chiffrement du message clair : %x\n\n", enc.encrypted);
    printf("Message chiffré choisi : %x\n", dec.encrypted);
    printf("Déchiffrement du message chiffré : %x\n", dec.message);*/

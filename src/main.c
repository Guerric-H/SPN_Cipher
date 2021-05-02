
#include<stdio.h>
#include <stdlib.h>
#include <string.h>

#include"Interface.h"
#include "Encryption.h"
#include "Decryption.h"
#include "Attack.h"


int main (int argc, char **argv){
    
    int continue_prog = 1;
    puts("\nVeuillez choisir ce que vous souhaitez faire en rentrant le caractère associé.\n"
        "Notes: \n1) Seul le premier caractère est considéré même si plusieurs sont insérés.\n"
        "2) Les valeurs sont en héxadécimal de valeur maximum 2^24 (6 unité héxadécimales), utiliser 0x est optionnel.");
    
    while(continue_prog){
        continue_prog = userInterface();
    }
    //attack(0x5530a0, 0xbf2eef, 0x156a97, 0x73f787);
    //attack(0xae4def,0x832331,0xd218a6,0x9200d5);

    //CandidateKeys result = attack(0xef333a, 0xfb432f, 0x250324, 0xa71982);
    //printf("k1 = %x     k2 = %x\n",result.k1,result.k2);
    return 0;
}
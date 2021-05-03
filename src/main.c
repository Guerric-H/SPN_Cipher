
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
    return 0;
}
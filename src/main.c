#include <string.h>
#include "Interface.h"

int main(int argc, char **argv)
{
    int continue_prog = 1;
    puts("\nVeuillez choisir ce que vous souhaitez faire en insérant le caractère associé.\n"
         "Notes: \n1) Seul le premier caractère est considéré même si plusieurs sont insérés.\n"
         "2) Les valeurs/messages sont en héxadécimal et inférieures à 2^24 (6 unités héxadécimales), utiliser 0x est optionnel.");

    while (continue_prog)
    {
        continue_prog = userInterface();
    }
    return 0;
}
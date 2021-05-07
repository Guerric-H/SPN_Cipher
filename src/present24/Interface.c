#include "Interface.h"

/*Take a string and convert it in hexadecimal. Handle 0x format
    after conversion, ensure that values are not out of range (2^24).
    return 1 if they are.
    return 0 if everything is ok.
*/
int string_converter(char *str_input, char *str_key, uint32_t *input, uint32_t *key)
{
    *input = (uint32_t)strtoul(str_input, NULL, 16);
    *key = (uint32_t)strtoul(str_key, NULL, 16);

    if (*input > 0xffffff || *key > 0xffffff)
    {
        puts("Les valeurs choisies sont trop grandes.\n");
        return 1;
    }
    return 0;
}

// For each result found, write them on the terminal, if no keys were found, put a pre defined message.
void display_result(KeysList *result)
{
    CandidateKeys *current = result->first;
    if (!result->size)
    {
        puts("Aucune combinaison de clés ne correspond à ces messages et chiffrés.\n");
        return;
    }
    while (current)
    {
        printf("Clé 1 : %06x\t| Clé 2 : %06x\n", current->k1, current->k2);
        current = current->next;
    }
}

/*
    Display Users Choice at disposal, wait for its input.
    Depending on the input, use appropriate case to ask for values, convert them and proceed to call the appropriate function.
    Case 1  : Encryption, wait for 2 values ; message and key
    Case 2  : Decryption, wait for 2 values ; encrypted and key
    Case 3  : Attack, wait for 4 values ; m1, c1 and m2, c2
    Case 4  : Quit the program
    Default : Invalid input.
    If not case 4, ask for an other operation right after.
*/
int userInterface()
{

    int continue_prog = 1;
    char choice = 0;
    char *message_checker;
    char *key_checker;
    Encrypt enc;
    Decrypt dec;
    AttackInput att;
    uint32_t *sub_keys = malloc(sizeof(uint32_t) * 11);

    puts("\nChiffrer\t: 1\nDéchiffrer\t: 2\nAttaquer\t: 3\nQuitter\t\t: q\n");
    scanf(" %c", &choice);

    switch (choice)
    {
    case '1':
        puts("Veuillez choisir le message, puis la clé.\n");

        puts("Message :");
        scanf(" %ms", &message_checker);
        puts("Clé :");
        scanf(" %ms", &key_checker);
        if (!string_converter(message_checker, key_checker, &enc.message, &enc.key))
        {
            uint32_t encrypted = encryption(enc.message, enc.key, sub_keys);
            printf("Chiffré\t: %x\n", encrypted);
        }
        free(message_checker);
        free(key_checker);
        break;

    case '2':
        puts("Veuillez choisir le chiffré, puis la clé.\n");

        puts("Chiffré :");
        scanf(" %ms", &message_checker);
        puts("Clé :");
        scanf(" %ms", &key_checker);

        if (!string_converter(message_checker, key_checker, &dec.encrypted, &dec.key))
        {
            uint32_t message = decryption(dec.encrypted, dec.key, sub_keys);
            printf("Message\t: %x\n", message);
        }
        free(message_checker);
        free(key_checker);
        break;
    case '3':
        puts("Veuillez choisir le message numéro 1, puis le chiffré numéro 1.\n");

        puts("Message 1:");
        scanf(" %ms", &message_checker);
        puts("Chiffré 1:");
        scanf(" %ms", &key_checker);
        int i = string_converter(message_checker, key_checker, &att.m1, &att.c1);

        puts("Veuillez choisir le message numéro 2, puis le chiffré numéro 2.\n");
        puts("Message 2:");
        scanf(" %ms", &message_checker);
        puts("Chiffré 2:");
        scanf(" %ms", &key_checker);
        int j = string_converter(message_checker, key_checker, &att.m2, &att.c2);
        if (!(i || j))
        {
            puts("Veuillez patienter pendant l'attaque ...\n");
            KeysList *result = attack(att, sub_keys);
            display_result(result);
            free(result);
        }
        free(message_checker);
        free(key_checker);
        break;

    case 'q':
        puts("Fin du programme.\n");
        continue_prog = 0;
        break;

    default:
        puts("La valeur choisie ne correspond à aucune des options proposées.\n");
        break;
    }
    return continue_prog;
}
#include "Encryption.h"
//#include "Decryption.h"
//#include "Attack.h"

int main (int argc, char **argv){

    Encrypt test ;
    initEncryption(&test);
    __uint32_t clair = 1;
    __uint32_t chiffré = Substitution(clair);

    printf("%d\n",chiffré);
    
 return 0 ;
}

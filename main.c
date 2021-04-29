#include "stdlib.h"
#include "Encryption.h"
//#include "Decryption.h"
//#include "Attack.h"

int main (int argc, char **argv){

    Encrypt test;
    test.message = 0xf955b9;
    test.subKeys = malloc(sizeof(uint32_t) * 11);
    test.subKeys = SubKeyGeneration(0xd1bd2d, test.subKeys);
    test.encrypted = Encryption(test.message, test.subKeys);
    
    return 0;
}
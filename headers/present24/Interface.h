#pragma once
#include "Attack.h"

//Dissect the line written, handle hexadecimal and return 1 if the value is above 2^24 (more than 6 bits in hexadecimal).
int string_converter(char* str_input, char* str_key, uint32_t* input, uint32_t* key);

//Function that print each key combination result.
void display_result(KeysList* result);

//Global user Interface, use case to behave properly depending on user input.
int userInterface();
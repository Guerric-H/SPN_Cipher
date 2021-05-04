#pragma once
#include <stdint.h>
#include "Attack.h"

int string_converter(char* str_input, char* str_key, uint32_t* input, uint32_t* key);

void display_result(KeysList* result);

int userInterface();
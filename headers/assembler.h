#pragma once

#include <stdio.h>
#include "stack.h"
#include "error.h"

Stack_Err assembler(const char * txt_filename, const char * byte_filename);
Stack_Err parse_line(const char * line, FILE * file_byte);
int * load_bytecode(const char * file_byte, size_t * size);
int check_register(const char * reg_buffer);


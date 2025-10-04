#pragma once

#include <stdio.h>
#include "stack.h"
#include "error_stack.h"

Stack_Err assembler(const char * txt_filename, const char * byte_filename);
Stack_Err parse_line(const char * line, FILE * file_byte);
StackElem * load_bytecode(const char * file_byte, size_t * size);

#pragma once

#include <stdio.h>
#include "stack.h"
#include "error.h"

Stack_Err assembler(const char * txt_filename, const char * byte_filename);
Stack_Err parse_line(const char * line, FILE * file_byte);
int * load_bytecode(const char * file_byte, size_t * size);
void init_labels(void);
Spu_Err parse_argument(const char *arg, int * value);
Spu_Err first_pass(const char * txt_filename);
Spu_Err second_pass(const char *txt_filename);


#define MAX_LABELS 16
#define MAX_CODE_SIZE 1024

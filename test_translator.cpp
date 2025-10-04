#include <stdio.h>
#include <stdlib.h>
#include "stack.h"
#include "error_stack.h"
#include "read_file.h"
#include "assembler.h"
#include "spu.h"


int main(void)
{
    assembler("text/testcommands.txt", "text/byte_code.txt");

    size_t buffer_size = 0;
    StackElem * code = load_bytecode("byte_code.txt", &buffer_size);
    
    run_spu(code, buffer_size);

    printf("Programm is finished\n");
    free(code);
    
    return 0;
}
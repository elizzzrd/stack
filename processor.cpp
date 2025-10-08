#include <stdio.h>
#include <stdlib.h>
#include "stack.h"
#include "errors.h"
#include "read_file.h"
#include "assembler.h"
#include "spu.h"


int main(void)
{
    log_init(LOG_FILENAME);

    spu_t spu = {};
    Spu_Err spu_err = spu_init(&spu);
    if (spu_err != SPU_OK)
    {
        log_message("SPU initialization error\n", __FILE__, __LINE__);
        finish_program(&spu);
        return 1;
    }

    Stack_Err stack_err = assembler("text/testcommands.txt", "text/byte_code.txt");
    if (stack_err != STACK_OK)
    {
        log_message("Assembly error\n", __FILE__, __LINE__);
        finish_program(&spu);
        return 1;
    }

    spu_err |= run_spu(&spu);
    if (spu_err != SPU_OK)
    {
        log_message("Run spu error\n", __FILE__, __LINE__);
        finish_program(&spu);
        return 1;
    }

    finish_program(&spu);
    return 0;
}
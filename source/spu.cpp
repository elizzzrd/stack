#include <stdio.h>
#include <math.h>
#include <assert.h>
#include <string.h>
#include "stack.h"
#include "errors.h"
#include "read_file.h"
#include "assembler.h"
#include "spu.h"


Spu_Err spu_init(spu_t * spu)
{
    Spu_Err errors = SPU_OK;

    if (spu == NULL) 
    {
        errors |= SPU_NULL_PTR;
        return errors;
    }

    spu -> code_size = 0;
    int * code = load_bytecode("byte_code.txt", &(spu -> code_size));

    size_t instructor_ptr = 0;
    spu -> stack = {};
    if((stack_init(&(spu -> stack), 8)) != STACK_OK) errors |= SPU_STACK_ERROR;
    StackElem regs[16] = {};

    return errors;
}


Spu_Err run_spu(spu_t * spu)
{
    assert(spu != NULL);

    Spu_Err spu_errors = SPU_OK;
    Stack_Err stack_errors = STACK_OK;

    while ((spu -> instructor_ptr) < (spu -> code_size))
    {
        int cmd = (spu -> code)[(spu -> instructor_ptr)++];

        switch(cmd) 
        {
            case HLT: (spu -> instructor_ptr) = (spu -> code_size); break;
            case PUSH: 
            {
                if ((spu -> instructor_ptr) >= (spu -> code_size))
                {
                    (spu -> instructor_ptr) = spu -> code_size;
                    spu_errors |= SPU_INVALID_COMMAND;
                    fprintf(log_file, "Error: missing argument for PUSH\n"); // error to log file
                    break;
                }
                int value = (spu -> code)[spu -> instructor_ptr];
                if (is_command(value))
                {
                    spu_errors |= SPU_INVALID_COMMAND;  
                    fprintf(log_file, "Error: invalid argument for PUSH — got another command (%d)\n", value);
                    break; 
                }
                (spu->instructor_ptr)++;
                stack_push(&(spu->stack), value);
                break;
            }
            case POPR:
            {
                int reg_num = (spu -> code)[spu -> instructor_ptr++];
                StackElem a = 0;
                stack_errors = stack_pop(&(spu -> stack), &a);
                spu -> regs[reg_num] = a;
                break;
            }
            case PUSHR:
            {
                int reg_num = (spu -> code)[spu -> instructor_ptr++];
                stack_errors = stack_push(&(spu -> stack), spu -> regs[reg_num]);
                break;
            }
            case DUMP: stack_dump(&(spu -> stack), stack_errors, __FILE__, __LINE__); break;
            case ADD: 
            {
                StackElem a = 0, b = 0;
                stack_errors = stack_pop(&(spu -> stack), &a);
                stack_errors = stack_pop(&(spu -> stack), &b);
                stack_errors = stack_push(&(spu -> stack), a + b);

                break;
            }
            case SUB:
            {
                StackElem a = 0, b = 0;
                stack_errors = stack_pop(&(spu -> stack), &a);
                stack_errors = stack_pop(&(spu -> stack), &b);
                stack_errors = stack_push(&(spu -> stack), a - b);

                break;                    
            }
            case MUL:
            {
                StackElem a = 0, b = 0;
                stack_errors = stack_pop(&(spu -> stack), &a);
                stack_errors = stack_pop(&(spu -> stack), &b);
                stack_errors = stack_push(&(spu -> stack), a * b);

                break;    
            }     
            case DIV:
            {
                StackElem a = 0, b = 0;
                stack_errors = stack_pop(&(spu -> stack), &a);
                stack_errors = stack_pop(&(spu -> stack), &b);
                if (b == 0)
                {
                    log_message("Error: division by zero\n", __FILE__, __LINE__); 
                    spu_errors |= SPU_DIVISION_BY_ZERO;
                    stack_push(&(spu -> stack), a);
                    stack_push(&(spu -> stack), b);
                }
                stack_errors = stack_push(&(spu -> stack), a / b);

                break;
            }       
            case SQRT:
            {
                StackElem a = 0;
                stack_errors = stack_pop(&(spu -> stack), &a);
                if (a < 0) 
                {
                    spu_errors |= SPU_INVALID_COMMAND;
                    log_message("Error: sqrt of negative number\n", __FILE__, __LINE__); 
                    stack_push(&(spu -> stack), a);
                }
                else
                {
                    stack_errors = stack_push(&(spu -> stack), (StackElem) sqrt(a));
                }
                break;
            }    
            case OUT:
            {
                StackElem a = 0;
                stack_errors = stack_pop(&(spu -> stack), &a);
                printf("%d\n", a);
                break;
            }
            default: log_message("Unknown commmand\n", __FILE__, __LINE__); break; 
        }

        if (stack_errors != STACK_OK) 
        {
            spu_errors |= SPU_STACK_ERROR;
            break;
        }
    }
    
    printf("Programm is finished!\n");
    return spu_errors;
}


bool is_command(int code)
{
    return (code >= PUSH && code <= HLT);
}


Spu_Err spu_destroy(spu_t * spu)
{
    assert(spu != NULL);
    Spu_Err errors = SPU_OK;

    free(spu -> code);
    spu -> code = NULL;
    spu -> code_size = 0;
    spu -> instructor_ptr = 0;
    if ((stack_destroy(&(spu -> stack))) != STACK_OK) errors |= SPU_STACK_ERROR;
    memset(spu->regs, 0, sizeof(spu->regs)); // нормально ли так очищать значения из статического массива

    return SPU_OK; 
}
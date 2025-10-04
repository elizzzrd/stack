#include <stdio.h>
#include <math.h>
#include <assert.h>
#include "stack.h"
#include "error_stack.h"
#include "read_file.h"


Stack_Err run_spu(StackElem * code, size_t size)
{
    assert(code != NULL);

    stack_t stack = {};
    Stack_Err err = stack_init(&stack, 8);

    int instructor_ptr = 0;

    while (instructor_ptr < size)
    {
        int cmd = code[instructor_ptr++];

        switch(cmd) 
        {
            case HLT: instructor_ptr = size; break;
            case PUSH: 
            {
                if (instructor_ptr >= size)
                {
                    instructor_ptr = size;
                    fprintf(stderr, "Need value for push\n");
                    break;
                }
                stack_push(&stack, code[instructor_ptr++]);
                break;
            }
            case DUMP: stack_dump(&stack, err, __FILE__, __LINE__); break;
            case ADD: 
            {
                StackElem a = 0, b = 0;
                err = stack_pop(&stack, &a);
                err = stack_pop(&stack, &b);
                err = stack_push(&stack, a + b);

                break;
            }
            case SUB:
            {
                StackElem a = 0, b = 0;
                err = stack_pop(&stack, &a);
                err = stack_pop(&stack, &b);
                err = stack_push(&stack, a - b);

                break;                    
            }
            case MUL:
            {
                StackElem a = 0, b = 0;
                err = stack_pop(&stack, &a);
                err = stack_pop(&stack, &b);
                err = stack_push(&stack, a * b);

                break;    
            }     
            case DIV:
            {
                StackElem a = 0, b = 0;
                err = stack_pop(&stack, &a);
                err = stack_pop(&stack, &b);
                if (b == 0)
                {
                    printf("Error: division by zero\n");
                    stack_push(&stack, a);
                    stack_push(&stack, b);
                }
                err = stack_push(&stack, a / b);

                break;
            }       
            case SQRT:
            {
                StackElem a = 0;
                err = stack_pop(&stack, &a);
                if (a < 0) 
                {
                    printf("Error: sqrt of negative number\n");
                    stack_push(&stack, a);
                }
                else
                {
                    err = stack_push(&stack, (StackElem) sqrt(a));
                }
                break;
            }    
            case OUT:
            {
                StackElem a = 0;
                err = stack_pop(&stack, &a);
                printf("%d\n", a);
                break;
            }
            default: printf("Unknown commmand\n"); break;
        }
    }
    
    printf("Programm is finished!\n");
    stack_destroy(&stack);
    return STACK_OK;
}
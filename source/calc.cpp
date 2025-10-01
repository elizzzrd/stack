#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <math.h>
#include "stack.h"
#include "error_stack.h"

#define NMAX 10
#define NCOMMANDS 9

typedef enum 
{
    PUSH, DUMP, ADD, SUB, MUL,
    DIV, SQRT, OUT, HLT
} Stack_commands;


void to_upper_str(const char * input, char * output)
{
    int i = 0;
    for (i = 0; input[i] != '\0'; i++)
    {
        output[i] = (char)toupper((unsigned char) input[i]);
    }
    output[i] = '\0';
    return;
}

void skip_line(void) 
{
    while (getchar() != '\n') continue; 
}

static const char * stack_commands[] = 
{
    "PUSH", "DUMP", "ADD", "SUB", "MUL", 
    "DIV", "SQRT","OUT", "HLT"
};

int check_option(char * option)
{
    char option_upper[NMAX] = {};
    to_upper_str(option, option_upper);
    for (int i = 0; i < NCOMMANDS; i++)
    {
        if (strcmp(option_upper, stack_commands[i]) == 0) return i;
    }
    return -1;
}

// typedef Stack_Err (* stack_function_t)(stack_t * stack);

// stack_function_t operators[] = 
// {
//     push, dump, add, sub, mul, div, sqrt, out, hlt
// };

// Stack_Err push(stack_t * stack)
// {

// }


int main(void)
{
    stack_t stack = {};
    Stack_Err err = stack_init(&stack, 8);

    char calc_option[NMAX] = {};
    int value = 0;
    int index = 0;

    printf("\n===========Stack calculator===========\n" 
           "Enter commands (push N, dump, add, sub, mul, div, sqrt, out, hlt)\n");


    while (scanf("%s", calc_option) == 1)
    {
        if ((index = check_option(calc_option)) == -1) 
        {
            printf("Unknown command. Please try again\n");
            skip_line();
            continue;
        }
        
        if (index == PUSH)
        {
            if (scanf("%d", &value) != 1)
            {
                printf("Error: push requires an integer\n");
                skip_line();
                continue;
            }
            stack_push(&stack, value);
        }
        else if (index == HLT) break;
        else
        {
            switch(index) 
            {
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
    }
    printf("Programm is finished!\n");
    stack_destroy(&stack);
    return 0;
}
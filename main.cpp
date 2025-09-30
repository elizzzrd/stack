#include <stdio.h>
#include "stack.h"
#include "error.h"

int main() 
{
    stack_t stack = {};
    Stack_Err err = stack_init(&stack, 5);   

    if (err != STACK_OK) 
    {
        stack_dump(&stack, err, __FILE__, __LINE__);
        return 1;
    }


    for (int i = 1; i < 4; i++)
    {
        err = stack_push(&stack, i * 10);
        if (err != STACK_OK) 
        {
            stack_dump(&stack, err, __FILE__, __LINE__);
            return 1;
        }
    }

    printf("Elements added to stack!\n");

    StackElem value = 0;
    while ((err = stack_pop(&stack, &value)) == STACK_OK) 
    {
        printf("Taken value: %d\n", value);
    }
    

    stack_destroy(&stack);
    return 0;
}

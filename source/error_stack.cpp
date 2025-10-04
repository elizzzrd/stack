#include <stdio.h>
#include "stack.h"
#include "error_stack.h"


Stack_Err stack_verify(const stack_t * stack) 
{
    if (stack == NULL)         return STACK_NULL_PTR;
    if (stack -> data == NULL) return STACK_UNINITIALIZED;

    //if ((stack -> capacity == 0)) return STACK_INVALID_CAPACITY;
    //if (stack -> size > stack->capacity) return STACK_OVERFLOW;

    if (stack -> left_canary != CANARY_LEFT_VALUE ||
        stack -> right_canary != CANARY_RIGHT_VALUE) return STACK_CANARY_CORRUPTED;
    if (stack->data[-1] != CANARY_LEFT_VALUE || stack->data[stack->capacity] != CANARY_RIGHT_VALUE)
        return STACK_CANARY_CORRUPTED;

    return STACK_OK;
}


const char * stack_error_string(Stack_Err error) 
{
    static const char * error_strings[] = 
    {
        "Success",
        "Memory allocation failed",
        "Null pointer provided", 
        "Stack is uninitialized",
        "Stack overflow",
        "Stack underflow",
        "Invalid capacity",
        "Invalid size",
        "Stack canary corrupted"
    };
    
    if (error < STACK_OK || error > STACK_FILE_ERROR) return "Unknown error";
    
    return error_strings[error];
}



void stack_dump(const stack_t * stack, Stack_Err error, const char * file, int line)
{
    fprintf(stderr, "============STACK DUMP============\n");
    fprintf(stderr, "\nstack_dump called from %s : %d\n", file, line);
    fprintf(stderr, "Error: %s (%d)\n", stack_error_string(error), error);

    if (stack == NULL) 
    {
        fprintf(stderr, "Stack pointer is NULL\n");
        return;
    }
    
    fprintf(stderr, "Stack [%p]\n", stack);
    fprintf(stderr, "{\n\tSize = %zu\n", stack->size);
    fprintf(stderr, "\tCapacity = %zu\n}\n", stack->capacity);
    fprintf(stderr, "data [%p]\n", (void*)stack->data);
    fprintf(stderr, "{\n");

    if (stack->data == NULL) 
    {
        fprintf(stderr, "Data array is NULL\n");
        return;
    }
    
    if (stack->size == 0) 
    {
        fprintf(stderr, "\t[empty]\n}\n");
    } 
    else 
    {
        size_t i = 0;
        for (i = 0; i < stack -> size; i++)
        {
            fprintf(stderr, "\t*[%zu] = %d\n", i, stack -> data[i]);
        }
        for (; i < stack->capacity; i++)
        {
            fprintf(stderr, "\t [%zu] = RUBBISH\n", i);
        }
        fprintf(stderr, "}\n");
    }
    fprintf(stderr, "============END STACK DUMP============\n");
}


#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include "stack.h"
#include "error.h"
 

Stack_Err stack_init(stack_t * stack, int capacity)
{
    if (stack == NULL) return STACK_NULL_PTR;
    if (capacity == 0) return STACK_INVALID_CAPACITY;
    
    stack -> data = (StackElem *) calloc((size_t) capacity, sizeof(StackElem));
    if (stack->data == NULL) return STACK_MEMORY_ALLOCATION_ERROR;

    stack -> size = 0;
    stack -> capacity = capacity;

    return STACK_OK;
}


Stack_Err stack_destroy(stack_t * stack)
{
    if (stack == NULL) return STACK_NULL_PTR;
    
    free(stack->data);
    stack -> data = NULL;
    stack -> size = 0;
    stack -> capacity = 0;
    
    return STACK_OK;
}

Stack_Err stack_push(stack_t * stack, StackElem value)
{
    STACK_CHECK(stack);
    
    stack -> data[stack -> size++] = value;
    
    STACK_CHECK(stack);
    return STACK_OK;
}

Stack_Err stack_pop(stack_t * stack, StackElem * value)
{
    if (value == NULL) return STACK_NULL_PTR;
    if (stack -> size == 0) return STACK_UNDERFLOW;
    STACK_CHECK(stack);
    
    *value = stack -> data[--(stack -> size)];
    
    STACK_CHECK(stack);
    return STACK_OK;
}

void print_stack(const stack_t * stack) 
{
    if (stack == NULL) 
    {
        printf("Stack pointer is NULL\n");
        return;
    }

    printf("Stack (size = %d, capacity = %d): ", stack->size, stack->capacity);

    if (stack->size == 0) 
    {
        printf("[empty]\n");
        return;
    }

    for (int i = 0; i < stack->size; i++) 
    {
        printf("[%d] %d ", i, stack->data[i]);
    }
    printf("\n");
}
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include "stack.h"
#include "error_stack.h"
 

Stack_Err stack_init(stack_t * stack, size_t capacity)
{
    if (stack == NULL) return STACK_NULL_PTR;
    if (capacity <= 0) return STACK_INVALID_CAPACITY;
    
    stack -> left_canary = CANARY_LEFT_VALUE;
    stack -> right_canary = CANARY_RIGHT_VALUE;

    stack -> data = (StackElem *) calloc((size_t) capacity + 2, sizeof(StackElem));
    if (stack -> data == NULL) return STACK_MEMORY_ALLOCATION_ERROR;

    stack -> data[0] = CANARY_LEFT_VALUE;                           
    stack -> data[capacity+1] = CANARY_RIGHT_VALUE;

    stack -> data = stack->data + 1;

    stack -> size = 0;
    stack -> capacity = capacity;

    return STACK_OK;
}


Stack_Err stack_destroy(stack_t * stack)
{
    if (stack == NULL) return STACK_NULL_PTR;
    
    free(stack->data - 1);
    stack -> data = NULL;
    stack -> size = 0;
    stack -> capacity = 0;
    stack -> left_canary = 0;
    stack -> right_canary = 0;

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
    if (stack == NULL) return STACK_NULL_PTR;
    if (value == NULL) return STACK_NULL_PTR;
    if (stack -> size == 0) return STACK_UNDERFLOW;
    STACK_CHECK(stack);
    
    *value = stack -> data[--(stack -> size)];
    
    STACK_CHECK(stack);
    return STACK_OK;
}


bool stack_is_empty(const stack_t * stack) 
{
    if (stack == NULL) return true;
    return stack->size == 0;
}

bool stack_is_full(const stack_t * stack) 
{
    if (stack == NULL) return false;
    return stack->size >= stack->capacity;
}



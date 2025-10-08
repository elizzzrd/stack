#include <stdio.h>
#include <stdbool.h>
#include <assert.h>
#include <stdlib.h>
#include "stack.h"
#include "errors.h"
 
// нужно ли делать реализацию наподобие getline, если передан capasity =< 0, то функция сама его увеличит до валидного значения 
Stack_Err stack_init(stack_t * stack, size_t capacity)
{
    Stack_Err errors = STACK_OK;
    if (stack == NULL) 
    {
        errors |= STACK_NULL_PTR;
        return errors;
    }
        
    stack -> left_canary = CANARY_LEFT_VALUE;
    stack -> right_canary = CANARY_RIGHT_VALUE;

    stack -> data = (StackElem *) calloc((size_t) capacity + 2, sizeof(StackElem));
    if (stack -> data == NULL) errors |= STACK_MEMORY_ALLOCATION_ERROR;

    stack -> data[0] = CANARY_LEFT_VALUE;                           
    stack -> data[capacity+1] = CANARY_RIGHT_VALUE;

    stack -> data = stack->data + 1;

    stack -> size = 0;
    stack -> capacity = capacity;

    return errors;
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
    Stack_Err errors = STACK_OK;
    STACK_CHECK(stack);
    
    if ((stack -> size) >= (stack -> capacity))
    {
        size_t new_capacity = (stack -> capacity) * 2;
        errors |= stack_resize(stack, new_capacity);
    }

    stack -> data[stack -> size++] = value;
    
    STACK_CHECK(stack);
    return errors;
}


Stack_Err stack_pop(stack_t * stack, StackElem * value)
{
    Stack_Err errors = STACK_OK;
    if (value == NULL) return STACK_NULL_PTR;
    STACK_CHECK(stack);
    if (stack -> size == 0) errors |= STACK_UNDERFLOW;

    *value = stack -> data[--(stack -> size)];
    
    if (stack -> size > 0 && stack -> size < (stack -> capacity / 4))
    {
        size_t new_capacity = (stack -> capacity) / 2;
        errors |= stack_resize(stack, new_capacity);
    }

    STACK_CHECK(stack);
    return errors;
}


Stack_Err stack_resize(stack_t * stack, size_t new_capacity)
{
    if (stack == NULL) return STACK_NULL_PTR;

    if (new_capacity == 0) (new_capacity) = 1;
    if (new_capacity < 4) new_capacity = 4; 

    StackElem * old_ptr = (stack -> data) - 1;
    StackElem * new_ptr = (StackElem *) realloc(old_ptr, (new_capacity + 2) * sizeof(StackElem));
    if (!new_ptr) return STACK_MEMORY_ALLOCATION_ERROR;
    else
    {
        new_ptr[0] = CANARY_LEFT_VALUE;
        new_ptr[new_capacity + 1] = CANARY_RIGHT_VALUE;

        stack -> data = new_ptr + 1;
        stack -> capacity = new_capacity;
    }
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
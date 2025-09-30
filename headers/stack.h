#pragma once
#include <stdbool.h>

typedef int StackElem;


typedef struct 
{
    StackElem left_canary;
    StackElem * data;
    size_t size;
    size_t capacity;
    StackElem right_canary;
} stack_t;

typedef enum 
{
    STACK_OK = 0,

    STACK_MEMORY_ALLOCATION_ERROR,
    STACK_NULL_PTR,
    STACK_UNINITIALIZED,

    STACK_OVERFLOW,
    STACK_UNDERFLOW,

    STACK_INVALID_CAPACITY,
    STACK_INVALID_SIZE,

    STACK_CANARY_CORRUPTED
} Stack_Err; 

Stack_Err stack_init(stack_t * stack, size_t capacity);
Stack_Err stack_destroy(stack_t * stack);
Stack_Err stack_push(stack_t * stack, StackElem value);
Stack_Err stack_pop(stack_t * stack, StackElem * value);
bool stack_is_empty(const stack_t * stack);
bool stack_is_full(const stack_t * stack);

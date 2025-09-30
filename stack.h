#pragma once
#define SIZE_MAX 100
typedef int StackElem;

typedef struct 
{
    StackElem * data;
    int size;
    int capacity;
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

} Stack_Err;  


Stack_Err stack_init(stack_t * stack, int capacity);
Stack_Err stack_destroy(stack_t * stack);
Stack_Err stack_push(stack_t * stack, StackElem value);
Stack_Err stack_pop(stack_t * stack, StackElem * value);
void print_stack(const stack_t * stack);
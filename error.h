#pragma once
#include "stack.h"


#define STACK_CHECK(stack_ptr) \
    do { \
        Stack_Err err = stack_verify(stack_ptr); \
        if (err != STACK_OK) { \
            stack_dump(stack_ptr, err, __FILE__, __LINE__);  \
            return err; \
        } \
    } while (0)



void stack_dump(const stack_t * stack, Stack_Err error, const char * file, int line);
Stack_Err stack_verify(const stack_t * stack); 
const char* stack_error_string(Stack_Err error);
#include <stdio.h>
#include "stack.h"
#include "error_stack.h"


void print_stack(const stack_t *stack) 
{
    if (!stack) { printf("Stack = NULL\n"); return; }
    printf("Stack (size=%zu, capacity=%zu): ", stack->size, stack->capacity);
    for (size_t i = 0; i < stack->size; i++) printf("%d ", stack->data[i]);
    printf("\n");
}


void test_stack() 
{
    stack_t st = {};
    Stack_Err err = stack_init(&st, 2);

    if (err != STACK_OK) 
    {
        stack_dump(&st, err, __FILE__, __LINE__);
        return;
    }
    /*
    printf("\n=== TEST: push ===\n");
    for (int i = 1; i <= 3; i++) {
        err = stack_push(&st, i * 10);
        if (err != STACK_OK) stack_dump(&st, err, __FILE__, __LINE__);
    }
    print_stack(&st);
    
    // printf("\n=== TEST: overflow ===\n");
    // err = stack_push(&st, 999);
    // if (err != STACK_OK) stack_dump(&st, err, __FILE__, __LINE__);
    // printf("Expected overflow error: %d\n", err);
    // print_stack(&st);

    printf("\n=== TEST: pop ===\n");
    StackElem value;
    while ((err = stack_pop(&st, &value)) == STACK_OK) 
    {
        printf("pop: %d\n", value);
        print_stack(&st);
    }

    printf("\n=== TEST: underflow ===\n");
    err = stack_pop(&st, &value);
    if (err != STACK_OK) stack_dump(&st, err, __FILE__, __LINE__);
    printf("Expected error underflow: %d\n", err);
    print_stack(&st);

    printf("\n=== TEST: canary defence ===\n");
    st.data[-1] = 123;
    err = stack_verify(&st);
    if (err == STACK_CANARY_CORRUPTED) {
        printf("!!! Canary corrupted, defence succeed !!!\n");
    } else {
        printf("Error: stack_verify has not detected anything\n");
    }
    */
    
    printf("=== Заполнение и увеличение ===\n");
    for (int i = 0; i < 10; i++) {
        stack_push(&st, i);
        printf("push(%d), size=%zu, capacity=%zu\n", i, st.size, st.capacity);
    }

    printf("=== Проверим данные ===\n");
    for (size_t i = 0; i < st.size; i++) {
        printf("[%zu] = %d\n", i, st.data[i]);
    }

    stack_destroy(&st);
}


int main() 
{
    test_stack();
    return 0;
}

#include "stack.h"

bool
stack_is_empty (struct stack * stk) {
    return !stk;
}

size_t
stack_size (struct stack * stk) {

    size_t size = 0;

    for ( struct stack * ptr = stk; ptr; ptr = ptr->next ) {
        ++size;
    }

    return size;
}

struct stack *
stack_pop (struct stack ** stk) {

    if ( !stk ) { return NULL; }

    struct stack * old_head = *stk;

    *stk = *stk ? (*stk)->next : NULL;

    return old_head;
}

bool
stack_push (struct stack ** stk, struct stack new_head) {

    if ( !stk ) { return NULL; }

    size_t sz = sizeof(struct stack);
    struct stack * new = malloc(sz);
    if ( !new ) {
        return false;
    }

    memcpy(new, &new_head, sz);

    new->next = *stk;
    *stk = new;

    return !!*stk;
}

signed long
stack_search (struct stack * stk, signed value) {

    if ( !stk ) { return -1; }

    long index = 0;
    for ( struct stack * ptr = stk; ptr; ptr = ptr->next ) {
        ++index;
        if ( ptr->value == value ) { break; }
        if ( !ptr->next ) {
            index = 0;
        }
    }

    return index - 1;
}

signed
stack_peak (struct stack * stk, long idx) {

    if ( (size_t )idx > stack_size(stk) ) {
        return 0;
    }

    long index = 0;
    for ( struct stack * ptr = stk; ptr; ptr = ptr->next ) {
        if ( index++ == idx ) {
            return ptr->value;
        }
    }

    return 0;
}

void
stack_print (struct stack * stk) {

    for ( struct stack * ptr = stk; ptr; ptr = ptr->next ) {
        printf("%c ", ptr->value);
    }

    putchar('\n');
}

void
stack_free (struct stack * stk) {

    if ( !stk ) { return; }

    struct stack * next = stk;
    while ( next ) {
        if ( next->next ) {
            struct stack * n = next->next;
            free(next);

            next = n;
            continue;
        }

        free(next);
        return;
    }
}


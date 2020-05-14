#ifndef STACK_H
#define STACK_H

#pragma once

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

struct stack {
    signed value;
    struct stack * next;
};

#define STACK_INIT(name) struct stack * name = NULL

bool
stack_is_empty (struct stack *);

size_t
stack_size (struct stack *);

struct stack *
stack_pop (struct stack **);

bool
stack_push (struct stack **, struct stack);

#define STACK_PUSH(stk, val) stack_push(&(stk), (struct stack){ .value = (val) })

long
stack_search (struct stack *, signed);

void
stack_print (struct stack *);

void
stack_free (struct stack *);

#endif


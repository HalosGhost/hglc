#ifndef MAIN_H
#define MAIN_H

#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <pcq.h>

#include <readline/readline.h>
#include <readline/history.h>

#define DE_BRUIJN_INDEX_NOTATION \
    X(DE_BRUIJN_Lambda, "lambda", "'\\\\' | \"λ\"") \
    X(DE_BRUIJN_Identifier, "id", "/[0-9]+/") \
    X(DE_BRUIJN_empty_space, "empty", "\"\"") \
    X(DE_BRUIJN_Abstraction, "abs", "<lambda> <term>") \
    X(DE_BRUIJN_Atom, "atom", "'(' <term> ')' | <id>") \
    X(DE_BRUIJN_Application, "app", "<atom> <appL>") \
    X(DE_BRUIJN_Term, "term", "<app> | <abs>") \
    X(DE_BRUIJN_ApplicationL, "appL", "<atom> <appL> | <empty>") \
    X(DE_BRUIJN_Program, "program", "/^/ <abs> /$/")

#define CHURCH_NOTATION \
    X(CHURCH_Lambda, "lambda", "'\\\\' | \"λ\"") \
    X(CHURCH_Dot, "dot", "'.'") \
    X(CHURCH_Identifier, "id", "/[a-z]/") \
    X(CHURCH_empty_space, "empty", "\"\"") \
    X(CHURCH_Abstraction, "abs", "<lambda> <id> <dot> <term>") \
    X(CHURCH_Atom, "atom", "'(' <term> ')' | <id>") \
    X(CHURCH_Application, "app", "<atom> <appL>") \
    X(CHURCH_Term, "term", "<app> | <abs>") \
    X(CHURCH_ApplicationL, "appL", "<atom> <appL> | <empty>") \
    X(CHURCH_Program, "program", "/^/ <abs> /$/")

#define X(id, name, def) + 1
static const int PARSER_COUNT =
    CHURCH_NOTATION
    DE_BRUIJN_INDEX_NOTATION
;
#undef X

void
de_bruijn (pcq_ast_t *);

void
ast_remove_child (pcq_ast_t *, signed);

#endif

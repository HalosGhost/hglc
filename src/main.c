#include <stdio.h>
#include <stdlib.h>
#include <pcq.h>

#include <readline/readline.h>
#include <readline/history.h>

#define DE_BRUIJN_INDEX_NOTATION \
    X(Lambda, "lambda", "'\\\\' | \"λ\"") \
    X(Identifier, "id", "/[0-9]+/") \
    X(Abstraction, "abs", "<lambda> <term>") \
    X(Atom, "atom", "'(' <term> ')' | <id>") \
    X(Application, "app", "<atom> <appL>") \
    X(Term, "term", "<app> | <abs>") \
    X(ApplicationL, "appL", "<atom> <appL> | \"\"") \
    X(Program, "program", "/^/ <abs> /$/")

#define CHURCH_NOTATION \
    X(Lambda, "lambda", "'\\\\' | \"λ\"") \
    X(Dot, "dot", "'.'") \
    X(Identifier, "id", "/[a-z]/") \
    X(Abstraction, "abs", "<lambda> <id> <dot> <term>") \
    X(Atom, "atom", "'(' <term> ')' | <id>") \
    X(Application, "app", "<atom> <appL>") \
    X(Term, "term", "<app> | <abs>") \
    X(ApplicationL, "appL", "<atom> <appL> | \"\"") \
    X(Program, "program", "/^/ <abs> /$/")

#define X(id, name, def) + 1
static const int PARSER_COUNT = CHURCH_NOTATION;
#undef X

/**
 * Todo
 **
 * beta()
 */

signed
main (void) {

    #define X(id, name, def) pcq_parser_t *id = pcq_new(name);
    CHURCH_NOTATION
    #undef X

    pcq_err_t * failed_to_parse = pcqa_lang(PCQA_LANG_DEFAULT,
    #define X(id, name, def) name ": " def ";\n"
        CHURCH_NOTATION
    #undef X
    #define X(id, name, def) , id
        CHURCH_NOTATION,
    #undef X
        NULL
    );

    if ( failed_to_parse ) {
        #define X(id, name, def) name ": " def ";\n"
        printf("%s\n",
            CHURCH_NOTATION
        );
        #undef X

        pcq_err_print(failed_to_parse);
        return EXIT_FAILURE;
    }

    char * input;
    pcq_result_t r;
    while ( (input = readline("> ")) ) {
        add_history(input);

        if ( pcq_parse("<stdin>", input, Program, &r) ) {
            pcq_ast_print(r.output);
            pcq_ast_delete(r.output);
        } else {
            pcq_err_print(r.error);
            pcq_err_delete(r.error);
        }

        free(input);
    }

    #define X(id, name, def) , id
    pcq_cleanup(PARSER_COUNT
        CHURCH_NOTATION
    );
    #undef X

    return EXIT_SUCCESS;
}

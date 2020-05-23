#include "main.h"

/**
 * Todo
 **
 * beta()
 */

signed
main (void) {

    #define X(id, name, def) pcq_parser_t *id = pcq_new(name);
    DE_BRUIJN_INDEX_NOTATION
    CHURCH_NOTATION
    #undef X

    pcq_err_t * failed_to_generate = pcqa_lang(PCQA_LANG_DEFAULT,
    #define X(id, name, def) name ": " def ";\n"
        CHURCH_NOTATION
    #undef X
    #define X(id, name, def) , id
        CHURCH_NOTATION
    #undef X
    #ifndef NDEBUG
        , NULL
    #endif
    );

    if ( failed_to_generate ) {
        #define X(id, name, def) name ": " def ";\n"
        printf("%s\n",
            CHURCH_NOTATION
        );
        #undef X

        pcq_err_print(failed_to_generate);
        return EXIT_FAILURE;
    }

    failed_to_generate = pcqa_lang(PCQA_LANG_DEFAULT,
    #define X(id, name, def) name ": " def ";\n"
        DE_BRUIJN_INDEX_NOTATION
    #undef X
    #define X(id, name, def) , id
        DE_BRUIJN_INDEX_NOTATION
    #undef X
    #ifndef NDEBUG
        , NULL
    #endif
    );

    if ( failed_to_generate ) {
        #define X(id, name, def) name ": " def ";\n"
        printf("%s\n",
            DE_BRUIJN_INDEX_NOTATION
        );
        #undef X

        pcq_err_print(failed_to_generate);
        return EXIT_FAILURE;
    }

    char * input;
    pcq_result_t r;
    while ( (input = readline("> ")) ) {
        add_history(input);

        if ( pcq_parse("<stdin>", input, CHURCH_Program, &r) ) {
            STACK_INIT(bound_variables);
            de_bruijn(r.output, bound_variables);
            stack_free(bound_variables);
            memset(free_variables_in_scope, 0, sizeof free_variables_in_scope);

            church(r.output, bound_variables);
            stack_free(bound_variables);
            memset(free_variables_in_scope, 0, sizeof free_variables_in_scope);

            PRINT_CHURCH(r.output);
            pcq_ast_delete(r.output);
        } else {
            pcq_err_print(r.error);
            pcq_err_delete(r.error);
        }

        free(input);
    }

    #define X(id, name, def) , id
    pcq_cleanup(PARSER_COUNT
        DE_BRUIJN_INDEX_NOTATION
        CHURCH_NOTATION
    );
    #undef X

    return EXIT_SUCCESS;
}

void
de_bruijn (pcq_ast_t * church, struct stack * bound) {

    bool added_binder = false;

    for ( signed i = 0; i < church->children_num; ++i ) {
        pcq_ast_t * child = church->children[i];

        if ( !strncmp(child->tag, "dot", 3) || !strncmp(child->tag, "lambda", 6) ) {
            continue;
        }

        char binder = 0;
        if ( strstr(child->tag, "id") ) {
            binder = *child->contents;

            if ( !strncmp(child->tag, "id", 2) ) {
                added_binder = true;
                STACK_PUSH(bound, binder);
                child->contents[0] = 0;
                continue;
            }

            if ( !strncmp(child->tag, "atom", 4) ) {
                long idx = stack_search(bound, binder);
                if ( idx == -1 ) {
                    long j;
                    for ( j = 0; j < MAX_FREE_VARIABLE_COUNT; ++j ) {
                        if ( !free_variables_in_scope[j] ) {
                            free_variables_in_scope[j] = binder;
                            break;
                        }

                        if ( free_variables_in_scope[j] == binder ) {
                            break;
                        }
                    }

                    idx = (long )stack_size(bound) + j;
                }

                size_t len = !idx ? 1 : (size_t )log10(idx) + 1;
                char * n = realloc(child->contents, len + 1);
                child->contents = n;
                snprintf(child->contents, len + 1, "%ld", idx);
            }
        }

        de_bruijn(child, bound);
    }

    if ( added_binder ) {
        free(stack_pop(&bound));
    }
}

void
church (pcq_ast_t * de_bruijn, struct stack * bound) {

    for ( signed i = 0; i < de_bruijn->children_num; ++i ) {
        pcq_ast_t * child = de_bruijn->children[i];

        if ( !strncmp(child->tag, "dot", 3) ) {
            continue;
        }

        if ( !strncmp(child->tag, "lambda", 6) ) {
            STACK_PUSH(bound, bound ? bound->value + 1 : 'a');
            continue;
        }

        if ( child->children_num > 0 ) {
            church(child, bound);
            continue;
        }

        if ( !bound ) {
            continue;
        }

        if ( !strncmp(child->tag, "id", 2) ) {
            char * n = realloc(child->contents, 2);
            child->contents = n;
            snprintf(child->contents, 2, "%1c", bound->value);
            continue;
        }

        if ( !strncmp(child->tag, "atom", 4) ) {
            signed long idx = 0;
            sscanf(child->contents, "%ld", &idx);
            char binder = stack_peak(bound, idx);
            if ( !binder ) {
                signed height = stack_size(bound);
                binder = bound->value + 1 + (signed )(idx - height);
            }

            char * n = realloc(child->contents, 2);
            child->contents = n;
            snprintf(child->contents, 2, "%1c", binder);
            continue;
        }

        church(child, bound);
    }
}

void
print_ast_contents (pcq_ast_t * ast, enum Notation notation) {

    for ( signed i = 0; i < ast->children_num; ++i ) {
        pcq_ast_t * child = ast->children[i];

        if ( strstr(child->tag, "empty") ) {
            continue;
        }

        if ( notation == DE_BRUIJN ) {
            if ( !strncmp(child->tag, "dot", 3) ) {
                continue;
            }

            if ( !strncmp(child->tag, "atom|id", 7) ) {
                printf("%s", child->contents);
                continue;
            }
        } else {
            if ( strstr(child->tag, "id") || !strncmp(child->tag, "dot", 3) ) {
                printf("%s", child->contents);
                continue;
            }
        }

        if ( !strncmp(child->tag, "lambda", 6) ) {
            printf("λ");
            continue;
        }

        if ( strstr(child->tag, "char") ) {
            printf("%s", child->contents);
            continue;
        }

        if ( strstr(child->tag, "appL") ) {
            putchar(' ');
        }

        print_ast_contents(child, notation);
    }
}

void
ast_remove_child (pcq_ast_t * root, signed index) {

    if ( !root || (index + 1) >= root->children_num ) { return; }

    pcq_ast_delete(root->children[index]);
    root->children[index] = NULL;

    for ( signed i = index + 1; i < root->children_num; ++i ) {
        root->children[i - 1] = root->children[i];
    }

    --root->children_num;
}


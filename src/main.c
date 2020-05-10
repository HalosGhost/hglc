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
        CHURCH_NOTATION,
    #undef X
        NULL
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
        DE_BRUIJN_INDEX_NOTATION,
    #undef X
        NULL
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
            de_bruijn(r.output);
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
        DE_BRUIJN_INDEX_NOTATION
        CHURCH_NOTATION
    );
    #undef X

    return EXIT_SUCCESS;
}

void
de_bruijn (pcq_ast_t * church) {

    for ( signed i = 0; i < church->children_num; ++i ) {
        if ( strstr(church->children[i]->tag, "dot") ) {
            ast_remove_child(church, i--);
        } else {
            de_bruijn(church->children[i]);
        }
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


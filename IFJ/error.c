/*****************************************************************\
|	Project: Implementace překladače imperativního jazyka IFJ18   |
|																  |
|	Authors: Radovan Babic, xbabic09                              |
|            Daniela Peskova, xpesko00                            |
|            Adam Kucera, xkucer95                                |
|            Filip Dianiska, xdiani00                             |
|                                                                 |
|	File: error.c                                                 |                                                                  |
\*****************************************************************/

#include "error.h"

void err_check(int err){

    switch (err){
        case INTERNAL_ERR:
            fprintf(stderr, "INTERNAL ERROR!\n");
            exit(INTERNAL_ERR);

        case SYNTAX_ERR:
            fprintf(stderr, "SYNTAX ERROR!\n");
            exit(SYNTAX_ERR);

        case LEXICAL_ERR:
            fprintf(stderr, "LEXICAL ERROR!\n");
            exit(LEXICAL_ERR);

        case DEFINITION_ERR:
            fprintf(stderr, "DEFINITION ERROR!\n");
            exit(DEFINITION_ERR);

        case TYPE_ERR:
            fprintf(stderr, "TYPE ERROR!\n");
            exit(TYPE_ERR);

        case PARAMS_COUNT_ERR:
            fprintf(stderr, "PARAMS COUNT ERROR!\n");
            exit(PARAMS_COUNT_ERR);

        case SEMANTIC_ERR:
            fprintf(stderr, "SEMANTIC ERROR!\n");
            exit(SEMANTIC_ERR);

        case ZERO_DIVISION_ERR:
            fprintf(stderr, "ZERO DIVISION ERROR!\n");
            exit(ZERO_DIVISION_ERR);

        case EOF:
            fprintf(stderr, "EOF ERROR!\n");
            exit(SYNTAX_ERR);
    }
}
/*****************************************************************\
|	Project: Implementace překladače imperativního jazyka IFJ18   |
|																  |
|	Authors: Radovan Babic, xbabic09                              |
|            Daniela Peskova, xpesko00                            |
|            Adam Kucera, xkucer95                                |
|            Filip Dianiska, xdiani00                             |
|                                                                 |
|	File: error.h                                                 |                                                                  |
\*****************************************************************/

#ifndef ERROR_H
#define ERROR_H

#include "parser.h"

/* CONSTANT DEFINITIONS */
#define CHECK_OK 0
#define LEXICAL_ERR 1
#define SYNTAX_ERR 2

#define POTENTIAL_SYNTAX_ERR 420

#define DEFINITION_ERR 3
#define TYPE_ERR 4
#define PARAMS_COUNT_ERR 5
#define SEMANTIC_ERR 6

#define ZERO_DIVISION_ERR 9

#define INTERNAL_ERR 99

/**
 * Funckia na kontrolu nastavenia priznaku Error
 *
 * Pokial bol priznak nastaveny, funkcia vypise chybove hlasenie
 * a ukonci program s adekvatnou navratovou hodnotou
 *
 * @param externa premenna Error
**/
void err_check(int err);

#endif //ERROR_H

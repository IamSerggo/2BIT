/*****************************************************************\
|	Project: Implementace překladače imperativního jazyka IFJ18     |
|																                                  |
|	Authors: Radovan Babic, xbabic09                                |
|            Daniela Peskova, xpesko00                            |
|            Adam Kucera, xkucer95                                |
|            Filip Dianiska, xdiani00                             |
|                                                                 |
|	File: parser.h                                                  |                                                                |
\*****************************************************************/

#ifndef PARSER_H
#define PARSER_H

#include <stdbool.h>

#include "scanner.h"
#include "precedence_sa.h"
#include "queue.h"
#include "symtable.h"
#include "error.h"

#define NO_BUILT_IN_FUNC 69

/**
 * Hlavna funckia, kontroluje syntax po jednotlivych konstrukciach
 * Funkcia zaroven vzdy kontroluje nastavenie premennej Error
 *
 * @param aktualna pozicia v subore
 * @param keyword na kontrolu vyhodnocovania STATEMENTOV, START ked nechceme kontrolovat statement
 *
 * @return pozicia v subore, na ktorej funckia skoncila, EOF v pripade konca suboru
**/
int _PROG(int curr_char, tTokenName tName);

/**
 * Funckia na kontrolu statements v podmienkach, cykloch a definiciach
 * EOF kontrola
 *
 * Funkcia zabezpecuje, ze statement urcite nebude definicia funckia
 *
 * @param aktualna pozicia v subore
 * @param keyword na kontrolu vyhodnocovania, jeho nacitanim funkcia konci
 *
 * @return pozicia v subore, na ktorej funckia skoncila, EOF v pripade konca suboru
**/
int _STMNT(int curr_char, tTokenName tName);

/**
 * Funkcia na kontrolu syntaxe blokovych komentarov
 * Error kontrola
 * V pripade EOF skor ako povolene, funckia nastavuje Error na SYNTAX_ERR
 *
 * @param aktualna pozicia v subore
 *
 * @return pozicia v subore, na ktorej funckia skoncila
**/
int _COMMENT(int curr_char);


/**
 * Funkcia na kontrolu syntaxe while cyklov
 * Error kontrola
 * V pripade EOF skor ako povolene, funckia nastavuje Error na SYNTAX_ERR
 *
 * @param aktualna pozicia v subore
 *
 * @return pozicia v subore, na ktorej funckia skoncila
**/
int _LOOP(int curr_char);

/**
 * Funkcia na kontrolu syntaxe if podmienok
 * Error kontrola
 * V pripade EOF skor ako povolene, funckia nastavuje Error na SYNTAX_ERR
 *
 * @param aktualna pozicia v subore
 *
 * @return pozicia v subore, na ktorej funckia skoncila
**/
int _COND(int curr_char);

/**
 * Funkcia na kontrolu syntaxe definicie funckii
 * Error kontrola
 * V pripade EOF skor ako povolene, funckia nastavuje Error na SYNTAX_ERR
 *
 * Funckia kontroluje spravnost parametrov pomocou funkcie _PARAMS
 *
 * @param aktualna pozicia v subore
 *
 * @return pozicia v subore, na ktorej funckia skoncila
**/
int _F_DEF(int curr_char);

/**
 * Funkcia na kontrolu syntaxe parametrov pri definicii funckii
 * Error kontrola
 * V pripade EOF skor ako povolene, funckia nastavuje Error na SYNTAX_ERR
 *
 * Spravnost parametrov v zatvorkach riesi funckia _PARAMS_IN_PARS
 * Spravnost parametrov v definicii funckii riesi funckia _DEF_PARS
 * Spravnost parametrov v mimo zatvoriek riesi funckia _PARAMS_NO_PARS
 *
 * @param aktualna pozicia v subore
 * @param pri definovanej funkcii urcuje pocet argumentov
 * @param rozdiel medzi definiciou a volanim funckie
 *
 * @return pozicia v subore, na ktorej funckia skoncila, EOF v pripade konca suboru
**/
int _PARAMS(int curr_char, unsigned num_of_params, bool is_def);

/**
 * Pomocne funkcie na kontrolu spravnosti parametrov
 * Error kontrola
 * V pripade EOF skor ako povolene, funckia nastavuje Error na SYNTAX_ERR
 *
 * @param aktualna pozicia v subore
 *
 * @return pozicia v subore, na ktorej funckia skoncila, EOF v pripade konca suboru
**/
int _DEF_PARAMS(int curr_char);
int _PARAMS_IN_PARS(int curr_char, unsigned num_of_params);
int _PARAMS_NO_PARS(int curr_char, unsigned num_of_params);
int _PARAMS_TYPE_CHECK(tToken ** Token);
void tcheck(tToken ** T);

extern tToken *extT;
extern tBTNodePtr LocalFramePtr;
extern tBTNodePtr RootPtr;
extern tBTNodePtr TempUndefinedFunctions;
extern bool CHECKING_LOCAL_FRAME;
extern int FUNC_TO_DEFINE;


#endif //PARSER_H

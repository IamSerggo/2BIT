/*****************************************************************\
|	Project: Implementace překladače imperativního jazyka IFJ18     |
|																                                  |
|	Authors: Radovan Babic, xbabic09                                |
|            Daniela Peskova, xpesko00                            |
|            Adam Kucera, xkucer95                                |
|            Filip Dianiska, xdiani00                             |
|                                                                 |
|	File: symtable.h                                                |
\*****************************************************************/

#include "scanner.h"

#ifndef SYMTABLE_H
#define SYMTABLE_H

#define FREE_NUM_OF_PARAMS 42069 // INT32_MAX
/* struktura uzlu binarneho stromu */
typedef struct tBTNode {
  char * name;           // nazov identifikatoru
  tTokenName type;       // typ id (ID / ID_FUNC)
  unsigned paramsCount;  // pocet parametrov pri ID_FUNC
  unsigned paramId;      // id parametru vo funkcii, 0 -> nieje paramteroms

  struct tBTNode * LPtr;  // lavy podstrom
  struct tBTNode * RPtr;  // pravy podstrom

} * tBTNodePtr;

/* inicializuje tabulku symbolov
 * @param ukazatel na koren tabulky symbolov
 */
void SymInit(tBTNodePtr * RootPtr);

/**
 * Definuje identifikator
 *
 * @param ukazatel na koren tabulky symbolov
 * @param ukazatel na nazov identifikatoru
 * @param typ (ID / ID_FUNC)
 * @param pocet parametrov funkcie (pri type ID je ignorovany)
**/
void Define(tBTNodePtr * RootPtr, char * idName, tTokenName idType, unsigned paramsCount, unsigned paramId);

/**
 * Vracia odkaz na uzol identifikatora v tabulke symbolov, vracia NULL ak je nedefinovany
 *
 * @param ukazatel na koren tabulky symbolov
 * @param ukazatel na nazov identifikatoru
**/
tBTNodePtr IsDefined(tBTNodePtr * RootPtr, char * idName);

/* zrusi tabulku symbolov
 * @param ukazatel na koren tabulky symbolov
 */
void SymDispose(tBTNodePtr * RootPtr);


#endif

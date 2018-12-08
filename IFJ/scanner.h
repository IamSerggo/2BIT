/*****************************************************************\
|	Project: Implementace překladače imperativního jazyka IFJ18     |
|																                                  |
|	Authors: Radovan Babic, xbabic09                                |
|            Daniela Peskova, xpesko00                            |
|            Adam Kucera, xkucer95                                |
|            Filip Dianiska, xdiani00                             |
|																                                  |
\*****************************************************************/

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdio.h>

#ifndef SCANNER_H
#define SCANNER_H

#define BOF -2 // begging of file

extern int Error; // cislo chyby

/* poradie tTokenName prosim nemenit, precedence_sa na nom zavisi */
typedef enum {
  PLUS,        //  +
  MINUS,       //  -
  STAR,        //  *
  SLASH,       //  /
  LT,          //  <
  GT,          //  >
  LE,          //  <=
  GE,          //  >=
  NE,          //  !=
  EQ,          //  ==
  LEFT_PAR,    //  (
  RIGHT_PAR,   //  )
  ID,          // z koncovych stavov: ID_LOW, ID
  DOLAR,       // $ - len pre ucely PSA, nebude vracana v tokene

  START,       // sluzi ako inicializacna hodnota, nebude vracana v tokene

  INTEGER_L,   // z koncovych stavov: INT_L, ZERO, INT_OCT_L, INT_BIN_L, FS_INT_HEX_L
  STRING_L,    // z koncovych stavov: FS_STR_L
  FLOAT_L,     // z koncovych stavov: FLOAT_L, FS_FLOAT_EXP_L

  ID_FUNC,     // z koncovych stavov: ID_FUNC
  ASSIGN,      //  =
  EOL,         //  \n
  COMMA,       //  ,

  /* keywords */
  KW_DEF,
  KW_DO,
  KW_ELSE,
  KW_END,
  KW_IF,
  KW_NOT,
  KW_NIL,
  KW_THEN,
  KW_WHILE,
  KW_BEGIN // pre bonusovu cast

} tTokenName;

typedef union {
  char * String;  // pre String literal alebo pre identifikator
  int Integer;    // pre Integer literal
  double Float;   // pre Float literal
  // bool Bool bonus
} tTokenData;
// poznamka: token->name == STRING_L && token->data.String == NULL znamena "" (prazdny retazec)


typedef struct {
  tTokenName name; // nazov tokenu
  tTokenData data;
} tToken;


/**
 * Funkcia scanneru precita dalsi token,
 * vracia po nom nasledujuci znak, pri chybe alebo dosiahnuti konca suboru vracia EOF
 *
 * @param ukazatel na ukazatel na tToken (pri chybe ostane nedefinovany!)
 * @param posledny precitany znak, BOF ak nebol este precitany ziaden znak
**/
int NextToken(tToken ** token, int c);

#endif

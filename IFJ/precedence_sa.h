/*****************************************************************\
|	Project: Implementace překladače imperativního jazyka IFJ18     |
|																                                  |
|	Authors: Radovan Babic, xbabic09                                |
|          Daniela Peskova, xpesko00                              |
|          Adam Kucera, xkucer95                                  |
|          Filip Dianiska, xdiani00                               |
|                                                                 |
|	File: precedence_sa.h                                           |                                                                |
\*****************************************************************/

#ifndef PRECEDENCE_SA_H
#define PRECEDENCE_SA_H

#include "scanner.h"
#include "symtable.h"
#include "generator.h"

#define TRUE 1
#define FALSE 0
#define PSA_ALPPHABET_SIZE 14
#define RULES_SIZE 12

extern bool cmpOpAlreadyUsed; /* zabrani dvojitemu pouzitiu relacneho operatoru */
extern bool definitionError; /* ak nastane def aj syntax error, syntax ma prednost */

/*-------------------------- definície funkcií -------------------------------*/

/* hlavna funkcia precedencnej analyzy

  @param aktualny char zo scaneru potrebny pre nacitanie tokenu
  @return posledny nacitany token, ktory uz nepatri do vyrazu
*/
tToken* precedence_analysis(int*);

/* overi, ci token zo vstupu patri do precedencnej abecedy
  @param posledny precitany token
  @return true - patri, false - nepatri
*/
int is_token_from_alphabet(tTokenName);

/* precita token a skontroluje ho
  @param aktualny char zo scaneru potrebny pre nacitanie tokenu
  @return precitany token
*/
tToken* read_token(int*);

/* skontroluje, ci je token definovany
  ak nie je, nastavi Error na 3 - chyba nedefinovanej premennennej/funkcie
  @param posledny precitany token
  @return id parametru vo funkcii - pre generovanie
*/
unsigned is_token_defined(tToken*);

/* len pre ucely algoritmu precedencnej analyzy
  skontroluje tokenName, ak je string, float alebo integer, prepise ho na ID,
  inak neprepisuje
  @param token->name
  @return token->name
*/
int control_token(int);

/* pokusi sa aplikovat na string jedno z moznych pravidiel
  @param string z psa stacku
*/
bool try_to_apply_rule(char*);

/* kontroluje, ci je operator na vstupe relacny
  @param operator
  @return true - operator je relacny, inak false
*/
bool isCmpOp(int);

/* skonvertuje token->name pre ucely PSA
  @param token name typu char
  @return token name typu int
*/
char convert_int_to_char(char);

/* skonvertuje token->name pre ucely PSA
  @param token name typu int
  @return token name typu char
*/
int convert_char_to_int(int);

extern tBTNodePtr RootPtr;

/* tabulka pravidiel */
static const char* rules[RULES_SIZE] = {
  "i",      // E->i
  ")E(",    // E->(E)   reverse kvoli zasobniku
  "E+E",    // E->E+E
  "E-E",    // E->E-E
  "E*E",    // E->E*E
  "E/E",    // E->E/E
  "E<E",    // E->E<E
  "E>E",    // E->E>E
  "ELE",    // E->E<=E
  "EGE",    // E->E>=E
  "EQE",    // E->E==E
  "ENE"     // E->E!=E
};

/* precedencna tabulka */
static const char precedence_table[PSA_ALPPHABET_SIZE][PSA_ALPPHABET_SIZE] = {

  /* < -- @; > -- & */
	/*   	     +   -   *   /   <   >   <=  >=  !=  ==  (   )   i   $ */
	/* +  */ {'&','&','@','@','&','&','&','&','&','&','@','&','@','&'},
	/* -  */ {'&','&','@','@','&','&','&','&','&','&','@','&','@','&'},
	/* *  */ {'&','&','&','&','&','&','&','&','&','&','@','&','@','&'},
	/* /  */ {'&','&','&','&','&','&','&','&','&','&','@','&','@','&'},
	/* <  */ {'@','@','@','@',' ',' ',' ',' ',' ',' ','@','&','@','&'},
	/* >  */ {'@','@','@','@',' ',' ',' ',' ',' ',' ','@','&','@','&'},
	/* <= */ {'@','@','@','@',' ',' ',' ',' ',' ',' ','@','&','@','&'},
	/* >= */ {'@','@','@','@',' ',' ',' ',' ',' ',' ','@','&','@','&'},
	/* != */ {'@','@','@','@',' ',' ',' ',' ',' ',' ','@','&','@','&'},
	/* == */ {'@','@','@','@',' ',' ',' ',' ',' ',' ','@','&','@','&'},
	/* (  */ {'@','@','@','@','@','@','@','@','@','@','@','=','@',' '},
	/* )  */ {'&','&','&','&','&','&','&','&','&','&',' ','&',' ','&'},
	/* i  */ {'&','&','&','&','&','&','&','&','&','&',' ','&',' ','&'},
	/* $  */ {'@','@','@','@','@','@','@','@','@','@','@',' ','@',' '}
};


#endif

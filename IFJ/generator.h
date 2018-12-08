#ifndef GENERATOR_H
#define GENERATOR_H

#include "scanner.h"
#include "linked_list.h"

extern tLinkedList Compiled; // list so skompilovanym kodom

/* ----------------------------GENEROVANIE----------------------------------- */

/* vola sa na zaciatku generovania */
void Gen_Init_Program();

/* vypise skompilovany kod na stdout */
void Generate_Program();

/* ----------------------------TYPOVA-KONTROLA------------------------------- */

/* vola precedencia pri citani operandov vo vyraze */
void Gen_Push2DataStack(tToken * t, unsigned paramId);

/* vola precedencia pri aplikovani pravidla */
void Gen_ApplyOp(char op);

/* vola sa po ukonceni predecencie */
void Gen_EvalExpr();

/* ---------------------------DEFINICIA-A-PRIRADENIE------------------------- */

/* vola sa pri definici premennej */
void Gen_DefineVar(char * var, unsigned paramId);

/* vola sa po ukonceni precedencie pri priradeni */
void Gen_AssignTo(char * var, unsigned paramId);

/* --------------------------------IF-ELSE----------------------------------- */

/* vola sa po vyhodnoteni if (expr) */
void Gen_If();

/* vola sa pri else */
void Gen_Else();

/* vola sa pri konci IfElse statementu */
void Gen_EndIf();

/* ---------------------------------WHILE------------------------------------ */

/* vola sa pred najvrchnejsim while */
void MostTop_While();

/* vola sa pred vyhodnotenym podmienky cyklu */
void Gen_While_cond();

/* vola sa po KW_DO */
void Gen_While_begin();

/* vola sa pri definici premennej vnutri cyklu */
void Gen_PreWhile_Def(char * var, unsigned paramId);

/* vola sa pri KW_END */
void Gen_While_end();

/* --------------------------------FUNKCIE----------------------------------- */

/* vola sa pri KW_DEF */
void Gen_Func_PrepFrame();

void Gen_Push_Param(tToken * token, unsigned paramId);

void Gen_Call_Func(char * func);

void Gen_FuncDef_begin(char * func);

void Gen_FuncDef_end();

//------------------------------------------------------------------------------

void Gen_Call_Print(int n);

#endif

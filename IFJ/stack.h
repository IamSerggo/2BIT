/*****************************************************************\
|	Project: Implementace překladače imperativního jazyka IFJ18     |
|																                                  |
|	Authors: Radovan Babic, xbabic09                                |
|            Daniela Peskova, xpesko00                            |
|            Adam Kucera, xkucer95                                |
|            Filip Dianiska, xdiani00                             |
|                                                                 |
|	File: stack.h                                                   |
\*****************************************************************/


#ifndef STACK_H
#define STACK_H

#define MAXSTACK 50
#define STACK_BOTTOM '$'

/* zásobník hodnôt typu char */
typedef struct{
    char a[MAXSTACK];
    int top;
}tStack;

/* definície funkcií */
void stack_push(tStack*, char);
void stack_init(tStack*);
char stack_pop(tStack*);
int stack_empty(tStack*);
char stack_top_term(tStack*);
void copy_helper_to_stack(tStack*, tStack*, int);
void stack_change(tStack*, char);
char* stack_try_to_find_string(tStack*);
void stack_print(tStack*);

#endif

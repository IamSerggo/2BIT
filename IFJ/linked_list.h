#ifndef LINKEDLIST_H
#define LINKEDLIST_H

#include <stdlib.h>

/* zretazeny zoznam s obsahom pre Generator  */

typedef struct ListElem
{
  char * str;
  struct ListElem * next;
} * tListElem;

typedef struct LinkedList
{
  tListElem first;
  tListElem PreWhile;
  tListElem last;
} tLinkedList;


void list_Init(tLinkedList * list);

/* Vytvori novu polozku a vlozi na koniec zoznamu
 * pri CHYBE vracia 99, inak 0
 * @param ukazatel na list
 * @param ukazatel na retazec novej polozky
 */
int list_Append(tLinkedList * list, char * string);

/* Vytvori a vlozi novu polozku pred polozku list->PreWhile
 * pri CHYBE vracia 99, inak 0
 * @param ukazatel na list
 * @param ukazatel na retazec novej polozky
 */
int list_insert_PreWhile(tLinkedList * list, char * string);

/* Odstrani prvu polozku a vrati ukazatel na jej obsah */
char * list_PopFirst(tLinkedList * list);

#endif

/*****************************************************************\
|	Project: Implementace překladače imperativního jazyka IFJ18     |
|																                                  |
|	Authors: Radovan Babic, xbabic09                                |
|            Daniela Peskova, xpesko00                            |
|            Adam Kucera, xkucer95                                |
|            Filip Dianiska, xdiani00                             |
|                                                                 |
|	File: queue.h                                                   |
\*****************************************************************/

#include "scanner.h"

#ifndef QUEUE_H
#define QUEUE_H

/* fronta implementovana zretazenym zoznamom */

// struktura polozky fronty
typedef struct QItem {
  void * itemPtr;       // obsah polozky
  struct QItem * next;  // ukazatel na dalsiu polozku
} * tQItem;

// struktura fronty
typedef struct {
    tQItem front;  // zaciatok fronty
    tQItem back;   // koniec fronty
} tQueue;


/* Inicializuje frontu
 * @param ukazatel na frontu
*/
void queueInit(tQueue * q);


/* Vlozi obsah (jeho ukazatel) na koniec fronty
 * @param ukazatel na frontu
 * @param ukazatel na vkladany obsah
*/
void queuePush(tQueue * q, void * content);


/* Zrusi prvu polozku fronty a vrati jej obsah (jeho ukazatel)
 * @param ukazatel na frontu
*/
void * queuePop(tQueue * q);

#endif

/*****************************************************************\
|	Project: Implementace překladače imperativního jazyka IFJ18     |
|																                                  |
|	Authors: Radovan Babic, xbabic09                                |
|            Daniela Peskova, xpesko00                            |
|            Adam Kucera, xkucer95                                |
|            Filip Dianiska, xdiani00                             |
|                                                                 |
|	File: queue.c                                                   |
\*****************************************************************/

#include "queue.h"


void queueInit(tQueue * q) {
  if (q != NULL) {
    q->front = NULL;
    q->back  = NULL;
  }
}


void queuePush(tQueue * q, void * content) {

  if (q->back == NULL) {

    // pridanie prveho prvku

    if ((q->back = malloc(sizeof(tQItem))) == NULL) {
      Error = 99; // interna chyba
      return;
    }
    q->back->itemPtr = content;
    q->front = q->back;
  }
  else {

    // pridanie ostatnych prvkov

    if ((q->back->next = malloc(sizeof(tQItem))) == NULL) {
      Error = 99; // interna chyba
      return;
    }
    q->back = q->back->next;
    q->back->itemPtr = content;
  }
}


void * queuePop(tQueue * q) {
  if (q->front == NULL)
    return NULL;

  void * content = q->front->itemPtr; // obsah

  // prepisanie a uvolnenie front prvku
  tQItem tmp = q->front;
  q->front = q->front->next;
  free(tmp);

  return content;
}

#include "linked_list.h"

void list_Init(tLinkedList * list)
{
  list->first = NULL;
  list->last = NULL;
  list->PreWhile = NULL;
}

int list_Append(tLinkedList * list, char * string)
{
  tListElem tmp = malloc(sizeof(struct ListElem));
  if (tmp == NULL) // interna chyba
    return 99;

  tmp->next = NULL;
  tmp->str = string;

  if (list->first == NULL)
  {
    list->first = tmp;
    list->last = list->first;
  }
  else {
    list->last->next = tmp;
    list->last = list->last->next;
  }

  return 0;
}

int list_insert_PreWhile(tLinkedList * list, char * string)
{
  if (list->PreWhile != NULL) {
    tListElem tmp = malloc(sizeof(struct ListElem));
    if (tmp == NULL)
      return 99;

    tmp->str = string;
    tmp->next = (list->PreWhile->next != NULL) ? list->PreWhile->next : NULL;
    list->PreWhile->next = tmp;
  }
  return 0;
}

char * list_PopFirst(tLinkedList * list)
{
  if (list->first == NULL)
  {
    list->last = NULL;
    return NULL;
  }

  tListElem tmp = list->first;
  char * retval = list->first->str;

  list->first = list->first->next;

  free(tmp);

  return retval;
}

/*****************************************************************\
|	Project: Implementace překladače imperativního jazyka IFJ18     |
|																                                  |
|	Authors: Radovan Babic, xbabic09                                |
|            Daniela Peskova, xpesko00                            |
|            Adam Kucera, xkucer95                                |
|            Filip Dianiska, xdiani00                             |
|                                                                 |
|	File: symtable.c                                                |
\*****************************************************************/

#include "symtable.h"

void SymInit (tBTNodePtr * RootPtr) {
  *RootPtr = NULL; // dereferencia

  // Definuje VSTAVANE FUNKCIE
  Define(RootPtr, "inputs", ID_FUNC, 0, 0);
  Define(RootPtr, "inputi", ID_FUNC, 0, 0);
  Define(RootPtr, "inputf", ID_FUNC, 0, 0);
  Define(RootPtr, "length", ID_FUNC, 1, 0);
  Define(RootPtr, "substr", ID_FUNC, 3, 0);
  Define(RootPtr, "ord", ID_FUNC, 2, 0);
  Define(RootPtr, "chr", ID_FUNC, 1, 0);
  Define(RootPtr, "print", ID_FUNC, FREE_NUM_OF_PARAMS, 0); // lubovolny pocet paramtetrov
}


void Define(tBTNodePtr * RootPtr, char * idName, tTokenName idType, unsigned paramsCount, unsigned paramId) {
  while (*RootPtr != NULL) {
		if (strcmp((*RootPtr)->name, idName) == 0) {
      Error = 3; // chyba redefinice premennej
      return;
    }

		if (strcmp((*RootPtr)->name, idName) > 0)
			RootPtr = &(*RootPtr)->LPtr;
		else
			RootPtr = &(*RootPtr)->RPtr;
	}

	// pri uspesne alokaci noveho uzla ho inicializuje
	if ((*RootPtr = malloc(sizeof(struct tBTNode))) != NULL) {

    (*RootPtr)->name = idName;
    (*RootPtr)->type = idType;
    (*RootPtr)->paramsCount = paramsCount;
        (*RootPtr)->paramId = paramId;
		(*RootPtr)->LPtr = NULL;
		(*RootPtr)->RPtr = NULL;
	}
  else {
    Error = 99; // interna chyba prekladaca
  }
}


tBTNodePtr IsDefined(tBTNodePtr * RootPtr, char * idName) {
  while (*RootPtr != NULL) {
		if (strcmp((*RootPtr)->name, idName) == 0)
      return *RootPtr;

		if (strcmp((*RootPtr)->name, idName) > 0)
			RootPtr = &(*RootPtr)->LPtr;
		else
			RootPtr = &(*RootPtr)->RPtr;
	}

  return NULL;
}


void SymDispose(tBTNodePtr * RootPtr) {
  if (*RootPtr != NULL) {
    SymDispose(&(*RootPtr)->LPtr); // rekurzia nad levym podstromom
    SymDispose(&(*RootPtr)->RPtr); // rekurzia nad pravym podstromom
    free(*RootPtr);
    *RootPtr = NULL;
  }
}

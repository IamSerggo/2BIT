/*****************************************************************\
|	Project: Implementace překladače imperativního jazyka IFJ18   	|
|																  																|
|	Authors: Radovan Babic, xbabic09                              	|
|            Daniela Peskova, xpesko00                            |
|            Adam Kucera, xkucer95                                |
|            Filip Dianiska, xdiani00                             |
|                                                                 |
|	File: stack.c                                                 	|
\*****************************************************************/

// TODO ak je funkcia typu static, nemusis alokovat?? char* stack_try_to_find_string()

#include "stack.h"
#include "scanner.h"
#include "precedence_sa.h"

/* Funkcie pre zásobník */

/*------------------Zakladne funkcie z IAL projektu --------------------------*/

/* Inicializácia zásobníku */
void stack_init(tStack *S){
	S->top = 0;
}

/* Vloží hodnotu na vrchol zásobníku */
void stack_push(tStack *S, char alphabetChar){
                 /* Při implementaci v poli může dojít k přetečení zásobníku. */
  if (S->top==MAXSTACK){
    printf("stack_push error\n");
    Error = 42;
    return;
  } else {
		S->top++;
		S->a[S->top]=alphabetChar;
	}
}

/* Odstraní prvek z vrcholu zásobníku a současně vrátí jeho hodnotu */
char stack_pop(tStack *S){
                /* Operace nad prázdným zásobníkem způsobí chybu. */
	if (S->top==0){
    printf("stack_pop error\n");
    Error = 42;
    return -1;
  }	else {
		return (S->a[S->top--]);
	}
}

/* Je-li zásobník prázdný, vrátí hodnotu true */
int stack_empty(tStack *S){
  return(S->top == 0);
}

/*------------------Pomocne funkcie pre precedencnu SA------------------------*/

/* prejde cely stack a pokusi sa najst retazec "<stringOnStack"
	ak ho najde, vrati "stringOnStack" bez '<'
	ak ho nenajde, vrati NULL */
char* stack_try_to_find_string(tStack* S){
	char c = stack_pop(S); /* posledny nacitany char z vrcholu stacku */
	int i = 0; /* pomocna premenna - indexovanie vystupneho stringu */

	char* stringOnStack = malloc(MAXSTACK); /* vystupny string */
	if (stringOnStack == NULL){
		Error = 42;
		return stringOnStack;
	}


	while(c != '@') { /* '@' = '<' v PSA tabulke */
		stringOnStack[i] = c;
		c = stack_pop(S);
		i++;
	}

	stringOnStack[i] = '\0';

	return stringOnStack;
}

/* za dany terminal vlozi symbol '@' - nahradza v tabulke symbol '<' */
void stack_change(tStack *S, char terminal){

	tStack helper; // pomocny stack, do kt. ukladame neterminaly na vrchole stacku
  char item; // prvy terminal na zasobniku
	int counter = 0;

  stack_init(&helper);
  stack_push(&helper, STACK_BOTTOM);

  /* do terminalu si nacitame dalsi prvok na zasobniku */
  while(!stack_empty(S)) {

		item = stack_pop(S);
		/* nasli sme prvu zhodu na zasobniku */
		if (item == terminal){
			stack_push(S, item);
			stack_push(S, '@'); /* '@' - nahrada v PSA za '<' */
			copy_helper_to_stack(S, &helper, counter);
			return;
		}

		counter++;
		stack_push(&helper, item);
	}

	Error = 42;
}

/* vráti najvrchnejší terminál */
char stack_top_term(tStack *S){

  tStack helper; // pomocny stack, do kt. ukladame neterminaly na vrchole stacku
  char terminal; // navratova hodnota, prvy terminal na zasobniku
	int i = 0;

  stack_init(&helper);
  stack_push(&helper, STACK_BOTTOM);

  /* do terminalu si nacitame dalsi prvok na zasobniku */
  while(!stack_empty(S)) {
		terminal = stack_pop(S);

    /* ak je prvok v psa abecede, je to terminal, inak neterminal */
    if (is_token_from_alphabet(convert_char_to_int(terminal))) {
      /* potrebujeme na stack vratit aj terminal, ktory sme prave nasli */
			stack_push(S, terminal);
      copy_helper_to_stack(S, &helper, i);
      return terminal;
    }

    /* ak nas 'terminal' v skutocnosti nie je terminal,
     ulozime si ho do pomocneho stacku */
    stack_push(&helper, terminal);
		i++;
  }

	copy_helper_to_stack(S, &helper, i);
  return terminal;
}

/* skopirujem cely obsah helpera spat na stack */
void copy_helper_to_stack(tStack *S, tStack *helper, int counter) {

  int c;
  // while((c = stack_pop(helper)) != STACK_BOTTOM){
	while(counter > 0){
		c = stack_pop(helper);
    stack_push(S, c);
		counter--;
  }
}

void stack_print(tStack *S) {

	char c;
	int i = 0;
	tStack helper;
	stack_init(&helper);
	stack_push(&helper, STACK_BOTTOM);

	while(!stack_empty(S)){
		c = stack_pop(S);
		printf("c: %c\n", c);
		stack_push(&helper, c);
		i++;
	}

	copy_helper_to_stack(S, &helper, i);
}

/*****************************************************************\
|	Project: Implementace překladače imperativního jazyka IFJ18     |
|																                                  |
|	Authors: Radovan Babic, xbabic09                                |
|            Daniela Peskova, xpesko00                            |
|            Adam Kucera, xkucer95                                |
|            Filip Dianiska, xdiani00                             |
|                                                                 |
|	File: precedence_sa.c                                           |                                                                |
\*****************************************************************/

#include "precedence_sa.h"
#include "stack.h"
#include "scanner.h"
#include "error.h"
#include "generator.h"

tToken* precedence_analysis(int *charFromScanner) {

  tToken* T;
  tStack S;
  char stackTop; /* prvy terminal na stacku */
  char inputToken; /* token na vstupe */
  char* stringOnStack; /* string zo stacku, na ktory budeme uplatnovat pravidla*/
  int int_stackTop, int_inputToken;
  unsigned paramId;

  stack_init(&S);
  stack_push(&S, STACK_BOTTOM);

  /*ak je externy token prazdny(extT->name = START), citam novy token
  inak za svoj prvy token beriem extT*/
  if (extT->name != START){
    T = extT;
    if (T->name != KW_NIL)
      paramId = is_token_defined(T);
    else
      Error = SYNTAX_ERR;
    if ( _PARAMS_TYPE_CHECK(&T) )
      Gen_Push2DataStack(T, paramId);
    T->name = control_token(T->name);
  } else {
    T = read_token(charFromScanner);
  }


  if (Error) {
    cmpOpAlreadyUsed = false;
    return T;
  }

  /* ak prvy nacitany token nie je z abecedy: */
  if (!is_token_from_alphabet(T->name)){
    if (T->name == ID_FUNC)
      Error = POTENTIAL_SYNTAX_ERR;
    else
      Error = SYNTAX_ERR;
  }

  if (Error) {
    cmpOpAlreadyUsed = false;
    return T;
  }

  /* ALGORITMUS Z PREDNASKY */
  do {
    stackTop = stack_top_term(&S);
    inputToken = convert_int_to_char(T->name);

    if (Error) {
      cmpOpAlreadyUsed = false;
      return T;
    }

    if (!is_token_from_alphabet(T->name)) {
      inputToken = STACK_BOTTOM;
    }

    int_stackTop = convert_char_to_int(stackTop);
    int_inputToken = convert_char_to_int(inputToken);

    switch (precedence_table[int_stackTop][int_inputToken]) {
      case '@': /* '<' */
        /* zamen mi stackTop za stackTop< a pushni token na stack, potom
        poziadaj o novy token */
        stack_change(&S, stackTop);
        stack_push(&S, inputToken );
        T = read_token(charFromScanner);
        break;

      case '&': /* '>' */
        /* pokus sa najst na stacku <string, ak ho najdes, hladaj pravidlo,
        ktore nanho uplatnis. Kedze string je uz popnuty zo stacku, nemusis
        ho zamienat, iba pushni lavu stranu pravidla na stack.
        Je potrebne uvolnit pamat, ktoru sme alokovali na string vo funkcii
        stack_try_to_find_string() */
        stringOnStack = stack_try_to_find_string(&S);
        if (stringOnStack != NULL && try_to_apply_rule(stringOnStack)){
          free(stringOnStack);
          stack_push(&S, 'E');
        } else {
          free(stringOnStack);
          Error = 2;
          cmpOpAlreadyUsed = false;
          return T;
        }
        break;

      case '=':
        /* pushnem inputToken na stack a poziadam o nový token */
        stack_push(&S, inputToken);
        T = read_token(charFromScanner);
        break;

      default:
        if (stackTop == STACK_BOTTOM && inputToken == STACK_BOTTOM) {
          while(!stack_empty(&S))
            int_stackTop = stack_pop(&S);
          break;
        }

        Error = 2;
        cmpOpAlreadyUsed = false;
        return T;
    }

  } while (!(stackTop == STACK_BOTTOM && inputToken == STACK_BOTTOM));


  /* ak sa stack nevyprázdnil, došlo k syntaktickej chybe */
  if (!stack_empty(&S)) {
    Error = 2;
    }

  /* syntax error ma vyssiu prioritu ako definition error*/
  if (definitionError)
    Error = DEFINITION_ERR;

  if(T->name != KW_DO && T->name != KW_THEN && T->name != EOL)
    Error = SYNTAX_ERR;

  if (Error) {
    cmpOpAlreadyUsed = false;
    return T;
  }

  cmpOpAlreadyUsed = false;
  Gen_EvalExpr();
  return T;
}

int is_token_from_alphabet(tTokenName tokenName){

  /* ak token patri do abecedy, jeho hodnota je medzi 0 a 13 - vid. scanner.h */
  if (tokenName <= ID && tokenName >= PLUS)
    return TRUE;

  return FALSE;
}

bool try_to_apply_rule(char* stringOnStack){

  for (int i = 0; i < RULES_SIZE; i++){
    if (strcmp(rules[i], stringOnStack) == 0) {
      if (strcmp(rules[i], "i") != 0 && strcmp(rules[i], ")E(") != 0){
        Gen_ApplyOp(stringOnStack[1]);
        // generuj(stringOnStack[1])
      }
      return true;
    }
  }

  return false;
}

unsigned is_token_defined(tToken* T){

  tBTNodePtr helper;

  if (T->name == ID){

    /* funkciu IsDefined volame nad RootPtr alebo LocalFramePtr
      podla toho, ci sme v definicii funkcie alebo nie */
    if (CHECKING_LOCAL_FRAME)
      helper = IsDefined(&LocalFramePtr, T->data.String);
    else
      helper = IsDefined(&RootPtr, T->data.String);

    /* ID nie je definovane */
    if (helper == NULL){
      definitionError = DEFINITION_ERR;
      return 0;
    }
    else if (helper->type != ID && helper->type != ID_FUNC){
      Error = SYNTAX_ERR;
    }
    else if (helper->type == ID_FUNC){
      Error = POTENTIAL_SYNTAX_ERR;
      T->name = ID_FUNC;
    }

    return helper->paramId;
  }

  return 0;
}

tToken* read_token(int* charFromScanner){

  tToken* T;
  unsigned paramId;

  *charFromScanner = NextToken(&T, *charFromScanner);

  if (Error) return T;

  if (isCmpOp(T->name)){
    Error = (cmpOpAlreadyUsed) ? 4:0;
    cmpOpAlreadyUsed = TRUE;
  }
  if (Error) return T;

  /* ak je to id premennej, ktora este nie je definovana, Error = 3
  ak je to id funkcie, potencialny error*/

  if(T->name != KW_NIL)
    paramId = is_token_defined(T);
  else{
   Error = SYNTAX_ERR;
    if (Error) {
      cmpOpAlreadyUsed = false;
      return T;
    }
  }

  // volaj generator este pred prepisom token name
  if ( _PARAMS_TYPE_CHECK(&T) && !definitionError)
    Gen_Push2DataStack(T, paramId);


  T->name = control_token(T->name);
  return T;
}

int control_token(int tokenName){

  switch (tokenName) {
    case INTEGER_L:
    case STRING_L:
    case FLOAT_L:
    case KW_NIL:
      return ID;
  }
  return tokenName;
}

bool isCmpOp(int tokenName){

  if (tokenName >= LT && tokenName <= EQ)
    return TRUE;

  return FALSE;
}

char convert_int_to_char(char input){
  switch (input) {
    case ID:
      return 'i';
    case LEFT_PAR:
      return '(';
    case RIGHT_PAR:
      return ')';
    case PLUS:
      return '+';
    case MINUS:
      return '-';
    case STAR:
      return '*';
    case SLASH:
      return '/';
    case LT:
      return '<';
    case GT:
      return '>';
    case LE:      // <=
      return 'L';
    case GE:      // >=
      return 'G';
    case EQ:      // ==
      return 'Q';
    case NE:      // !=
      return 'N';
    case DOLAR:
      return '$';
    case ASSIGN:
      Error = POTENTIAL_SYNTAX_ERR;
    default:
      return input;
  }
}

int convert_char_to_int(int input){
  switch (input) {
    case 'i':
      return ID;
    case '(':
      return LEFT_PAR;
    case ')':
      return RIGHT_PAR;
    case '+':
      return PLUS;
    case '-':
      return MINUS;
    case '*':
      return STAR;
    case '/':
      return SLASH;
    case '<':
      return LT;
    case '>':
      return GT;
    case 'L':     // <=
      return LE;
    case 'G':     // >=
      return GE;
    case 'Q':     // ==
      return EQ;
    case 'N':     // !=
      return NE;
    case '$':
      return DOLAR;
    default:
      return input;
  }
}

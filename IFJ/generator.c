#include "generator.h"

#include <string.h>
#include <stdarg.h>

tLinkedList Compiled;

// id labelov
unsigned long tc_id;      // typova kontrola
unsigned long ifelse_id;  // labely pre if
unsigned long while_id;   // labely pre while
unsigned long overf_id;   // labely pre funkcie

unsigned long param_id;   // id parametrov vo funkcii

const char * Par     = "LF@\x25""Par_"; // cislovany parameter vo funkcii
const char * PrepPar = "TF@\x25""Par_";  // pushovanie do funkcie

// globalne premenne
const char * GF_retval = "GF@\x25retval";
const char * GF_op1 = "GF@\x25op1";
const char * GF_op2 = "GF@\x25op2";
const char * GF_op1_type = "GF@\x25op1_type";
const char * GF_op2_type = "GF@\x25op2_type";
const char * GF_printParams = "GF@\x25printParams";

// konstanty
const char * Exit_Type = "int@4";
const char * Exit_Zero = "int@9";
const char * StrType   = "string@string";
const char * FloatType = "string@float";
const char * IntType   = "string@int";
const char * ZeroInt   = "int@0";
const char * ZeroFloat = "float@0x0p+0";
const char * ExprTrue  = "bool@true";
const char * MaxChr    = "int@255";

/* interne labely */
const char * op1Int    = "\x25""op1Int_";
const char * op2Int    = "\x25""op2Int_";
const char * op1Float  = "\x25""op1Float_";
const char * op2Float  = "\x25""op2Float_";
const char * op1Str    = "\x25""op1Str_";
const char * op2Str    = "\x25""op2Str_";
const char * typeOk    = "\x25""typeOk_";
const char * retypeOp1 = "\x25""retypeOp1_";
const char * retypeOp2 = "\x25""retypeOp2_";
const char * opConcat  = "\x25""opConcat_";
const char * notStr    = "\x25""notStr_";
const char * divFloat  = "\x25""divFloat_";
const char * divInt    = "\x25""divInt_";
const char * divZero   = "\x25""divZero_";
const char * divOk     = "\x25""divOk_";
const char * EndIf     = "\x25""EndIf_";
const char * ElseSt    = "\x25""ElseSt_";
const char * whileCond = "\x25""whileCond_";
const char * whileEnd  = "\x25""whileEnd_";
const char * overf     = "\x25""overf_";

// pre lepsiu citatelnost
#define APPEND(param) \
  if (list_Append(& Compiled, param) == 99) { \
    Error = 99;                               \
    return;                                   \
  }


// vrati symbol tokenu vo vyraze s prislusnym prefixom
char * tok2symb(tToken * t)
{
  char buff[24];
  const char * prefix;
  switch (t->name) {
    case ID:
      prefix = "LF@";
    break;
    case FLOAT_L:
      prefix = "float@";
      snprintf(buff, 24, "%a", t->data.Float);
      t->data.String = buff;
    break;
    case INTEGER_L:
      prefix = "int@";
      snprintf(buff, 24, "%d", t->data.Integer);
      t->data.String = buff;
    break;
    case STRING_L:
      prefix = "string@";
    break;
    default:;
  }

  char * retval = malloc(sizeof(char) *
    (strlen(prefix) + strlen(t->data.String) + 1));

  if (retval != NULL) {
    strcpy(retval, prefix);
    strcat(retval, t->data.String);
  }
  return retval;
}

// generuje prislusny label s jednoznacnym Id
char * appendID(const char * baseName, unsigned long counter)
{
  char id[21]; // len(str(2^64))
  sprintf(id, "%lu", counter);
  char * label = malloc(sizeof(char) * (strlen(baseName) + strlen(id)));
  if (label != NULL) {
    strcpy(label, baseName);
    strcat(label, id);
  }
  return label;
}

/* vytvori label dynamicky retazec z const char * */
char * Const2Dynamic(const char * func)
{
  char * label = malloc(sizeof(char) * (strlen(func) + 1));
  if (label != NULL) {
    strcpy(label, func);
  }
  return label;
}

// alokuje retazec obsahujuci intrukciu so volitnelnym poctom symbolov
char * WriteInst(const char * inst, int n, ...)
{
  unsigned size = strlen(inst);

  va_list args;
  char * symb[3];
  va_start(args, n);
  for (int i = 0; i < n; i++) {
    symb[i] = va_arg(args, char *);
    size += strlen(symb[i]);
  }
  va_end(args);
  char * line = malloc(sizeof(char) * (size + n + 2));
  if (line == NULL)
    return NULL;

  strcpy(line, inst);
  for (int i = 0; i < n; i++) {
    strcat(line, " ");
    strcat(line, symb[i]);

    if (symb[i][0] != 'G' && symb[i][1] != 'F') {
      if (symb[i] != Exit_Type && symb[i] != Exit_Zero && symb[i] != StrType &&
          symb[i] != IntType  && symb[i] != FloatType && symb[i] == ZeroInt && symb[i] == ZeroFloat)
      {
        free(symb[i]);
      }
    }
  }
  strcat(line, "\n");
  return line;
}

/*----------------------------------PUBLIC------------------------------------*/

void printParamInc()
{
  APPEND (WriteInst("ADD", 3, GF_printParams, GF_printParams, Const2Dynamic("int@1")));
}

void Gen_Init_Program()
{
  tc_id = 0;
  ifelse_id = 0;
  while_id = 0;

  list_Init(& Compiled);

  APPEND (WriteInst("DEFVAR", 1, GF_retval));
  APPEND (WriteInst("DEFVAR", 1, GF_op1));
  APPEND (WriteInst("DEFVAR", 1, GF_op2));
  APPEND (WriteInst("DEFVAR", 1, GF_op1_type));
  APPEND (WriteInst("DEFVAR", 1, GF_op2_type));
  APPEND (WriteInst("DEFVAR", 1, GF_printParams));

  // definicie vstavanych funkcii
  //char * inputs = Const2Dynamic("\x25""inputs");
  //char * inputi = Const2Dynamic("\x25""inputi");
  //char * inputf = Const2Dynamic("\x25""inputf");
  char * length = Const2Dynamic("length");
  //char * substr = Const2Dynamic("\x25""substr");
  //char * ord    = Const2Dynamic("ord");
  //char * chr    = Const2Dynamic("chr");
  char * print  = Const2Dynamic("\x25""print");

  char * startLabel = Const2Dynamic("\x25""startLabel");

  APPEND (WriteInst("JUMP", 1, startLabel));

  //printf
  APPEND (WriteInst("LABEL", 1, print));
  APPEND (WriteInst("LABEL", 1, Const2Dynamic("\x25""printLoop")));
  APPEND (WriteInst("JUMPIFEQ", 3, Const2Dynamic("\x25""printEnd"), GF_printParams, ZeroInt));

  APPEND (WriteInst("SUB", 3, GF_printParams, GF_printParams, Const2Dynamic("int@1")));
  APPEND (WriteInst("POPS", 1, GF_op1));
  APPEND (WriteInst("CONCAT", 3, GF_retval, GF_retval, GF_op1));
  APPEND (WriteInst("JUMP", 1, Const2Dynamic("\x25""printLoop")));
  APPEND (WriteInst("LABEL", 1, Const2Dynamic("\x25""printEnd")));

  APPEND (WriteInst("WRITE", 1, GF_retval));
  APPEND (WriteInst("POPFRAME", 0));
  APPEND (WriteInst("RETURN", 0));

  // length
  APPEND (WriteInst("LABEL", 1, length));
  APPEND (WriteInst("STRLEN", 2, GF_retval, appendID(Par, 1)));
  APPEND (WriteInst("POPFRAME", 0));
  APPEND (WriteInst("RETURN", 0));

  // ord
  /*
  APPEND (WriteInst("LABEL", 1, ord));
  APPEND (WriteInst("JUMPIFEQ", 3, Const2Dynamic("\x25""OrdOk_1"), appendID(Par, 1), StrType));
  APPEND (WriteInst("EXIT", 1, Exit_Type));
  APPEND (WriteInst("LABEL", 1, Const2Dynamic("\x25""OrdOk_1")));
  APPEND (WriteInst("JUMPIFEQ", 3, Const2Dynamic("\x25""OrdOk_2"), appendID(Par, 2), IntType));
  APPEND (WriteInst("EXIT", 1, Exit_Type));
  APPEND (WriteInst("LABEL", 1, Const2Dynamic("\x25""OrdOk_2")));
  APPEND (WriteInst("STRI2INT", 3, GF_retval, appendID(Par, 1), appendID(Par, 1)));
  APPEND (WriteInst("POPFRAME", 0));
  APPEND (WriteInst("RETURN", 0));
*/

  // chr
  /*
  APPEND (WriteInst("LABEL", 1, chr));
  APPEND (WriteInst("TYPE", 1, GF_op1_type));
  APPEND (WriteInst("JUMPIFEQ", 3, Const2Dynamic("\x25""ChrTypeOk")));
  APPEND (WriteInst("EXIT", 1, Exit_Type));
  APPEND (WriteInst("LABEL", 1, Const2Dynamic("\x25""ChrTypeOk")));
  APPEND (WriteInst("PUSHS", 1, MaxChr));
  APPEND (WriteInst("PUSHS", 1, appendID(Par, 1)));
  APPEND (WriteInst("GTS", 0));
  APPEND (WriteInst("PUSHS", 1, ZeroInt));
  APPEND (WriteInst("PUSHS", 1, appendID(Par, 1)));
  APPEND (WriteInst("LTS", 0));
  APPEND (WriteInst("ORS", 0));
  APPEND (WriteInst("POPS", 1, GF_retval));
  APPEND (WriteInst("JUMPIFNEQ", 3, Const2Dynamic("\x25""ChrRangeOk")));
  APPEND (WriteInst("EXIT", 1, Exit_Type));
  APPEND (WriteInst("LABEL", 1, Const2Dynamic("\x25""ChrRangeOk")));
  APPEND (WriteInst("INT2CHAR", 1, appendID(Par, 1)));
*/

  // hlavne telo programu
  APPEND (WriteInst("LABEL", 1, startLabel));
  APPEND (WriteInst("CREATEFRAME", 0));
  APPEND (WriteInst("PUSHFRAME", 0));
}

void Generate_Program()
{
  printf("%s\n", ".IFJcode18"); // header

  while (Compiled.first != NULL) {
    char * str = list_PopFirst(& Compiled);

    if (str == NULL) { // interna chyba prekladaca
      Error = 99;
      return;
    }
    printf("%s", str);
    free(str);
  }
}

void Gen_Push2DataStack(tToken * t, unsigned paramId)
{
  if (t->name == ID && paramId) {
    APPEND (WriteInst("PUSHS", 1, appendID(Par, paramId)));
  }
  else {
    APPEND (WriteInst("PUSHS", 1, tok2symb(t)));
  }
}

void Gen_ApplyOp(char op)
{
  APPEND (WriteInst("POPS", 1, GF_op2));
  APPEND (WriteInst("POPS", 1, GF_op1));
  APPEND (WriteInst("TYPE", 2, GF_op1_type, GF_op1));
  APPEND (WriteInst("TYPE", 2, GF_op2_type, GF_op2));

  APPEND (WriteInst("JUMPIFEQ", 3, appendID(op1Int, tc_id), GF_op1_type, IntType));
  APPEND (WriteInst("JUMPIFEQ", 3, appendID(op2Int, tc_id), GF_op2_type, IntType));
  APPEND (WriteInst("JUMPIFEQ", 3, appendID(op1Float, tc_id), GF_op1_type, FloatType));
  APPEND (WriteInst("JUMPIFEQ", 3, appendID(op2Float, tc_id), GF_op2_type, FloatType));

  if (op == '+' || op == '>' || op == '<' || op == 'L' || op == 'G' || op == 'Q' || op == 'N')
  {
    APPEND (WriteInst("JUMPIFEQ", 3, appendID(op1Str, tc_id), GF_op1_type, StrType));
    APPEND (WriteInst("JUMPIFEQ", 3, appendID(op2Str, tc_id), GF_op2_type, StrType));
  }

  // spolocna pre aritmeticke opreacie
  APPEND (WriteInst("EXIT", 1, Exit_Type));
  APPEND (WriteInst("LABEL", 1, appendID(op1Int, tc_id)));
  APPEND (WriteInst("JUMPIFEQ", 3, appendID((op == '/') ? divInt : typeOk, tc_id), GF_op2_type, IntType));
  APPEND (WriteInst("JUMPIFEQ", 3, appendID(retypeOp1, tc_id), GF_op2_type, FloatType));
  APPEND (WriteInst("EXIT", 1, Exit_Type));
  APPEND (WriteInst("LABEL", 1, appendID(retypeOp1, tc_id)));
  APPEND (WriteInst("INT2FLOAT", 2, GF_op1, GF_op1));
  APPEND (WriteInst("JUMP", 1, appendID((op == '/') ? divFloat : typeOk, tc_id)));
  APPEND (WriteInst("LABEL", 1, appendID(op2Int, tc_id)));
  APPEND (WriteInst("JUMPIFEQ", 3, appendID((op == '/') ? divInt : typeOk, tc_id), GF_op1_type, IntType));
  APPEND (WriteInst("JUMPIFEQ", 3, appendID(retypeOp2, tc_id), GF_op1_type, FloatType));
  APPEND (WriteInst("EXIT", 1, Exit_Type));
  APPEND (WriteInst("LABEL", 1, appendID(retypeOp2, tc_id)));
  APPEND (WriteInst("INT2FLOAT", 2, GF_op2, GF_op2));
  APPEND (WriteInst("JUMP", 1, appendID((op == '/') ? divFloat : typeOk, tc_id)));
  APPEND (WriteInst("LABEL", 1, appendID(op1Float, tc_id)));
  APPEND (WriteInst("JUMPIFEQ", 3, appendID((op == '/') ? divFloat : typeOk, tc_id), GF_op2_type, FloatType));
  APPEND (WriteInst("JUMPIFEQ", 3, appendID(retypeOp2, tc_id), GF_op2_type, IntType));
  APPEND (WriteInst("EXIT", 1, Exit_Type));
  APPEND (WriteInst("LABEL", 1, appendID(op2Float, tc_id)));
  APPEND (WriteInst("JUMPIFEQ", 3, appendID((op == '/') ? divFloat : typeOk, tc_id), GF_op1_type, FloatType));
  APPEND (WriteInst("JUMPIFEQ", 3, appendID(retypeOp1, tc_id), GF_op1_type, IntType));
  APPEND (WriteInst("EXIT", 1, Exit_Type));

  if (op == '/') {
    APPEND (WriteInst("LABEL", 1, appendID(divFloat, tc_id)));
    APPEND (WriteInst("JUMPIFEQ", 3, appendID(divZero, tc_id), GF_op2, ZeroFloat));

    APPEND (WriteInst("PUSHS", 1, GF_op1));
    APPEND (WriteInst("PUSHS", 1, GF_op2));
    APPEND (WriteInst("DIVS", 0));
    APPEND (WriteInst("JUMP", 1, appendID(divOk, tc_id)));

    APPEND (WriteInst("LABEL", 1, appendID(divInt, tc_id)));
    APPEND (WriteInst("JUMPIFEQ", 3, appendID(divZero, tc_id), GF_op2, ZeroInt));

    APPEND (WriteInst("PUSHS", 1, GF_op1));
    APPEND (WriteInst("PUSHS", 1, GF_op2));
    APPEND (WriteInst("IDIVS", 0));
    APPEND (WriteInst("JUMP", 1, appendID(divOk, tc_id)));

    APPEND (WriteInst("LABEL", 1, appendID(divZero, tc_id)));
    APPEND (WriteInst("EXIT", 1, Exit_Zero));

    APPEND (WriteInst("LABEL", 1, appendID(divOk, tc_id)));

    tc_id ++;
    return;
  }

  if (op == '+')
  {
    APPEND (WriteInst("LABEL", 1, appendID(op1Str, tc_id)));
    APPEND (WriteInst("JUMPIFEQ", 3, appendID(opConcat, tc_id), GF_op2_type, StrType));
    APPEND (WriteInst("EXIT", 1, Exit_Type));
    APPEND (WriteInst("LABEL", 1, appendID(op2Str, tc_id)));
    APPEND (WriteInst("JUMPIFEQ", 3, appendID(opConcat, tc_id), GF_op1_type, StrType));
    APPEND (WriteInst("EXIT", 1, Exit_Type));

    APPEND (WriteInst("LABEL", 1, appendID(typeOk, tc_id)));

    APPEND (WriteInst("PUSHS", 1, GF_op1));
    APPEND (WriteInst("PUSHS", 1, GF_op2));
    APPEND (WriteInst("ADDS", 0));

    APPEND (WriteInst("JUMP", 1, appendID(notStr, tc_id)));
    APPEND (WriteInst("LABEL", 1, appendID(opConcat, tc_id)));
    APPEND (WriteInst("CONCAT", 3, GF_op1, GF_op1, GF_op2));
    APPEND (WriteInst("PUSHS", 1, GF_op1));
    APPEND (WriteInst("LABEL", 1, appendID(notStr, tc_id)));

    tc_id ++;
    return;
  }

  if (op == '>' || op == '<' || op == 'L' || op == 'G' || op == 'Q' || op == 'N')
  {
    APPEND (WriteInst("LABEL", 1, appendID(op1Str, tc_id)));
    APPEND (WriteInst("JUMPIFEQ", 3, appendID(typeOk, tc_id), GF_op2_type, StrType));
    APPEND (WriteInst("EXIT", 1, Exit_Type));
    APPEND (WriteInst("LABEL", 1, appendID(op2Str, tc_id)));
    APPEND (WriteInst("JUMPIFEQ", 3, appendID(typeOk, tc_id), GF_op1_type, StrType));
    APPEND (WriteInst("EXIT", 1, Exit_Type));
  }

  APPEND (WriteInst("LABEL", 1, appendID(typeOk, tc_id)));

  switch (op) {
    case '-':
    APPEND (WriteInst("PUSHS", 1, GF_op1));
    APPEND (WriteInst("PUSHS", 1, GF_op2));
    APPEND (WriteInst("SUBS", 0));
    break;

    case '*':
    APPEND (WriteInst("PUSHS", 1, GF_op1));
    APPEND (WriteInst("PUSHS", 1, GF_op2));
    APPEND (WriteInst("MULS", 0));
    break;

    case '>':
    APPEND (WriteInst("PUSHS", 1, GF_op1));
    APPEND (WriteInst("PUSHS", 1, GF_op2));
    APPEND (WriteInst("GTS", 0));
    break;

    case '<':
    APPEND (WriteInst("PUSHS", 1, GF_op1));
    APPEND (WriteInst("PUSHS", 1, GF_op2));
    APPEND (WriteInst("LTS", 0));
    break;

    case 'L':
    APPEND (WriteInst("PUSHS", 1, GF_op1));
    APPEND (WriteInst("PUSHS", 1, GF_op2));
    APPEND (WriteInst("LTS", 0));

    APPEND (WriteInst("PUSHS", 1, GF_op1));
    APPEND (WriteInst("PUSHS", 1, GF_op2));
    APPEND (WriteInst("EQS", 0));
    APPEND (WriteInst("ORS", 0));
    break;

    case 'G':
    APPEND (WriteInst("PUSHS", 1, GF_op1));
    APPEND (WriteInst("PUSHS", 1, GF_op2));
    APPEND (WriteInst("GTS", 0));

    APPEND (WriteInst("PUSHS", 1, GF_op1));
    APPEND (WriteInst("PUSHS", 1, GF_op2));
    APPEND (WriteInst("EQS", 0));
    APPEND (WriteInst("ORS", 0));
    break;

    case 'E':
    APPEND (WriteInst("PUSHS", 1, GF_op1));
    APPEND (WriteInst("PUSHS", 1, GF_op2));
    APPEND (WriteInst("EQS", 0));
    break;

    case 'N':
    APPEND (WriteInst("PUSHS", 1, GF_op1));
    APPEND (WriteInst("PUSHS", 1, GF_op2));
    APPEND (WriteInst("EQS", 0));
    APPEND (WriteInst("NOTS", 0));
    break;
  }

  tc_id ++;
}

void Gen_EvalExpr()
{
  APPEND (WriteInst("POPS", 1, GF_retval));
}

void Gen_DefineVar(char * var, unsigned paramId)
{
  if (paramId) {
    APPEND (WriteInst("DEFVAR", 1, appendID(Par, paramId)));
    return;
  }
  char * prefix = "LF@";
  char * symb = malloc(sizeof(char) * (strlen(prefix) + strlen(var) + 1));
  strcpy(symb, prefix);
  strcat(symb, var);
  APPEND (WriteInst("DEFVAR", 1, symb));
  free(symb);
}

void Gen_AssignTo(char * var, unsigned paramId)
{
  if (paramId) {
    APPEND (WriteInst("MOVE", 2, appendID(Par, paramId), GF_retval));
    return;
  }
  char * prefix = "LF@";
  char * symb = malloc(sizeof(char) * (strlen(prefix) + strlen(var) + 1));
  if (symb == NULL) {
    Error = 99;
    return;
  }
  strcpy(symb, prefix);
  strcat(symb, var);
  APPEND (WriteInst("MOVE", 2, symb, GF_retval));
  free(symb);
}

void Gen_If()
{
  APPEND (WriteInst("JUMPIFNEQ", 3, appendID(ElseSt, ifelse_id), GF_retval, ExprTrue));
}

void Gen_Else()
{
  APPEND (WriteInst("JUMP", 1, appendID(EndIf, ifelse_id)));
  APPEND (WriteInst("LABEL", 1, appendID(ElseSt, ifelse_id)));
}

void Gen_EndIf()
{
  APPEND (WriteInst("LABEL", 1, appendID(EndIf, ifelse_id)));
  ifelse_id ++;
}

void MostTop_While()
{
  Compiled.PreWhile = Compiled.last;
}

void Gen_While_cond()
{
  APPEND (WriteInst("LABEL", 1, appendID(whileCond, while_id)));
}

void Gen_While_begin()
{
  APPEND (WriteInst("JUMPIFNEQ", 3, appendID(whileEnd, while_id), GF_retval, ExprTrue));
}

void Gen_PreWhile_Def(char * var, unsigned paramId)
{
  if (paramId) {
    APPEND (WriteInst("DEFVAR", 1, appendID(Par, paramId)));
    return;
  }
  char * prefix = "LF@";
  char * symb = malloc(sizeof(char) * (strlen(prefix) + strlen(var) + 1));
  if (symb == NULL) {
    Error = 99;
    return;
  }
  strcpy(symb, prefix);
  strcat(symb, var);
  if (list_insert_PreWhile (& Compiled, WriteInst("DEFVAR", 1, symb)) == 99) {
    Error = 99;
  }
  free(symb);
}

void Gen_While_end()
{
  APPEND (WriteInst("JUMP", 1, appendID(whileCond, ifelse_id)));
  APPEND (WriteInst("LABEL", 1, appendID(whileEnd, while_id)));
  while_id++;
}

void Gen_Func_PrepFrame()
{
  APPEND (WriteInst("CREATEFRAME", 0));
  param_id = 1;
}

void Gen_Push_Param(tToken * token, unsigned paramId)
{
  char * param = appendID(PrepPar, param_id);
  APPEND (WriteInst("DEFVAR", 1, param));
  APPEND (WriteInst("MOVE", 2, param, tok2symb(token)));

  param_id ++;
}

void Gen_Call_Func(char * func)
{
  APPEND (WriteInst("PUSHFRAME", 0));
  APPEND (WriteInst("CALL", 1, func));
}

void Gen_Call_Print(int n)
{
  char buff[30];
  sprintf(buff, "int@%d", n);
  APPEND (WriteInst("MOVE", 2, GF_printParams, Const2Dynamic(buff)));
  APPEND (WriteInst("POPFRAME", 0));
  APPEND (WriteInst("PUSHFRAME", 0));
  APPEND (WriteInst("CALL", 1, "print")); 
}

void Gen_FuncDef_begin(char * func)
{
  APPEND (WriteInst("JUMP", 1, appendID(overf, overf_id)));
  APPEND (WriteInst("LABEL", 1, func));
}

void Gen_FuncDef_end()
{
  APPEND (WriteInst("POPFRAME", 0));
  APPEND (WriteInst("RETURN", 0));
  APPEND (WriteInst("LABEL", 1, appendID(overf, overf_id)));
  overf_id++;
}

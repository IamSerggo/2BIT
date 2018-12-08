#include <ctype.h>
#include <limits.h>
#include <float.h>
#include <errno.h>
#include "scanner.h"

#define STR_INIT_CAP 10

/* pomocna funkcia prida znak do dynamicky alokovaneho retazca */
static inline void AddCharToDStr(char ** dynamicStr, unsigned * StrSize, unsigned * StrCap, char c) {

  if (*dynamicStr == NULL) { // interna chyba prekladaca
    Error = 99;
    return;
  }

  (*dynamicStr)[*StrSize] = c;

  if ((*StrSize)++ == *StrCap) {
    *StrCap *= 2; // zdvojnasobi velkost
    *dynamicStr = realloc(*dynamicStr, sizeof(char) * (*StrCap));
  }
}


/* makro pre pridavanie escape seqvencie */
#define APPEND_ESC_SEQ(DStr, StrSize, StrCap, escBuff) \
  AddCharToDStr(DStr, StrSize, StrCap, escBuff[0]); \
  AddCharToDStr(DStr, StrSize, StrCap, escBuff[1]); \
  AddCharToDStr(DStr, StrSize, StrCap, escBuff[2]); \
  AddCharToDStr(DStr, StrSize, StrCap, escBuff[3]);


/* pomocna funkcia zistujuca ci je retazec keyword */
int IsKeyword(char * str) {
  if (strcmp(str, "def") == 0)
    return KW_DEF;

  else if (strcmp(str, "do") == 0)
    return KW_DO;

  else if (strcmp(str, "else") == 0)
    return KW_ELSE;

  else if (strcmp(str, "end") == 0)
    return KW_END;

  else if (strcmp(str, "if") == 0)
    return KW_IF;

  else if (strcmp(str, "not") == 0)
    return KW_NOT;

  else if (strcmp(str, "nil") == 0)
    return KW_NIL;

  else if (strcmp(str, "then") == 0)
    return KW_THEN;

  else if (strcmp(str, "while") == 0)
    return KW_WHILE;

  /* nieje keyword */
  return -1;
}


/* pomocna funkcia pre bezpecnu konverziu na int, pri overflow vracia 1  */
int SafeToInt(int *dest, char * digits, int base) {

  int MulMax = INT_MAX / base; // vacsia_hodnota * base = overflow
  int digitVal;

  while (*digits != '\0') {
    if (base == 16)
      digitVal = (isdigit(*digits) ? *digits - '0' : toupper(*digits) - 'A' + 10);
    else
      digitVal = *digits - '0';

    if (*dest > MulMax)
      return 1;

    *dest *= base;
    if ((*dest += digitVal) < 0)
      return 1;

    digits ++;
  }
  return 0; // nedoslo k overflow
}


/* pomocna funkcia pre blokovy koment, vracia false pri lexikalnej chybe */
bool CheckBlockComment(int * c) {
  char buff[6];
  int i;

  for (i = 0; i < 5; i++) { // begin
    *c = getchar();
    if (*c == EOF)
      break;

    buff[i] = *c;
  }
  buff[i] = '\0';

  if (strcmp(buff, "begin") == 0) {
    while (true) {
      *c = getchar();

      if (*c == '\n') {
        if ((*c = getchar()) == '=') {
          for (i = 0; i < 3; i++) {
            if ((*c = getchar()) == EOF)
              break;

            buff[i] = *c;
          }
          buff[3] = '\0';

          if (strcmp(buff, "end") == 0) {
            *c = getchar();

            if (isspace(*c) || *c == EOF) {
              while (*c != EOL && *c != EOF)
                *c = getchar();

              return true;
            }
            else {
              return false;
            }
          }
        }
      }

      if (*c == EOF) {
        return false;
      }
    }
  }
  else {
    while (--i >= 0) {
      ungetc(buff[i], stdin);
    }
    *c = '=';
  }

  return true;
}

/* -------------------------------- PUBLIC ---------------------------------- */

int NextToken(tToken ** token, int c) {

  *token = malloc(sizeof(*token));
  if (*token == NULL) {
    return EOF;
  }

  /* START */
  (*token)->name = START;

  if (c == BOF ) {
    c = getchar();
    if (c == '=') {
      if (! CheckBlockComment(& c)) { // blokove komenty
        free(*token);
        Error = 1;
        return EOF;
      }
    }
  }

  if (c == '\n') {
    c = getchar();
    if (c == '=') {
      if (! CheckBlockComment(& c)) { // blokove komenty
        free(*token);
        Error = 1;
        return EOF;
      }
    }
    (*token)->name = EOL;
    return c;
  }

  while (isspace(c)) { // pozerie biele znaky
    if (c == '\n') {
      (*token)->name = EOL;
      return getchar();
    }
    c = getchar();
  }

  if (c == EOF) { // koniec vstupu
    (*token)->name = EOL;
    return EOF;
  }

  /* jednoduche stavy bez citania retazcov */

  switch (c) {
    case '+':
      (*token)->name = PLUS;
      return getchar();

    case '-':
      (*token)->name = MINUS;
      return getchar();

    case '*':
      (*token)->name = STAR;
      return getchar();

    case '/':
      (*token)->name = SLASH;
      return getchar();

    case '(':
      (*token)->name = LEFT_PAR;
      return getchar();

    case ')':
      (*token)->name = RIGHT_PAR;
      return getchar();

    case ',':
      (*token)->name = COMMA;
      return getchar();

    case '=':
      (*token)->name = ASSIGN;
      if ((c = getchar()) == '=') {
        (*token)->name = EQ;
        return getchar();
      }
      return c;

    case '<':
      (*token)->name = LT;
      if ((c = getchar()) == '=') {
        (*token)->name = LE;
        return getchar();
      }
      return c;

    case '>':
      (*token)->name = GT;
      if ((c = getchar()) == '=') {
        (*token)->name = GE;
        return getchar();
      }
      return c;

    case '!':
      /* NOT */
      if ((c = getchar()) != '=') { // koniec v nekoncovom stave
        free(*token);
        Error = 1; // lexikalna chyba
        return EOF;
      }
      (*token)->name = NE;
      return getchar();

    case '#':
      /* HASHTAG (jednoriadkovy koment) */
      do {
        c = getchar();
      } while(c != '\n' && c != EOF);
      (*token)->name = EOL;
      return getchar();

  } // koniec switchu


  /* stavy s citanim retazcov */

  char * strPtr;                   //  ukazatel na dynamicky alokovany retazec
  unsigned StrSize = 0;            //  velkost dynamicky alokovaneho retazca strPtr
  unsigned StrCap = STR_INIT_CAP;  // kapacita dynamicky alokovaneho retazca strPtr

  if ((strPtr = malloc(sizeof(char) * StrCap)) == NULL) { // interna chyba
    Error = 99;
    free(*token);
    return EOF;
  }

  /* citanie String literal */

  if (c == '\"') {

    /* stav STR_L */
    c = getchar();

    do {
      if (c < 32 || c == '\n') { // koniec v nekoncovom stave
        free(strPtr);
        free(*token);
        Error = 1; // lexikalna chyba
        return EOF;
      }

      if (c == '\\') {
        /* STR_L_ESCAPE */

        switch (c = getchar()) {
          case '\"':
            APPEND_ESC_SEQ(& strPtr, & StrSize, & StrCap, "\\034");
            break;
          case '\\':
            APPEND_ESC_SEQ(& strPtr, & StrSize, & StrCap, "\\092");
            break;
          case 'n':
            APPEND_ESC_SEQ(& strPtr, & StrSize, & StrCap, "\\010");
            break;
          case 't':
            APPEND_ESC_SEQ(& strPtr, & StrSize, & StrCap, "\\009");
            break;
          case 's':
            APPEND_ESC_SEQ(& strPtr, & StrSize, & StrCap, "\\032");
            break;
          case 'x':
            /* ESC_HEX_FIRST */
            if (! isxdigit(c = getchar())) { // koniec v nekoncovom stave
              free(strPtr);
              free(*token);
              Error = 1; // lexikalna chyba
              return EOF;
            }
            unsigned char EscVal = (isdigit(c) ? c - '0' : toupper(c) - 'A' + 10); // povinny prvy znak

            /* ESC_HEX_SECOND */
            if (isxdigit(c = getchar())) {
              EscVal = EscVal * 16 + (isdigit(c) ? c - '0' : toupper(c) - 'A' + 10); // nepovinny druhy znak
            }
            else if (c < 32 || c == '\n') { // koniec v nekoncovom stave
              free(strPtr);
              free(*token);
              Error = 1; // lexikalna chyba
              return EOF;
            }
            else // uz bol prijaty dalsi znak za escape sekvenciou
              ungetc(c, stdin);

            /* konverzia EscVal na znaky do EscBuff */
            char EscBuff[4];
            EscBuff[3] = EscVal % 10 + '0'; EscVal /= 10;
            EscBuff[2] = EscVal % 10 + '0'; EscVal /= 10;
            EscBuff[1] = EscVal % 10 + '0';
            EscBuff[0] = '\\';

            APPEND_ESC_SEQ(& strPtr, & StrSize, & StrCap, EscBuff);
            break;

          default: // koniec v nekoncovom stave
            free(strPtr);
            free(*token);
            Error = 1; // lexikalna chyba
            return EOF;
        }
      } // escape sekvencia uspesne precitana

      else if (c == '#') {
        APPEND_ESC_SEQ(& strPtr, & StrSize, & StrCap, "\\035");
      }
      else if (c == ' ') { // whitespace nad 31
        APPEND_ESC_SEQ(& strPtr, & StrSize, & StrCap, "\\032");
      }
      else {
        /* prida dalsi znak do retazca */
        AddCharToDStr(& strPtr, & StrSize, & StrCap, c);
      }

      if (Error == 99) {return EOF; free(*token);}

      c = getchar(); // cita dalsi znak

    } while (c != '\"'); // KONIEC cyklu

    AddCharToDStr(& strPtr, & StrSize, & StrCap, '\0'); // prida null
    if (Error == 99) {return EOF; free(*token);}

    (*token)->name = STRING_L;
    (*token)->data.String = strPtr;

    return getchar();
  }


  /* citanie Integer / Float literal */

  int base; // pre bonus BASE, uchovava zaklad sustavy cisla

  if (c == '0') {

    /* stav ZERO */

    (*token)->name = INTEGER_L; // datovy typ Integer
    (*token)->data.Integer = 0;

    AddCharToDStr(& strPtr, & StrSize, & StrCap, c);
    if (Error == 99) {return EOF; free(*token);}
    c = getchar();

    if (c != '.') {
      /* bonusova cast BASE */

      if (c == 'b') {
        /* FS_INT_BIN_L */
        base = 2;

        do { // pozerie nuly na zaciatku
          c = getchar();
        } while (c == '0');

        while (c == '0' || c == '1') {
          AddCharToDStr(& strPtr, & StrSize, & StrCap, c);
          if (Error == 99) {return EOF; free(*token);}
          c = getchar();
        }
      }
      else if (c == '\\') {
        /* INT_HEX_L */
        if ((c = getchar()) == 'x') {
          /* FS_INT_HEX_L */
          base = 16;

          do { // pozerie nuly na zaciatku
            c = getchar();
          } while (c == '0');

          while (isxdigit(c)) {
            AddCharToDStr(& strPtr, & StrSize, & StrCap, c);
            if (Error == 99) {return EOF; free(*token);}
            c = getchar();
          }
        }
      }
      else {
        /* INT_OCT_L */
        base = 8;

        while (c == '0') { // pozerie nuly na zaciatku
          c = getchar();
        }

        while (c >= '0' && c <= '8') {
          AddCharToDStr(& strPtr, & StrSize, & StrCap, c);
          if (Error == 99) {return EOF; free(*token);}
          c = getchar();
        }
      }
    }
    else {
      // inak c == '.' -> prechod do FLOAT_DOT
      (*token)->name = FLOAT_L;
    }
  }
  else if (c >= '1' && c <= '9') {

    /* stav INT_L */

    base = 10;
    (*token)->name = INTEGER_L; // datovy typ Integer
    (*token)->data.Integer = 0;

    do {
      AddCharToDStr(& strPtr, & StrSize, & StrCap, c);
      if (Error == 99) {return EOF; free(*token);}

    } while (isdigit(c = getchar()));

    if (c == '.')
      (*token)->name = FLOAT_L; // prechod do FLOAT_DOT
  }

  if ((*token)->name == INTEGER_L) {

    /* konvertuje na int */
    AddCharToDStr(& strPtr, & StrSize, & StrCap, '\0');
    if (Error == 99) {return EOF; free(*token);}

    if (SafeToInt(& (*token)->data.Integer, strPtr, base)) { // overflow
      free(strPtr); // uvolni retazec Integeru
      free(*token);
      Error = 1; // lexikalna chyba
      return EOF;
    }
    //printf("%s\n", strPtr);
    free(strPtr); // uvolni retazec Integeru

    return c; // nasledujuci znak
  }
  else if ((*token)->name == FLOAT_L) {

    /* stav FLOAT_DOT */

    AddCharToDStr(& strPtr, & StrSize, & StrCap, '.');
    if (Error == 99) {return EOF; free(*token);}

    if (! isdigit(c = getchar())) { // koniec v nekoncovom stave
      free(strPtr);
      free(*token);
      Error = 1; // lexikalna chyba
      return EOF;
    }

    /* stav FLOAT_L */

    do {
      AddCharToDStr(& strPtr, & StrSize, & StrCap, c);
      if (Error == 99) {return EOF; free(*token);}

    } while (isdigit(c = getchar()));

    if (c == 'e' || c == 'E') {
      /* stav FLOAT_EXP_L */

      AddCharToDStr(& strPtr, & StrSize, & StrCap, c);
      if (Error == 99) {return EOF; free(*token);}

      c = getchar();

      if (c == '+' || c == '-') {
        AddCharToDStr(& strPtr, & StrSize, & StrCap, c);
        if (Error == 99) {return EOF; free(*token);}
        c = getchar();
      }

      if (! isdigit(c)) { // koniec v nekoncovom stave
        free(strPtr);
        free(*token);
        Error = 1; // lexikalna chyba
        return EOF;
      }

      /* stav FS_FLOAT_EXP_L */

      while (c == '0') { // pozerie pociatocne nuly
        c = getchar();
      }

      while(isdigit(c)) {
        AddCharToDStr(& strPtr, & StrSize, & StrCap, c);
        if (Error == 99) {return EOF; free(*token);}
        c = getchar();
      }
    }

    /* konvertuje na double */
    (*token)->data.Float = strtod(strPtr, NULL);
    if (errno == ERANGE) { // overflow
      free(strPtr); // uvolni retazec Floatu
      free(*token);
      Error = 1; // lexikalna chyba
      return EOF;
    }

    free(strPtr); // uvolni retazec Floatu
    return c;     // nasledujuci znak
  }


  /* citanie identifikatoru */

  if (islower(c)) {
    do {
      /* stav ID_LOW */

      AddCharToDStr(& strPtr, & StrSize, & StrCap, c);
      if (Error == 99) {return EOF; free(*token);}

    } while (islower(c = getchar()));

    if (c == '!' || c == '?') {
      (*token)->name = ID_FUNC;   // prechod do ID_FUNC;
    }
    else {
      (*token)->name = ID;
    }
  }

  if (c == '_' || ((*token)->name == ID && (isupper(c) || isdigit(c)))) {

    /* stav ID */

    do {
      AddCharToDStr(& strPtr, & StrSize, & StrCap, c);
      if (Error == 99) {return EOF; free(*token);}

      c = getchar();

    } while (isalnum(c) || c == '_');


    if (c == '!' || c == '?') {
      (*token)->name = ID_FUNC;   // prechod do ID_FUNC
    }
    else {
      (*token)->name = ID;
      AddCharToDStr(& strPtr, & StrSize, & StrCap, '\0'); // prida null
      if (Error == 99) {return EOF; free(*token);}
      (*token)->data.String = strPtr;
      return c;
    }
  }
  else if ((*token)->name == ID) {

    /* poznamka: strlen (keywords) < STR_INIT_CAP */
    AddCharToDStr(& strPtr, & StrSize, & StrCap, '\0'); // prida null
    if (Error == 99) {return EOF; free(*token);}

    int keyword = IsKeyword(strPtr);
    if (keyword != -1) {

      /* stav KEYWORD */
      (*token)->name = keyword;
    }
    else {
      (*token)->data.String = strPtr;
    }

    return c; // nasledujuci znak
  }


  if ((*token)->name == ID_FUNC) {
    int c_next = getchar();
    if (c == '!' && c_next == '=') {
      (*token)->name = ID;
      ungetc(c_next, stdin);
    }
    else {
      AddCharToDStr(& strPtr, & StrSize, & StrCap, c);
      c = c_next;
    }

    AddCharToDStr(& strPtr, & StrSize, & StrCap, '\0'); // prida null
    if (Error == 99) {return EOF; free(*token);}
    (*token)->data.String = strPtr;
    return c;
  }

  // CHYBA, nebol precitany platny lexem
  free(*token);
  Error = 1;
  return EOF;
}

/*****************************************************************\
|	Project: Implementace překladače imperativního jazyka IFJ18     |
|																                                  |
|	Authors: Radovan Babic, xbabic09                                |
|          Daniela Peskova, xpesko00                              |
|          Adam Kucera, xkucer95                                  |
|          Filip Dianiska, xdiani00                               |
|                                                                 |
|	File: parser.c                                                  |
\*****************************************************************/

#include "parser.h"
#include "generator.h"
#include "linked_list.h"

int STATEMENT_PROCESSED = 0;
int PARAMS_IN_DEFINITION = 0;
int POTENTIAL_PARAMS = 0;
int while_counter = 0;
bool in_while = false;
bool is_print = false;
int print_params = 0;
unsigned id_of_param = 0;

tBTNodePtr *SymTablePtr;
tToken *TempToken;

//pomocna funkcia, idealne by bolo, keby isla dopice
void copy_token(tToken ** T){
    (*T)->name = START;
}

void tcheck(tToken ** T){
    if ((*T)->name == ID) printf("ID\n");
    if ((*T)->name == ID_FUNC) printf("ID_FUNC\n");
    if ((*T)->name == KW_DEF) printf("DEF\n");
    if ((*T)->name == KW_END) printf("END\n");
    if ((*T)->name == KW_BEGIN) printf("BEGIN\n");
    if ((*T)->name == KW_WHILE) printf("WHILE\n");
    if ((*T)->name == KW_DO) printf("DO\n");
    if ((*T)->name == KW_IF) printf("IF\n");
    if ((*T)->name == KW_THEN) printf("THEN\n");
    if ((*T)->name == KW_ELSE) printf("ELSE\n");
    if ((*T)->name == EQ) printf("EQ\n");
    if ((*T)->name == ASSIGN) printf("=\n");
    if ((*T)->name == LEFT_PAR) printf("LEFT_PAR\n");
    if ((*T)->name == RIGHT_PAR) printf("RIGHT_PAR\n");
    if ((*T)->name == EOL) printf("EOL\n");
    if ((*T)->name == STRING_L) printf("STRING_L\n");
    if ((*T)->name == INTEGER_L) printf("INTEGER_L\n");
    if ((*T)->name == FLOAT_L) printf("FLOAT_L\n");
    if ((*T)->name == COMMA) printf("COMMA\n");
    if ((*T)->name == START) printf("START\n");
}


int _STMNT(int curr_char, tTokenName tName){
    int tmp = curr_char;

    while (STATEMENT_PROCESSED != 1) {
        if (tmp == EOF) Error = EOF;
        err_check(Error);
        tmp = _PROG(tmp, tName);
    }

    STATEMENT_PROCESSED = 0;
    err_check(Error);
    return tmp;
}

int _PARAMS_TYPE_CHECK(tToken ** Token){
    switch ((*Token)->name) {
        case ID:
        case INTEGER_L:
        case FLOAT_L:
        case STRING_L:
        case KW_NIL:
            return 1;

        default:
            return 0;
    }
}

int _PARAMS_NO_PARS(int curr_char, unsigned num_of_params){
    tToken *T;
    tBTNodePtr helper;

    int params_count = 0;
    int tmp = NextToken(&T, curr_char);
    err_check(Error);

    if (tmp == EOF) {
        if (num_of_params == 0 || num_of_params == (FREE_NUM_OF_PARAMS-1)) return EOF;
        else Error = PARAMS_COUNT_ERR;

        err_check(Error);
    }

    if (CHECKING_LOCAL_FRAME) SymTablePtr = &LocalFramePtr;
    else SymTablePtr = &RootPtr;

    if (T->name == COMMA){

        tmp = NextToken(&T, tmp);
        if (tmp == EOF && !Error) Error = EOF;
        err_check(Error);

        switch (T->name){
            case KW_NIL :
            case FLOAT_L :
            case INTEGER_L :
            case STRING_L :
            case ID :
                while (T->name != EOL){
                    if ( _PARAMS_TYPE_CHECK(&T) ){
                        if (T->name == ID){
                            helper = IsDefined(SymTablePtr, T->data.String);

                            if (helper == NULL) Error = DEFINITION_ERR;
                            else if (helper->type == ID_FUNC) Error = SEMANTIC_ERR;
                            err_check(Error);
                        }

                        if (is_print) {
                            Gen_Push2DataStack(T, id_of_param++);
                            print_params++;
                        }

                        else Gen_Push_Param(T, id_of_param++);

                        params_count += 1;

                        tmp = NextToken(&T, tmp);
                        err_check(Error);

                        if (T->name == COMMA || T->name == EOL) continue;
                        else{
                            Error = SYNTAX_ERR;
                            break;
                        }
                    }
                    if (T->name == COMMA){
                        tmp = NextToken(&T, tmp);
                        if (tmp == EOF && !Error) Error = SYNTAX_ERR;
                        err_check(Error);

                        if ( _PARAMS_TYPE_CHECK(&T) ) continue;
                        else{
                            Error = SYNTAX_ERR;
                            break;
                        }
                    }
                }
                break;
            default:
                Error = SYNTAX_ERR;
                break;
        }
    }
    else if (T->name == EOL) {
        if (params_count == num_of_params || num_of_params == (FREE_NUM_OF_PARAMS-1)) return tmp;
        else Error = PARAMS_COUNT_ERR;
    }
    else Error = SYNTAX_ERR;

    if (!Error && num_of_params != (FREE_NUM_OF_PARAMS-1)){
        if (params_count != num_of_params) Error = PARAMS_COUNT_ERR;
    }

    err_check(Error);
    return tmp;
}

int _PARAMS_IN_PARS(int curr_char, unsigned num_of_params){
    tToken *T;
    tBTNodePtr helper;
    int params_count = 0;

    int tmp = NextToken(&T, curr_char);
    if (tmp == EOF && !Error) Error = EOF;

    err_check(Error);

    switch (T->name){
        case RIGHT_PAR:
            if (num_of_params == 0) return tmp;
            else Error = PARAMS_COUNT_ERR;
            break;

        case ID:
        case STRING_L:
        case INTEGER_L:
        case FLOAT_L:
        case KW_NIL:
            while (T->name != RIGHT_PAR){
                if (T->name == COMMA) {
                    tmp = NextToken(&T, tmp);
                    if (tmp == EOF && !Error) Error = EOF;

                    err_check(Error);

                    if ( _PARAMS_TYPE_CHECK(&T) ) continue;
                    else {
                        Error = SYNTAX_ERR;
                        return tmp;
                    }
                }

                if ( _PARAMS_TYPE_CHECK(&T) ) {
                    if (T->name == ID){
                        helper = IsDefined(SymTablePtr, T->data.String);

                        if (helper == NULL) Error = DEFINITION_ERR;
                        else if (helper->type == ID_FUNC) Error = SEMANTIC_ERR;
                        err_check(Error);
                    }


                    if (is_print) {
                        Gen_Push2DataStack(T, id_of_param++);
                        print_params++;
                    }

                    else Gen_Push_Param(T, id_of_param++);

                    params_count += 1;

                    tmp = NextToken(&T, tmp);
                    if (tmp == EOF && !Error) Error = EOF;

                    err_check(Error);

                    if (T->name == COMMA || T->name == RIGHT_PAR) continue;

                    else {
                        Error = SYNTAX_ERR;
                        return tmp;
                    }
                }
            }
            break;

        default:
            Error = SYNTAX_ERR;
            break;
    }

    if (!Error && num_of_params != FREE_NUM_OF_PARAMS){
        if (params_count != num_of_params) Error = PARAMS_COUNT_ERR;
    }

    err_check(Error);
    return tmp;
}

int _DEF_PARAMS(int curr_char){
    tToken *T;

    unsigned param_id = 1;

    int tmp = NextToken(&T, curr_char);

    if (T->name == KW_NIL) Error = PARAMS_COUNT_ERR;

    if (tmp == EOF && !Error) Error = EOF;

    err_check(Error);

    if (T->name == RIGHT_PAR) {
        PARAMS_IN_DEFINITION = 0;
        return tmp;
    }
    else if (T->name == ID){
        while (T->name != RIGHT_PAR){

            if (T->name == COMMA) {
                tmp = NextToken(&T, tmp);
                if (T->name == KW_NIL) Error = PARAMS_COUNT_ERR;
                if (tmp == EOF && !Error) Error = EOF;

                err_check(Error);

                if (T->name == ID) continue;
                else {
                    Error = SYNTAX_ERR;
                    return tmp;
                }
            }

            if (T->name == ID) {
                PARAMS_IN_DEFINITION += 1;
                Define(&LocalFramePtr, T->data.String, ID, 0, param_id);

                Gen_DefineVar(T->data.String, param_id++);

                tmp = NextToken(&T, tmp);
                if (T->name == KW_NIL) Error = PARAMS_COUNT_ERR;
                if (tmp == EOF && !Error) Error = EOF;

                err_check(Error);

                if (T->name == COMMA || T->name == RIGHT_PAR) {
                    continue;
                }
                else {
                    Error = SYNTAX_ERR;
                    return tmp;
                }
            }
            else {
                Error = SYNTAX_ERR;
                return tmp;
            }
        }
    }
    else {
        Error = SYNTAX_ERR;
        return tmp;
    }

    err_check(Error);
    return tmp;
}

int _PARAMS(int curr_char, unsigned num_of_params, bool is_def){
    tToken *T;
    tBTNodePtr helper;

    int tmp = NextToken(&T, curr_char);

    err_check(Error);

    //if (tmp == EOF) Error = EOF;

    if (CHECKING_LOCAL_FRAME) SymTablePtr = &LocalFramePtr;
    else SymTablePtr = &RootPtr;

    id_of_param = 1;

    if (is_def){
        if (T->name == LEFT_PAR) tmp = _DEF_PARAMS(tmp);
        else {
            Error = SYNTAX_ERR;
            return tmp;
        }
    }
    else {

        if (T->name == LEFT_PAR) tmp = _PARAMS_IN_PARS(tmp, num_of_params);

        else {
            switch (T->name){
                case ID :
                    helper = IsDefined(SymTablePtr, T->data.String);
                    if (helper == NULL) Error = DEFINITION_ERR;
                    else if (helper->type == ID_FUNC) Error = SEMANTIC_ERR;


                case FLOAT_L :
                case INTEGER_L :
                case STRING_L :
                case KW_NIL :

                    if (!num_of_params) Error = PARAMS_COUNT_ERR;
                    err_check(Error);

                    Gen_Push_Param(T, id_of_param++);
                    if (is_print) print_params++;

                    tmp = _PARAMS_NO_PARS(tmp, (num_of_params - 1));
                    break;

                case EOL:
                    if (num_of_params == 0) return tmp;
                    else Error = PARAMS_COUNT_ERR;
                    break;

                case ASSIGN:
                    Error = DEFINITION_ERR;
                    break;

                default:
                    Error = SYNTAX_ERR;
                    break;
            }
        }
    }

    id_of_param = 0;
    err_check(Error);
    return tmp;
}

int _POTENTIONAL_FUNC(int curr_char, bool in_pars){
    tToken *T;
    tBTNodePtr helper;
    int tmp;

    if (in_pars){
        tmp = NextToken(&T, curr_char);
        if (tmp == EOF && !Error) Error = EOF;
        err_check(Error);

        switch (T->name){
            case ID :
            case INTEGER_L :
            case FLOAT_L :
            case STRING_L :
                while (T->name != RIGHT_PAR){
                    if (T->name == COMMA) {
                        tmp = NextToken(&T, tmp);
                        if (tmp == EOF && !Error) Error = EOF;

                        err_check(Error);

                        if ( _PARAMS_TYPE_CHECK(&T) ) continue;
                        else {
                            Error = SYNTAX_ERR;
                            return tmp;
                        }
                    }

                    if ( _PARAMS_TYPE_CHECK(&T) ) {
                        if (T->name == ID){
                            helper = IsDefined(&LocalFramePtr, T->data.String);

                            if (helper == NULL) Error = DEFINITION_ERR;
                            else if (helper->type == ID_FUNC) Error = SEMANTIC_ERR;
                            err_check(Error);
                        }

                        POTENTIAL_PARAMS += 1;

                        tmp = NextToken(&T, tmp);
                        if (tmp == EOF && !Error) Error = EOF;

                        err_check(Error);

                        if (T->name == COMMA || T->name == RIGHT_PAR) continue;

                        else {
                            Error = SYNTAX_ERR;
                            break;
                        }
                    }
                }
                break;

            case RIGHT_PAR:
                break;

            default:
                Error = SYNTAX_ERR;
                break;

        }
    }
    else {
        T = extT;
        tmp = curr_char;
        switch (T->name){
            case ID :
            case INTEGER_L :
            case FLOAT_L :
            case STRING_L :
                while (T->name != EOL){
                    if (T->name == COMMA) {
                        tmp = NextToken(&T, tmp);
                        if (tmp == EOF && !Error) Error = EOF;

                        err_check(Error);

                        if ( _PARAMS_TYPE_CHECK(&T) ) continue;
                        else {
                            Error = SYNTAX_ERR;
                            return tmp;
                        }
                    }

                    if ( _PARAMS_TYPE_CHECK(&T) ) {
                        if (T->name == ID){
                            helper = IsDefined(&LocalFramePtr, T->data.String);

                            if (helper == NULL) Error = DEFINITION_ERR;
                            else if (helper->type == ID_FUNC) Error = SEMANTIC_ERR;
                            err_check(Error);
                        }

                        POTENTIAL_PARAMS += 1;

                        tmp = NextToken(&T, tmp);
                        if (tmp == EOF && !Error) Error = EOF;

                        err_check(Error);

                        if (T->name == COMMA || T->name == EOL) continue;

                        else {
                            Error = SYNTAX_ERR;
                            break;
                        }
                    }
                }
                break;

            case EOL:
                break;

            default:
                Error = SYNTAX_ERR;
                break;
        }
    }


    err_check(Error);
    return tmp;
}

int _F_DEF(int curr_char){
    tToken *T;
    tToken *defineHelper;
    tBTNodePtr helper;
    tBTNodePtr undefined_helper;

    int tmp = NextToken(&T, curr_char);

    if (tmp == EOF && !Error) Error = EOF;
    err_check(Error);

    switch (T->name){
        case ID :
        case ID_FUNC :

            Gen_FuncDef_begin(T->data.String);

            undefined_helper = IsDefined(&TempUndefinedFunctions, T->data.String);
            if (undefined_helper != NULL && undefined_helper->type == ID_FUNC) FUNC_TO_DEFINE -= 1;

            helper = IsDefined(&RootPtr, T->data.String);

            if (helper == NULL) {
                defineHelper = T;

                tmp = _PARAMS(tmp, FREE_NUM_OF_PARAMS, true);

                if (undefined_helper != NULL && undefined_helper->paramsCount != PARAMS_IN_DEFINITION) {
                    Error = PARAMS_COUNT_ERR;
                    err_check(Error);
                }

                Define(&LocalFramePtr, defineHelper->data.String, ID_FUNC, PARAMS_IN_DEFINITION, 0);

                tmp = NextToken(&T, tmp);
                if (tmp == EOF && !Error) Error = EOF;
                err_check(Error);

                CHECKING_LOCAL_FRAME = true;

                if (T->name != EOL) Error = SYNTAX_ERR;
                else tmp = _STMNT(tmp, KW_END);

                Gen_FuncDef_end();

                CHECKING_LOCAL_FRAME = false;

                Define(&RootPtr, defineHelper->data.String, ID_FUNC, PARAMS_IN_DEFINITION, 0);

                PARAMS_IN_DEFINITION = 0;

                SymDispose(&LocalFramePtr);
                SymInit(&LocalFramePtr);
            }
            else Error = DEFINITION_ERR;

            break;

        default:
            Error = SYNTAX_ERR;
            return tmp;
    }

    err_check(Error);
    return tmp;
}

int _COND(int curr_char){
    tToken *T;
    int tmp = curr_char;

    T = precedence_analysis(&tmp);
    err_check(Error);

    switch (T->name){
        case KW_THEN :

            Gen_If();

            tmp = NextToken(&T, tmp);
            if (tmp == EOF && !Error) Error = EOF;
            err_check(Error);

            if (T->name != EOL) Error = SYNTAX_ERR;
            else {

                tmp = _STMNT(tmp, KW_ELSE); //ked nastane error tak sa to dalej ani nedostane

                Gen_Else();

                tmp = NextToken(&T, tmp);
                if (tmp == EOF && !Error) Error = EOF;
                err_check(Error);

                if (T->name != EOL) Error = SYNTAX_ERR;
                else {
                    tmp = _STMNT(tmp, KW_END);
                    Gen_EndIf();
                }
            }
            break;

        default:
            Error = SYNTAX_ERR;
            //front dispose
            break;
    }

    err_check(Error);
    return tmp;
}

int _LOOP(int curr_char){
    tToken *T;
    int tmp = curr_char;
    in_while = true;

    Gen_While_cond();

    T = precedence_analysis(&tmp);
    err_check(Error);

    switch (T->name){
        case KW_DO :
            Gen_While_begin();

            tmp = NextToken(&T, tmp);
            if (tmp == EOF && !Error) Error = EOF;
            err_check(Error);

            if (T->name != EOL) Error = SYNTAX_ERR;
            else tmp = _STMNT(tmp, KW_END);
            Gen_While_end();

            break;

        default:
            Error = SYNTAX_ERR;
            break;
    }

    err_check(Error);
    return tmp;
}

int _PROG(int curr_char, tTokenName tName){
    tToken *T;
    tBTNodePtr helper;
    tBTNodePtr local_helper;

    if (CHECKING_LOCAL_FRAME) SymTablePtr = &LocalFramePtr;
    else SymTablePtr = &RootPtr;

    int tmp = NextToken(&T, curr_char);
    err_check(Error);
    copy_token(&extT);

    if (tmp == EOF) return EOF;

    if (T->name == tName) {
        STATEMENT_PROCESSED = 1;
        return tmp;
    }
    else if (tName != START) {
        if (T->name == KW_DEF) Error = SYNTAX_ERR;
    }

    err_check(Error);

    switch (T->name) {

        case KW_DEF :
            tmp = _F_DEF(tmp);
            break;

        case ID :
            helper = IsDefined(SymTablePtr, T->data.String);
            TempToken = T;

            /* ------------- nedefinovane ID ----------------- */
            if (helper == NULL) {
                if (CHECKING_LOCAL_FRAME) { //globalna funkcia volana v lokalnom scope
                    switch (T->name){
                        case ID:
                        case ID_FUNC:
                            local_helper = IsDefined(&RootPtr, T->data.String);
                            if (local_helper == NULL) break;
                            if (local_helper->type == ID_FUNC) {

                                Gen_Func_PrepFrame();

                                tmp = _PARAMS(tmp, local_helper->paramsCount, false);

                                Gen_Call_Func(T->data.String);

                                return tmp;
                            }
                            break;

                        default:
                            break;
                    }
                }

                tmp = NextToken(&T, tmp);
                err_check(Error);

                if (T->name != ASSIGN) {
                    if (CHECKING_LOCAL_FRAME) {
                        extT = T;

                        if (T->name == LEFT_PAR) tmp = _POTENTIONAL_FUNC(tmp, true);
                        else tmp = _POTENTIONAL_FUNC(tmp, false);

                        Define(&TempUndefinedFunctions, TempToken->data.String, ID_FUNC, POTENTIAL_PARAMS, 0);

                        POTENTIAL_PARAMS = 0;
                        FUNC_TO_DEFINE += 1;

                        extT->name = START;

                    }
                    else Error = DEFINITION_ERR;
                }

                else {

                    if (in_while) Gen_PreWhile_Def(TempToken->data.String, 0);
                    else Gen_DefineVar(TempToken->data.String, 0);

                    Define(SymTablePtr, TempToken->data.String, ID, 0, 0);
                    T = precedence_analysis(&tmp);

                }

                if (Error == POTENTIAL_SYNTAX_ERR) {
                    if (T->name == ID || T->name == ID_FUNC){
                        Error = CHECK_OK;

                        helper = IsDefined(SymTablePtr, T->data.String);

                        if (helper->type == ID_FUNC) {
                            Gen_Func_PrepFrame();

                            tmp = _PARAMS(tmp, helper->paramsCount, false);

                            Gen_Call_Func(T->data.String);
                        }
                        else {
                            extT = T;

                            T = precedence_analysis(&tmp);

                        }
                    }
                    else {
                        if (T->name == INTEGER_L || T->name == FLOAT_L || T->name == STRING_L) {
                            Error = CHECK_OK;
                            extT = T;
                            T = precedence_analysis(&tmp);

                        }
                        else Error = SYNTAX_ERR;
                    }
                }
                Gen_AssignTo(TempToken->data.String, 0);
            }

                /* ------------- definovana funckia ----------------- */

            else if (helper->type == ID_FUNC) {
                if ( strcmp(TempToken->data.String, "print") == 0){
                    is_print = true;

                    Gen_Func_PrepFrame();

                    tmp = _PARAMS(tmp, helper->paramsCount, false);

                    Gen_Call_Print(print_params);

                    print_params = 0;
                    is_print = false;
                }
                else{
                    Gen_Func_PrepFrame();

                    tmp = _PARAMS(tmp, helper->paramsCount, false);

                    Gen_Call_Func(T->data.String);
                }

            }

                /* ------------- definovana premenna ----------------- */

            else if (helper->type == ID) {

                extT = T;
                T = precedence_analysis(&tmp);


                if (Error == POTENTIAL_SYNTAX_ERR){
                    if (T->name != ASSIGN) Error = SYNTAX_ERR;
                    else {
                        Error = CHECK_OK;

                        tmp = NextToken(&T, tmp);
                        if (tmp == EOF && !Error) Error = EOF;
                        err_check(Error);

                        if (T->name == ID){
                            helper = IsDefined(SymTablePtr, T->data.String);

                            if (helper->type == ID_FUNC) {
                                Gen_Func_PrepFrame();

                                tmp = _PARAMS(tmp, helper->paramsCount, false);

                                Gen_Call_Func(T->data.String);
                            }
                            else {
                                extT = T;
                                T = precedence_analysis(&tmp);

                            }
                        }
                        else {
                            extT = T;
                            T = precedence_analysis(&tmp);
                        }
                    }
                }

                Gen_AssignTo(TempToken->data.String, 0);
            }
            break;

        case ID_FUNC :
            helper = IsDefined(&RootPtr, T->data.String);
            if (helper != NULL){
                Gen_Func_PrepFrame();

                tmp = _PARAMS(tmp, helper->paramsCount, false);

                Gen_Call_Func(T->data.String);
            }
            else Error = DEFINITION_ERR;
            break;

        case EOL :
            if (tName != START) return tmp;
            else tmp = _PROG(tmp, START);
            break;

        case KW_NIL :
            if (tName != START) return tmp;
            else tmp = _PROG(tmp, START);
            break;

        case KW_IF :
            tmp = _COND(tmp);
            break;

        case KW_WHILE :
            while_counter++;
            if (while_counter == 1) MostTop_While();
            tmp = _LOOP(tmp);
            break;

        case INTEGER_L :
        case FLOAT_L :
        case STRING_L :
        case LEFT_PAR :
            extT = T;
            T = precedence_analysis(&tmp);
            if (Error == POTENTIAL_SYNTAX_ERR) Error = SYNTAX_ERR;
            break;

        default:
            Error = SYNTAX_ERR;
            break;
    }

    err_check(Error);
    return tmp;
}

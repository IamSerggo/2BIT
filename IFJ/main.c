#include "parser.h"
#include "scanner.h"
#include "generator.h"

int Error = 0;
int FUNC_TO_DEFINE = 0;

tBTNodePtr RootPtr;
tBTNodePtr LocalFramePtr;
tBTNodePtr TempUndefinedFunctions;

tToken *extT;

bool CHECKING_LOCAL_FRAME = false;
bool cmpOpAlreadyUsed = false;
bool definitionError = false;

int main(){
  if ((extT = malloc(sizeof(tToken))) == NULL) {
    Error = INTERNAL_ERR; // interna chyba prekladaca
  }

  err_check(Error);

  SymInit(&RootPtr); //Globalne definovane ID a ID_FUNC
  SymInit(&LocalFramePtr);//Lokalne (vo funckii) definovane ID a ID_FUNC
  SymInit(&TempUndefinedFunctions);//Funkcie, ktore su najskor zavolane a potom definovane
  Gen_Init_Program();


  int tmp = _PROG(BOF, START);
  while (tmp != EOF) tmp = _PROG(tmp, START);

  if (FUNC_TO_DEFINE) Error = DEFINITION_ERR; //pokial je cely program OK, ale nejaka funckia ostala nedefinovana
  err_check(Error);

  free(extT);
  SymDispose(&RootPtr);
  SymDispose(&LocalFramePtr);
  SymDispose(&TempUndefinedFunctions);
  Generate_Program();

  return 0;
}

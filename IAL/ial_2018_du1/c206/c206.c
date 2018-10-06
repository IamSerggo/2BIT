
/* c206.c **********************************************************}
{* Téma: Dvousměrně vázaný lineární seznam
**
**                   Návrh a referenční implementace: Bohuslav Křena, říjen 2001
**                            Přepracované do jazyka C: Martin Tuček, říjen 2004
**                                            Úpravy: Kamil Jeřábek, září 2018
**
** Implementujte abstraktní datový typ dvousměrně vázaný lineární seznam.
** Užitečným obsahem prvku seznamu je hodnota typu int.
** Seznam bude jako datová abstrakce reprezentován proměnnou
** typu tDLList (DL znamená Double-Linked a slouží pro odlišení
** jmen konstant, typů a funkcí od jmen u jednosměrně vázaného lineárního
** seznamu). Definici konstant a typů naleznete v hlavičkovém souboru c206.h.
**
** Vaším úkolem je implementovat následující operace, které spolu
** s výše uvedenou datovou částí abstrakce tvoří abstraktní datový typ
** obousměrně vázaný lineární seznam:
**
**      DLInitList ...... inicializace seznamu před prvním použitím,
**      DLDisposeList ... zrušení všech prvků seznamu,
**      DLInsertFirst ... vložení prvku na začátek seznamu,
**      DLInsertLast .... vložení prvku na konec seznamu,
**      DLFirst ......... nastavení aktivity na první prvek,
**      DLLast .......... nastavení aktivity na poslední prvek,
**      DLCopyFirst ..... vrací hodnotu prvního prvku,
**      DLCopyLast ...... vrací hodnotu posledního prvku,
**      DLDeleteFirst ... zruší první prvek seznamu,
**      DLDeleteLast .... zruší poslední prvek seznamu,
**      DLPostDelete .... ruší prvek za aktivním prvkem,
**      DLPreDelete ..... ruší prvek před aktivním prvkem,
**      DLPostInsert .... vloží nový prvek za aktivní prvek seznamu,
**      DLPreInsert ..... vloží nový prvek před aktivní prvek seznamu,
**      DLCopy .......... vrací hodnotu aktivního prvku,
**      DLActualize ..... přepíše obsah aktivního prvku novou hodnotou,
**      DLSucc .......... posune aktivitu na další prvek seznamu,
**      DLPred .......... posune aktivitu na předchozí prvek seznamu,
**      DLActive ........ zjišťuje aktivitu seznamu.
**
** Při implementaci jednotlivých funkcí nevolejte žádnou z funkcí
** implementovaných v rámci tohoto příkladu, není-li u funkce
** explicitně uvedeno něco jiného.
**
** Nemusíte ošetřovat situaci, kdy místo legálního ukazatele na seznam
** předá někdo jako parametr hodnotu NULL.
**
** Svou implementaci vhodně komentujte!
**
** Terminologická poznámka: Jazyk C nepoužívá pojem procedura.
** Proto zde používáme pojem funkce i pro operace, které by byly
** v algoritmickém jazyce Pascalovského typu implemenovány jako
** procedury (v jazyce C procedurám odpovídají funkce vracející typ void).
**/

#include "c206.h"

int errflg;
int solved;

void DLError() {
/*
** Vytiskne upozornění na to, že došlo k chybě.
** Tato funkce bude volána z některých dále implementovaných operací.
**/
    printf ("*ERROR* The program has performed an illegal operation.\n");
    errflg = TRUE;             /* globální proměnná -- příznak ošetření chyby */
    return;
}

void DLInitList (tDLList *L) {
/*
** Provede inicializaci seznamu L před jeho prvním použitím (tzn. žádná
** z následujících funkcí nebude volána nad neinicializovaným seznamem).
** Tato inicializace se nikdy nebude provádět nad již inicializovaným
** seznamem, a proto tuto možnost neošetřujte. Vždy předpokládejte,
** že neinicializované proměnné mají nedefinovanou hodnotu.
**/

  //inicializujem prvky zoznamu na NULL
  L->First = NULL;
  L->Act = NULL;
  L->Last = NULL;

 //solved = FALSE;                   /* V případě řešení, smažte tento řádek! */
}

void DLDisposeList (tDLList *L) {
/*
** Zruší všechny prvky seznamu L a uvede seznam do stavu, v jakém
** se nacházel po inicializaci. Rušené prvky seznamu budou korektně
** uvolněny voláním operace free.
**/

  tDLElemPtr tmp;

  L->Act = NULL;
  L->Last = NULL;

  while (L->First != NULL){
    tmp = L->First;
    L->First = tmp->rptr;
    free(tmp);
  }
 //solved = FALSE;                   /* V případě řešení, smažte tento řádek! */
}

void DLInsertFirst (tDLList *L, int val) {
/*
** Vloží nový prvek na začátek seznamu L.
** V případě, že není dostatek paměti pro nový prvek při operaci malloc,
** volá funkci DLError().
**/
  tDLElemPtr tmp = malloc(sizeof(struct tDLElem));
  if (tmp == NULL) DLError();
  else{
    tmp->data = val;//vlozim data
    tmp->lptr = NULL;//vkladam na zaciatok
    tmp->rptr = L->First;//na pravo bude prvy prvok

      //ak je zoznam prazdny, musime nastavit posledny pointer na pridanu hodnotu
    if (L->First == NULL) L->Last = tmp;
    else L->First->lptr = tmp; //ak nie je, nahram prvok na prve miesto
    L->First = tmp;
  }
}

void DLInsertLast(tDLList *L, int val) {
/*
** Vloží nový prvek na konec seznamu L (symetrická operace k DLInsertFirst).
** V případě, že není dostatek paměti pro nový prvek při operaci malloc,
** volá funkci DLError().
**/
  tDLElemPtr tmp = malloc(sizeof(struct tDLElem));
  if (tmp == NULL) DLError(); //osetrenie podmienky
  else{
    tmp->data = val;//vlozim data
    tmp->lptr = L->Last;//vkladam na koniec
    tmp->rptr = NULL;//na pravo nebude nic => tmp je posledny prvok

    if (L->First == NULL) L->First = tmp; //prazdny zoznam => posledny prvok je v podstate prvy
    else L->Last->rptr = tmp;
    L->Last = tmp;
  }
}

void DLFirst (tDLList *L) {
/*
** Nastaví aktivitu na první prvek seznamu L.
** Funkci implementujte jako jediný příkaz (nepočítáme-li return),
** aniž byste testovali, zda je seznam L prázdný.
**/

  L->Act = L->First;

// solved = FALSE;                   /* V případě řešení, smažte tento řádek! */
}

void DLLast (tDLList *L) {
/*
** Nastaví aktivitu na poslední prvek seznamu L.
** Funkci implementujte jako jediný příkaz (nepočítáme-li return),
** aniž byste testovali, zda je seznam L prázdný.
**/

  L->Act = L->Last;
// solved = FALSE;                   /* V případě řešení, smažte tento řádek! */
}

void DLCopyFirst (tDLList *L, int *val) {
/*
** Prostřednictvím parametru val vrátí hodnotu prvního prvku seznamu L.
** Pokud je seznam L prázdný, volá funkci DLError().
**/

if (L->First == NULL) DLError(); //osetrenie podmienky
else *val = L->First->data; //vratim hodnotu prveho

 //solved = FALSE;                   /* V případě řešení, smažte tento řádek! */
}

void DLCopyLast (tDLList *L, int *val) {
/*
** Prostřednictvím parametru val vrátí hodnotu posledního prvku seznamu L.
** Pokud je seznam L prázdný, volá funkci DLError().
**/

  if (L->First == NULL) DLError(); //osetrenie podmienky
  else *val = L->Last->data; //vratim hodnotu posledneho

 //solved = FALSE;                   /* V případě řešení, smažte tento řádek! */
}

void DLDeleteFirst (tDLList *L) {
/*
** Zruší první prvek seznamu L. Pokud byl první prvek aktivní, aktivita
** se ztrácí. Pokud byl seznam L prázdný, nic se neděje.
**/

  if (L->First != NULL){

    if(L->Last == L->First) DLInitList(L); //ak je v zozname len jeden prvok, zoznam vymazem
    else{
      if (L->Act == L->First) L->Act = NULL; //ak je prvy prvok aktivny, aktivitu zrusim

      tDLElemPtr tmp = L->First; //prvy si ulozim do pomocneho prvku
      L->First = tmp->rptr; //na prve miesto vlozim druhy prvok
      L->First->lptr = NULL; //bude prvy, na lavo teda nie je ziadny prvok
      free(tmp); //prvok zahodim
    }
  }
 //solved = FALSE;                   /* V případě řešení, smažte tento řádek! */
}

void DLDeleteLast (tDLList *L) {
/*
** Zruší poslední prvek seznamu L. Pokud byl poslední prvek aktivní,
** aktivita seznamu se ztrácí. Pokud byl seznam L prázdný, nic se neděje.
**/

  if (L->First != NULL){
    if(L->Last == L->First) DLInitList(L); //ak je v zozname len jeden prvok, zoznam vymazem
    else{
      if (L->Act == L->Last) L->Act = NULL; //ak je posledny prvok aktivny, aktivitu zrusim

      tDLElemPtr tmp = L->Last; //posledny si ulozim do pomocneho prvku
      L->Last = tmp->lptr; //na posledne miesto vlozim predposleny prvok
      L->Last->rptr = NULL; //chcem aby bol posledny, na pravo teda nebude ziadny prvok
      free(tmp); //prvok zahodim
    }
  }
 //solved = FALSE;                   /* V případě řešení, smažte tento řádek! */
}

void DLPostDelete (tDLList *L) {
/*
** Zruší prvek seznamu L za aktivním prvkem.
** Pokud je seznam L neaktivní nebo pokud je aktivní prvek
** posledním prvkem seznamu, nic se neděje.
**/

  if (L->Act != NULL && L->Act != L->Last){ //osetrenie podmienky
    tDLElemPtr tmp = L->Act->rptr; //prvok si ulozim do pomocnej premennej

    if (tmp == L->Last) L->Last = L->Act; //pokial je ruseny prvok posledny, nastavim aktivny na posledny
    else {
      L->Act->rptr = tmp->rptr; //pokial nie je posledny, ruseny prvok preskocim (posuniem pointre)
      tmp->rptr->lptr = L->Act;
    }
    free(tmp); //prvok zahodim

  }
 //solved = FALSE;                   /* V případě řešení, smažte tento řádek! */
}

void DLPreDelete (tDLList *L) {
/*
** Zruší prvek před aktivním prvkem seznamu L .
** Pokud je seznam L neaktivní nebo pokud je aktivní prvek
** prvním prvkem seznamu, nic se neděje.
**/

  if (L->Act != NULL && L->Act != L->First){ //osetrenie podmienky
    tDLElemPtr tmp = L->Act->lptr; //pomocna premenna

    if (tmp == L->First) L->First = L->Act; //pokial je ruseny prvok prvy, nastavim aktivny na prvy
    else{
      L->Act->lptr = tmp->lptr; //pokial nie je prvy, znovu posuniem pointre (akokeby preskocim aj z lavej strany
      tmp->lptr->rptr = L->Act; //aj z pravej strany ruseny prvok)
    }
    free(tmp); //prvok zahodim
  }

 //solved = FALSE;                   /* V případě řešení, smažte tento řádek! */
}

void DLPostInsert (tDLList *L, int val) {
/*
** Vloží prvek za aktivní prvek seznamu L.
** Pokud nebyl seznam L aktivní, nic se neděje.
** V případě, že není dostatek paměti pro nový prvek při operaci malloc,
** volá funkci DLError().
**/

  if (L->Act != NULL){
    tDLElemPtr tmp = malloc(sizeof(struct tDLElem));
    if (tmp == NULL) DLError();
    else{
      tmp->data = val;
      tmp->rptr = L->Act->rptr;
      tmp->lptr = L->Act;
      L->Act->rptr = tmp;

      if (L->Act == L->Last) L->Last = tmp;
      else tmp->rptr->lptr = tmp;
    }
  }
 //solved = FALSE;                   /* V případě řešení, smažte tento řádek! */
}

void DLPreInsert (tDLList *L, int val) {
/*
** Vloží prvek před aktivní prvek seznamu L.
** Pokud nebyl seznam L aktivní, nic se neděje.
** V případě, že není dostatek paměti pro nový prvek při operaci malloc,
** volá funkci DLError().
**/

  if (L->Act != NULL){
    tDLElemPtr tmp = malloc(sizeof(struct tDLElem));
    if (tmp == NULL) DLError();
    else{
      tmp->data = val;
      tmp->lptr = L->Act->lptr;
      tmp->rptr = L->Act;
      L->Act->lptr = tmp;

      if (L->Act == L->First) L->First = tmp;
      else tmp->lptr->rptr = tmp;
    }
  }
 //solved = FALSE;                   /* V případě řešení, smažte tento řádek! */
}

void DLCopy (tDLList *L, int *val) {
/*
** Prostřednictvím parametru val vrátí hodnotu aktivního prvku seznamu L.
** Pokud seznam L není aktivní, volá funkci DLError ().
**/

  if (L->Act != NULL) *val = L->Act->data;
  else DLError();

 //solved = FALSE;                   /* V případě řešení, smažte tento řádek! */
}

void DLActualize (tDLList *L, int val) {
/*
** Přepíše obsah aktivního prvku seznamu L.
** Pokud seznam L není aktivní, nedělá nic.
**/

  if (L->Act != NULL) L->Act->data = val;
 //solved = FALSE;                   /* V případě řešení, smažte tento řádek! */
}

void DLSucc (tDLList *L) {
/*
** Posune aktivitu na následující prvek seznamu L.
** Není-li seznam aktivní, nedělá nic.
** Všimněte si, že při aktivitě na posledním prvku se seznam stane neaktivním.
**/

  if (L->Act != NULL){
    if (L->Act == L->Last) L->Act = NULL;
    else{
      tDLElemPtr tmp = L->Act->rptr;
      L->Act = tmp;
    }
  }
 //solved = FALSE;                   /* V případě řešení, smažte tento řádek! */
}


void DLPred (tDLList *L) {
/*
** Posune aktivitu na předchozí prvek seznamu L.
** Není-li seznam aktivní, nedělá nic.
** Všimněte si, že při aktivitě na prvním prvku se seznam stane neaktivním.
**/

  if (L->Act != NULL){
    if (L->Act == L->First) L->Act = NULL;
    else{
      tDLElemPtr tmp = L->Act->lptr;
      L->Act = tmp;
    }
  }
// solved = FALSE;                   /* V případě řešení, smažte tento řádek! */
}

int DLActive (tDLList *L) {
/*
** Je-li seznam L aktivní, vrací nenulovou hodnotu, jinak vrací 0.
** Funkci je vhodné implementovat jedním příkazem return.
**/

  return(L->Act == NULL ? 0 : 1);
 //solved = FALSE;                   /* V případě řešení, smažte tento řádek! */
}

/* Konec c206.c*/

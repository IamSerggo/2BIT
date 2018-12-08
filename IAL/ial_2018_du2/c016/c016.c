
/* c016.c: **********************************************************}
{* Téma:  Tabulka s Rozptýlenými Položkami
**                      První implementace: Petr Přikryl, prosinec 1994
**                      Do jazyka C prepsal a upravil: Vaclav Topinka, 2005
**                      Úpravy: Karel Masařík, říjen 2014
**                              Radek Hranický, 2014-2018
**
** Vytvořete abstraktní datový typ
** TRP (Tabulka s Rozptýlenými Položkami = Hash table)
** s explicitně řetězenými synonymy. Tabulka je implementována polem
** lineárních seznamů synonym.
**
** Implementujte následující procedury a funkce.
**
**  HTInit ....... inicializuje tabulku před prvním použitím
**  HTInsert ..... vložení prvku
**  HTSearch ..... zjištění přítomnosti prvku v tabulce
**  HTDelete ..... zrušení prvku
**  HTRead ....... přečtení hodnoty prvku
**  HTClearAll ... zrušení obsahu celé tabulky (inicializace tabulky
**                 poté, co již byla použita)
**
** Definici typů naleznete v souboru c016.h.
**
** Tabulka je reprezentována datovou strukturou typu tHTable,
** která se skládá z ukazatelů na položky, jež obsahují složky
** klíče 'key', obsahu 'data' (pro jednoduchost typu float), a
** ukazatele na další synonymum 'ptrnext'. Při implementaci funkcí
** uvažujte maximální rozměr pole HTSIZE.
**
** U všech procedur využívejte rozptylovou funkci hashCode.  Povšimněte si
** způsobu předávání parametrů a zamyslete se nad tím, zda je možné parametry
** předávat jiným způsobem (hodnotou/odkazem) a v případě, že jsou obě
** možnosti funkčně přípustné, jaké jsou výhody či nevýhody toho či onoho
** způsobu.
**
** V příkladech jsou použity položky, kde klíčem je řetězec, ke kterému
** je přidán obsah - reálné číslo.
*/

#include "c016.h"

int HTSIZE = MAX_HTSIZE;
int solved;

/*          -------
** Rozptylovací funkce - jejím úkolem je zpracovat zadaný klíč a přidělit
** mu index v rozmezí 0..HTSize-1.  V ideálním případě by mělo dojít
** k rovnoměrnému rozptýlení těchto klíčů po celé tabulce.  V rámci
** pokusů se můžete zamyslet nad kvalitou této funkce.  (Funkce nebyla
** volena s ohledem na maximální kvalitu výsledku). }
*/

int hashCode ( tKey key ) {
	int retval = 1;
	int keylen = strlen(key);
	for ( int i=0; i<keylen; i++ )
		retval += key[i];
	return ( retval % HTSIZE );
}

/*
** Inicializace tabulky s explicitně zřetězenými synonymy.  Tato procedura
** se volá pouze před prvním použitím tabulky.
*/

void htInit ( tHTable* ptrht ) {

	if (ptrht != NULL){ //kym nepridem na koniec, nastavim vsetky prvky na NULL
		for (int i = 0; i < HTSIZE; i++){
			(*ptrht)[i] = NULL;
		}
	}
 //solved = 0; /*v pripade reseni, smazte tento radek!*/
}

/* TRP s explicitně zřetězenými synonymy.
** Vyhledání prvku v TRP ptrht podle zadaného klíče key.  Pokud je
** daný prvek nalezen, vrací se ukazatel na daný prvek. Pokud prvek nalezen není,
** vrací se hodnota NULL.
**
*/

tHTItem* htSearch ( tHTable* ptrht, tKey key ) {

	if (ptrht != NULL){

		for (tHTItem* tmp = (*ptrht)[hashCode(key)]; tmp != NULL; tmp = tmp->ptrnext){ //prechadzam tabulku
			if (strcmp(tmp->key, key) == 0) return tmp; //nasiel som zhodu, vratim
		}

	}
 //solved = 0; /*v pripade reseni, smazte tento radek!*/
 	return NULL; //nenasiel som nic, vratim NULL
}

/*
** TRP s explicitně zřetězenými synonymy.
** Tato procedura vkládá do tabulky ptrht položku s klíčem key a s daty
** data.  Protože jde o vyhledávací tabulku, nemůže být prvek se stejným
** klíčem uložen v tabulce více než jedenkrát.  Pokud se vkládá prvek,
** jehož klíč se již v tabulce nachází, aktualizujte jeho datovou část.
**
** Využijte dříve vytvořenou funkci htSearch.  Při vkládání nového
** prvku do seznamu synonym použijte co nejefektivnější způsob,
** tedy proveďte.vložení prvku na začátek seznamu.
**/

void htInsert ( tHTable* ptrht, tKey key, tData data ) {

	if (ptrht != NULL){

		tHTItem* tmp = htSearch(ptrht, key); //skusim vyhladat v tabulke
		if (tmp == NULL){ //ak som nenasiel, musim novy prvok pridat

			if ((tmp = malloc(sizeof(tHTItem))) == NULL) return; //alokacia pamate

			int keylen = strlen(key);
			if ((tmp->key = malloc(keylen + 1)) == NULL) return; //alokacie pre kluc

			strcpy(tmp->key, key); //ulozim kluc
			tmp->data = data; //ulozim data

			tmp->ptrnext = (*ptrht)[hashCode(key)]; //previazem tabulku
			(*ptrht)[hashCode(key)] = tmp;

		}
		else tmp->data = data; //ak som nasiel, len aktualizujem data
	}
 //solved = 0; /*v pripade reseni, smazte tento radek!*/
}

/*
** TRP s explicitně zřetězenými synonymy.
** Tato funkce zjišťuje hodnotu datové části položky zadané klíčem.
** Pokud je položka nalezena, vrací funkce ukazatel na položku
** Pokud položka nalezena nebyla, vrací se funkční hodnota NULL
**
** Využijte dříve vytvořenou funkci HTSearch.
*/

tData* htRead ( tHTable* ptrht, tKey key ) {

	if (ptrht != NULL){
		tHTItem* tmp = htSearch(ptrht, key);
		if (tmp != NULL) return &(tmp->data); //ak som nasiel, vratim pointer na data
	}
	return NULL; //ak som nic nenasiel, vratim NULL
 //solved = 0; /*v pripade reseni, smazte tento radek!*/
}

/*
** TRP s explicitně zřetězenými synonymy.
** Tato procedura vyjme položku s klíčem key z tabulky
** ptrht.  Uvolněnou položku korektně zrušte.  Pokud položka s uvedeným
** klíčem neexistuje, dělejte, jako kdyby se nic nestalo (tj. nedělejte
** nic).
**
** V tomto případě NEVYUŽÍVEJTE dříve vytvořenou funkci HTSearch.
*/

void htDelete ( tHTable* ptrht, tKey key ) {
	if (ptrht != NULL){

		int index = hashCode(key);
		tHTItem* tmp = (*ptrht)[index]; //nahram si do premennej polozku tabulky
		tHTItem* tmp2 = tmp;

		if (tmp != NULL){ //ak taka polozka existuje, idem hladat spravny kluc
			while (tmp != NULL){ //prechadzam polozku az kym nenajdem ten spravny kluc

				if (strcmp(tmp->key, key) == 0){ // ak som kluc nasiel, idem mazat

					if (tmp->ptrnext == NULL && (*ptrht)[index] == tmp) (*ptrht)[index] = NULL; //ak je to jedina polozka a je hned prva
					else { //ked sa to nestalo, mozu nastat 2 situacie :

						if (tmp->ptrnext != NULL && (*ptrht)[index] == tmp) (*ptrht)[index] = tmp->ptrnext; //ak nie je jedina ale je prva
						else tmp2->ptrnext = tmp->ptrnext; //ak je na hocijakom mieste v polozke, previazem tabulku (mazany prvok preskocim)
					}

					free(tmp->key); //uvolnim pamat
					free(tmp);
					return;
				}

				tmp2 = tmp;
				tmp = tmp->ptrnext; // ked som nenasiel, pokracujem dalej
			}
		}
	}
 //solved = 0; /*v pripade reseni, smazte tento radek!*/
}

/* TRP s explicitně zřetězenými synonymy.
** Tato procedura zruší všechny položky tabulky, korektně uvolní prostor,
** který tyto položky zabíraly, a uvede tabulku do počátečního stavu.
*/

void htClearAll ( tHTable* ptrht ) {

	if (ptrht != NULL){

		for (int i = 0; i < HTSIZE; i++) { //v cykle prejdem celu tabulku
			free((*ptrht)[i]); //uvolnim polozky
			(*ptrht)[i] = NULL; //a nastavim ich na NULL
		}
	}
 //solved = 0; /*v pripade reseni, smazte tento radek!*/
}

#ifndef MAINMENU_H_INCLUDED
#define MAINMENU_H_INCLUDED

#include "DataManage.h"
#include <string.h>


//funkcija provjerava tip korisnika
//prima podatak tipa USER
//vraca 1-ako je gost, 2-ako je postojeci korisnik, 3-ako je admin
//f-ja pretpostavlja da ne moze da se desi pogresan unos
int checkType(char []);

//f-ja prikazuje glavni meni, na osnovu korisnika
//prima tip korisnika i ime grada
//za sada ne vraca nista, ali ce najvjerovatnije vracati nesto u buducnosti, nzm jos sta
void showMainMenu(char[], char[], char[]);



#endif // MAINMENU_H_INCLUDED

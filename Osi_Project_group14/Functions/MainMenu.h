#ifndef MAINMENU_H_INCLUDED
#define MAINMENU_H_INCLUDED

#include "DataManage.h"
#include <string.h>



int checkType(USER* user);//1 guest  2user 3 admin,  f-ja pretpostavlja da ne postoji mogucnost da je upisano nesto drugo
void showMainMenu(USER* user);



#endif // MAINMENU_H_INCLUDED

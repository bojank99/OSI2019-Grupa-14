#pragma once
#include<stdio.h>

//funkcija koja upisuje ime grada u datoteku
//funkcija prima datoteku u koju upisuje ime grada
void writeCity(FILE*);


//funkcija koja pri prvom pokretanju pita
//da li je sistem vec podesen za neki grad
//a ako nije onda postavi novi grad
//funkcija prima string, odnosno vraca ime grada, tj. taj string i jeste ime grada
void isThereCity(char []);


//ovu funkcija se moze pozivati svaki put
//pri otvaranju nove "stranice"
//funkcija prima ime grada
void newPage(char []);

#pragma once
/*OK, prije svega ovo su samo osnovne funkije za upis i brisanje unutaj fajla. Jos treba napisati funkcija da kod bude pregledniji i tako to.
Sto se tice fajlova, zasad samo podrazumjevani su 'Users' i 'Events' (u folderu 'Data'), kao glavni gdje se cuvaju podaci.
Format u kojem su upisani i u kojem se citajuse moze vidjeti iz funkcija; a mislio sam da na pocetku fajla bude kao broj na osnovu 
kojeg se racuna ID i za korinike i dogadjaje, naravno posto je to string mogu biti i slova, ali onda se mora dodatno definisati dodjeljjivanje id-a, naravno to se moze izbaciti. 
Funkcije za upis/ispis su samo tu radi formata, pa jos treba da se napisu one koje provjeravaju postojece korisnike i dogadjaje ali ima vremena.
*/
#include <stdio.h>


typedef struct user {
	char* userID;					
	char* userName; 
	char* password; 
	char* type;
	struct user* next;
}USER;

typedef struct Event {
	char* eventID; char* headline; char* category; char* description; char* date; char* user;
	int finished;					//kao bool
	struct Event* next;
}EVENT;

typedef struct EventTab {				//mozda korisiti za prikaz
	EVENT** first;							//lista dogadjaja
	USER* current;							//trenutno ulogovan user
}EventTab;

void createUser(USER*);					//pomocne funkcije za inicijalizovanje
void createEvent(EVENT*);

int openUserData(FILE**);				//za otvaranje fajlova
int openEventData(FILE**);

int loadUser(USER*,  FILE*);			//za ucitavanje iz fajla
int loadEvent(EVENT*,  FILE*);

void writeUser(USER*,  FILE*);			//za upisivanje u fajl
void writeEvent(EVENT*,  FILE*);

int deleteUser(char*);			//za brisanje iz fajla
int deleteEvent(char*);

void freeUser(USER*);					//pomocne za oslobadjanje memorije
void freeEvent(EVENT*);

void getId(int*,FILE*);					



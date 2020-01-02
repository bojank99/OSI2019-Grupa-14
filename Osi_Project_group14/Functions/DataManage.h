#pragma once
#include <stdio.h>


typedef struct user {
	char* userID;
	char* userName;
	char* password;
	char* type;
	struct user* next;
}USER;

typedef struct unregUser{
    USER base;
    char* email;
    char* name;
    char* surname;
    struct unregUser* next;
}UNREGUSER;

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

int openUserData(FILE**, char*);				//za otvaranje fajlova
int openEventData(FILE**, char*);

int loadUser(USER*,  FILE*);			//za ucitavanje iz fajla
int loadEvent(EVENT*,  FILE*);

void writeUser(USER*,  FILE*);			//za upisivanje u fajl
int appendUser(USER*,FILE*);                          //dodavanje na kraj(napravljeno da stavlja novi id na pocetak fajla,automatski stavlja tip korisnika i racuna id korisnika)
void writeEvent(EVENT*,  FILE*);

void allocUnregUser(UNREGUSER*);                      //pomocna da alocira prostor za stringove
int openUnregUserData(FILE**,char*);                  //otvaranje fajla sa unreg korisnicima
int loadUnregUser(UNREGUSER*,FILE*);                  //ucitavanje neregistrovanog korisnika
void writeUnregUser(UNREGUSER*,FILE*);                //format upisivanja
int unregUserExist(char*);                            //kao pomocna ako negdje zatreba
int approveUser(char*);                               //odobravanje jednog korisnika, unosi se username korisnika
void approveAllUsers();                               //odobravanje svhih korisnika iz fajla

int deleteUser(char*);			//za brisanje iz fajla
int deleteEvent(char*);
int deleteUnregUser(char*);

void freeUser(USER*);					//pomocne za oslobadjanje memorije
void freeEvent(EVENT*);
void freeUnregUser(UNREGUSER*);

void getId(int*,FILE*);



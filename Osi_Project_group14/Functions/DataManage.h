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

typedef struct EventTab {				    //mozda korisiti za prikaz
	EVENT** first;							//lista dogadjaja
	USER* current;							//trenutno ulogovan user
}EventTab;

typedef struct Index{  // Struktura za rad sa indeksnim datoteakama
    char eventID[15], key[15];
    int position;
}INDEX;


void createUser(USER*);					//pomocne funkcije za inicijalizovanje
void createEvent(EVENT*);

int openUserData(FILE**, char*);				//za otvaranje fajlova
int openEventData(FILE**, char*);
int openCategoryIndex(FILE**,char*);
int openDateIndex(FILE**,char*);

int loadUser(USER*,  FILE*);			//za ucitavanje iz fajla
int loadEvent(EVENT*,  FILE*);

void writeUser(USER*,  FILE*);			//za upisivanje u fajl
int appendUser(USER*,FILE*);             //dodavanje na kraj(napravljeno da stavlja novi id na pocetak fajla,automatski stavlja tip korisnika i racuna id korisnika)
void writeEvent(EVENT*,  FILE*);

int deleteUser(char*);			        //za brisanje iz fajla
int deleteEvent(char*,unsigned long int*);

void allocUnregUser(UNREGUSER*);                      //pomocna da alocira prostor za stringove
int openUnregUserData(FILE**,char*);                  //otvaranje fajla sa unreg korisnicima
int loadUnregUser(UNREGUSER*,FILE*);                  //ucitavanje neregistrovanog korisnika
void writeUnregUser(UNREGUSER*,FILE*);                //format upisivanja
int unregUserExist(char*);                            //kao pomocna ako negdje zatreba
int approveUser(char*);                               //odobravanje jednog korisnika, unosi se username korisnika
void approveAllUsers();                               //odobravanje svhih korisnika iz fajla

int removeEvent(char*);                 //brise iz fajla i update-uje indexe(ovu treba koristiti iz menija)
int updateIndex(int,int,char*);         //update-uje position dio svakog zapisa u indeksu, da bi se poklapalo nakon brisanja eventa
void freeUser(USER*);					//pomocne za oslobadjanje memorije
void freeEvent(EVENT*);
void freeUnregUser(UNREGUSER*);

void getId(int*,FILE*);


// Funkcija koja od datuma, koji je string, pravi cjelobrojni niz od tri elementa koji predstavljaju godinu, mjesec i dan
void makeDatum(const char*, int[]);

// Funkcija koja poredi da datuma, predpostavlja se da je datum u obliku "dd.mm.gggg."
int compareDatum (const char*, const char*);

//Funkcija koja upisuje sortirane podatke u indeksnu datoteku, prilikom dodavanja novog dogadjaja ili prilikom brisanja dogadjaja
//Prilikom upisa dodaje novi element na pocetak niza elemenata koji imaju isti kljuc
//Funkcija prima niz elementa koji su bili u datoteci, novi element koji se dodaje, naziv datoteke u koju se upisuje i pokazivac na funkciju koja poredi dva kljuca
void printInIndex(INDEX*, INDEX*, int, const char*, int (*compare)(const char*, const char*));

//Funkcija koja iscitava sve elemente iz datoteke
//Funkcija prima pokazivac na broj elemenata u datoteci i naziv datoteke, a pokazivac na niz elemenata u datoteci
INDEX* readFromIndex(int*, const char*);

//Funkcija koju pozivamo da bismo sortirali sve indeksne datoteke prilikom dodavanja ili brisanja elementa
//Funkcija prima pokazivac na dogadjaj koji dodajemo i poziciju tog dogadjaja u glavnoj datoteci (ako je pozicija 0, znaci da brisemo taj element iz datoteke)
void sort (EVENT*, int);


//f-ja za dodavanje komentara u datoteku
//prima id dogadjaja i komentar
void addComment(int, char*);


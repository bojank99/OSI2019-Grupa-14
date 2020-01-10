#pragma once
#include <stdio.h>
#include "Show.h"
#include <ctype.h>


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
	char* eventID; char* headline; char* category; char* description; char* date; char* user; char* time; char* location;
	int finished;					//kao bool
	struct Event* next;
}EVENT;

typedef struct comment{                     //struktura za komentar
    char* comEventID;
    char* comUsername;
    char* commentText;
}COMMENT;

typedef struct EventTab {				    //mozda korisiti za prikaz
	EVENT** first;							//lista dogadjaja
	USER* current;							//trenutno ulogovan user
}EventTab;

typedef struct Index{  // Struktura za rad sa indeksnim datoteakama
    char eventID[7], key[20];
    int position;
}INDEX;


void createUser(USER*);					//pomocne funkcije za inicijalizovanje
void createEvent(EVENT*);
void createComment(COMMENT*);

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

int openCommentData(FILE**,char*);                      //otvara fajl sa odobrenim komentarima
int loadComment(COMMENT*,FILE*);                        //ucitava jedan komentar
int approveComment(char*,char*);                        //odobrava komentar sa prosljedjenim eventID i username-om korisnika(mozda bi bilo blje da svaki neodobren ima svoj id)
int insertComment(COMMENT*);                            //upisuje komentar u Komentare tako da su svi komentari sa istim eventID grupisani
int deleteUnApprovedComment(char*,char*);               //brise komentar iz fajla neodobrenih komentara
int approveAllComments();                              //sve komentare iz fajla odobrava i upisuje u 'Komentari.txt'
void writeComment(COMMENT*,FILE*);                      //samo formatiran upis komentara u fajl

int removeEvent(char*);                 //brise iz fajla i update-uje indexe(ovu treba koristiti iz menija)
int updateIndex(int,int,char*);         //update-uje position dio svakog zapisa u indeksu, da bi se poklapalo nakon brisanja eventa
void freeUser(USER*);					//pomocne za oslobadjanje memorije
void freeEvent(EVENT*);
void freeUnregUser(UNREGUSER*);
void freeComment(COMMENT*);

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
//prima id dogadjaja, userName i komentar
void addComment(int,char*,char*);

//f-ja prima naziv kategorije
//vraca 0-ako je kategoriju nemoguce obrisati (standardna kategorija)
//vraca 1-ako je kategorija izbrisana
//f-ja ne prepoznaje pravilan naziv kategorije
//pri pozivu f-je morate unijeti tacan naziv kategorije koju hocete da izbrisete
int deleteCategory(char*);

//f-ja prima id, korisnicko ime, komentar
//f-ja ne prepoznaje pravilan id, korisnicko ime ili komentar
//stoga se mora unijeti tacan id, korisnicko ime i komentar
//kako bi se isto izbrisalo iz datoteke
void deleteComment(int, char*, char*);

//unos stringa
char* inputString();
//provjera da li ima slovo u stringu
int checkForAlpha(char*);
//funkcija koja sluzi za izbor opcije od begin do end (ukljucujuci oboje)
int chooseOption2(int, int);
// funkcija za dodavanje kategorije
void addCategory();
//funkcija koja vraca niz stringova koji su nazivi kategorija, a prima pokazivac na broj kategorija
char** readCategories(int*);
// funkcija koja sluzi za odabir kategorije pri dodavanju dogadjaja, prima pokazivac na dogadjaj
void inputCategory(EVENT *);
// funkcija koja vraca string koji predstavlja danasnji datum oblika dd.mm.gggg.
char* currentDate();
// funkcija koja provjerava da li je dogadjaj zavrsen u odnosu na danasnji datum
int isFinished(char*);
//funkcija za unos podataka o dogadjaju
void inputEventData(EVENT *);
//funkcija za generisanje indeksa, prima cijelobrojnu vrijednost, a vraca string oblika %06s
char* generateIndex(int);
//funkcija koja provjerava da li je dogadjaj validan, tj. da li se odrzava nakon vise od pet godina od danasnjeg dana
// ako je validan vratice se 1, ako nije validan vratice se 0
int isEventValid(char*);
//azuriranje indeksa u indeksnim datotekama
void everyIndexPosition(INDEX *, int, const char*);
//promjena pozicija u glavnoj datoteci zvog povecavanja broja cifra indeksa koji je prvi upisan u datoteku
void updateIndex3(int);
//funckija za dodavanje dogadjaja
void addEvent(char*,char*);
//pronalazak pozicije dogadjaja za ID-jem eventID u jednoj od indeksnih datoteka
int findPosition(char *);
//funkcija za promjenu podataka o dogadjaju koji se salje kao parametar
void editEvent (EVENT *, char*);


// funkcija za ispis dogadjaja na standardni izlaz
void stdprint(INDEX *, int);
// funkcija koja ispisuje danasnje dogadjaje
void printCurrentEvents(char*,char*,char*);
// funckija koja ispisuje prosle dogadjaje
void printPreviousEvents(char*,char*,char*);
//funkcija koja ispisuje buduce dogadjaje
void printFutureEvents(char*,char*,char*);
//funkcija koja sluzi za izbor kategorije
char* chooseCategory();
// funkcija koja ispisuje sve dogadjaje iz datoteke events koji su odredjene kateogrije
void printCategoryEvents(char*,char*,char*);
//funkcija koja ispisuje detaljne podatke o dogadjaju
void printDetails(char *eventID, char*, char*,char*);

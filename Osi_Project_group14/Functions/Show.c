#include <string.h>
#include <conio.h>
#include "DataManage.h"
#include "functions.h"
#include "MainMenu.h"
#define _CRT_SECURE_NO_WARNINGS

// u headeru je vecina objasnjenja za funkcije


USER temp;

void head(char* naslov)
{
    printf("****************************************************************************************************\n");		//50 komada (valjda)
    printf("                                        %s\n", naslov);
    printf("****************************************************************************************************\n");
}

int errorUnreg()
{
    char c;
    short t=1;                                                  //promjenjiva koja kontrolise while petlju
    printf("\n\nNe postoji korisnik sa takvim imenom!\n");

    //dok god je unos neodgovarajuci, petlja se ponavlja
    while(t)
    {
        printf("Pokusajte ponovo D/N: ");
        c=getch();
        if(c=='D' || c=='d')
        {
            t=0;
            return 1;
        }
        else if(c=='N' || c=='n')
        {
            t=0;
            return 0;
        }
        else
            printf("Pogresan unos!\n");
    }
    return 0;                                                           //stoji samo da bi izbjegao warning, ali nikada nece doci do toga
}

int errorWrong()
{
    char c;
    short t=1;                                                          //promjenjiva koja kontrolise while petlju

    printf("\n\nPogresna lozinka!\n");

    //dok god je unos neodgovarajuci, petlja se ponavlja
    while(t)
    {
        printf("Pokusajte ponovo D/N: ");
        c=getch();
        if(c=='D' || c=='d')
        {
            t=0;
            return 1;
        }
        else if(c=='N' || c=='n')
        {
            t=0;
            return 0;
        }
        else
            printf("Pogresan unos!\n");
    }
    return 0;                                                           //stoji samo da bi izbjegao warning, ali nikada nece doci do toga
}


void showOptions(char* choice)
{
    printf("1) Prijavite se\n");
    printf("2) Registrujte se\n");
    printf("3) Nastavi kao gost\n");
    printf("4) Izadji\n");
    printf("Unos: ");
    scanf("%c",choice);
}

int checkUserData(char* usrn,char* passw, char* type)
{
    FILE* users;
    int id;

    createUser(&temp);
    openUserData(&users,"r+");
    getId(&id, users);

    while (!feof(users))
    {
        loadUser(&temp, users);
        if (!strcmp(usrn, temp.userName))
        {
            if (!strcmp(passw, temp.password))
            {
                return 1;
                strcpy(type, temp.type);
            }
            else
            {
                return -1;
            }
        }
    }
    return 0;
    fclose(users);
}

void readPassword(char* password)
{
    char c;
    int k;
    int complete=0;
    int backSpace=0;

    for (k = 0; !complete ; backSpace=0)
    {
        c = getch();
        if (c == 13)
        {
            complete=1;
            backSpace=1;
        }
        else if (c == 010)
        {
            if(k!=0)
            {
                printf("\b \b");
                password[k]=0;
                k--;
                backSpace=1;
            }
            else
            {
                backSpace=1;
            }
        }
        //sifra je ogranicena na 20 karaktera
        if(!backSpace && k<20)
        {
            password[k] = c;
            c = '*';
            printf("%c", c);
            k++;
        }
    }
    password[k] = '\0';
}

void logInForm(char* user)
{

    head("Prijava");

    printf("Korisnicko ime: ");
    scanf("%s",user);
    printf("Sifra: ");
}

int userLogIn(char cityName[], char *userName, char *type)
{
    char* user = (char*)calloc(21, sizeof(char));
    char* password = (char*)calloc(21, sizeof(char));
    int i, whileNotLoged;
    whileNotLoged = 1;


    //u slucaju da nastane greska pri prijavljivanju
    //korisnik moze ponovo pokusati da se prijavi ("whileNotLoged")
    //ili se vratiti na pocetnu stranicu
    while(whileNotLoged)
    {
        newPage(cityName);
        logInForm(user);

        //ovdje se pocinje unositi sifra

        readPassword(password);

        //ovdje se provjerava da li je korisnik vec registrovan

        if (!(i = checkUserData(user, password, type)))
            whileNotLoged = errorUnreg();
        else if (i == -1)
            whileNotLoged = errorWrong();
        else
            whileNotLoged = 0;
    }
    if(i<1)
        i = 0;
    else
    {
        strcpy(userName, user);
    }


    free(user);
    free(password);

    return i;
}

void registerLayout(char cityName[])
{
    newPage(cityName);
    head("Registracija");

    USER forReading;
    createUser(&forReading);
    char ime[20];
    char prezime[21];
    char email[40];
    char username[20];
    char password[20];
    char c;
    short t=1,find, loop=1;                                 //t je loop, za potvrdu slanja registracije, find pronalazi korisnicko ime, loop je loop u slucaju da postoji korisnicko ime
    int id;
    FILE *inputRequest, *readFile;

    printf("Ime: ");
    scanf("%s", ime);
    printf("Prezime: ");
    scanf("%s", prezime);
    printf("E-mail: ");
    scanf("%s", email);

    readFile=fopen("Data/Users.txt","r");

    //ovaj loop ce se ponavljati dok god postoji korisnika sa istim korisnickim imenom
    while(loop)
    {
        if(readFile!=NULL)
        {
            find = 0;
            printf("Korisnicko ime: ");
            scanf("%s", username);

            rewind(readFile);
            getId(&id,readFile);

            //loop staje kada nadje isto korisnicko ime ili kad dodje do kraja fajla
            while(!find && loadUser(&forReading,readFile))
                if(!strcmp(username,forReading.userName))
                {
                    printf("\nPostoji korisnik sa istim korisnickim imenom\n");
                    printf("Molimo vas unesite drugo korisnicko ime\n\n");
                    find = 1;
                }
            if(!find)
                loop=0;
        }
    }
    fclose(readFile);

    printf("Sifra: ");
    readPassword(password);
    printf("\n");

    inputRequest=fopen("Data/Korisnicki_zahtjevi.txt","a");


    //loop traje dok god je unos za slanje zahtjeva nepravilan
    while(t)
    {
        printf("Da li zelite da posaljete zahtijev (D/N): ");
        c=getch();

        if(c=='D' || c=='d')
        {
            if(inputRequest!=NULL)
            {
                fprintf(inputRequest,"%s %s %s %s %s\n",username,password,email,ime,prezime);
            }
            t=0;
            printf("VAS ZAHTIJEV JE POSLAN\nKADA NALOG BUDE ODOBREN BICETE OBAVJESTENI\n");
            printf("\nZa izlaz pritisnite neko slovo na tastaturi");
            getch();

        }
        else if(c=='N' || c=='n')
            t=0;
        else
            printf("Pogresan unos!\n");
    }

    fclose(inputRequest);
    return;
}

void chooseOption(char cityName[])
{
    short choiceLoop = 0, isLoged;
    char *userName=(char*)calloc(21,sizeof(char));
    char *type=(char*)calloc(7,sizeof(char));


    //choiceLoop nam omogucava da se vratimo na pocetni ekran
    //ukoliko napustimo ekran za prijavu ili ekran za registraciju
    while(!choiceLoop)
    {
        char choice;
        newPage(cityName);
        showOptions(&choice);

        if(choice!='\n')
            while (choice < '1' || choice>'4')
            {
                newPage(cityName);
                printf("\n=== MOLIMO VAS DA IZABERETE NEKU OD POSTOJECIH OPCIJA ===\n\n");
                showOptions(&choice);
            }

        switch(choice)
        {
        case '1':
        {
            isLoged = userLogIn(cityName,userName,type);
            if(!isLoged)
                choiceLoop = 0;
            else
            {showMainMenu(cityName,type,userName);}
        }
        break;
        case '2':
            registerLayout(cityName);
            break;
        case '3':
            {
                showMainMenu(cityName,"guest","Gost1");
            };
        break;
        case '4': return;
        default:
            break;
        }
    }

    fflush(stdin);                              //pojma nemam sta je ovo, ostalo od dejana

}


void inputQuizQuestions(){
    FILE *fp=fopen("kviz.txt", "w");  // otvorimo datoteku za pitanja
    char *question, *a1, *a2, *a3;  // varijable za pitanje i ponudjene odgovore
    int i, ca;  // ca-tacan odgovor
    if(fp!=NULL){
        /*Unos pitanja sa ponudjenim odgovorima i tacan odgovor, zatim upis u datoteku*/
        for(i=0; i<10; i++){
            printf("Unesite pitanje: ");
            question=inputString(); question=(char*)realloc(question, size*sizeof(char));
            printf("1. ");
            a1=inputString(); a1=(char*)realloc(a1, MAX*sizeof(char));
            printf("2. ");
            a2=inputString(); a2=(char*)realloc(a2, MAX*sizeof(char));
            printf("3. ");
            a3=inputString(); a3=(char*)realloc(a3, MAX*sizeof(char));
            printf("Unesite redni broj tacnog odgovora: ");
            ca=chooseOption2(1,3);
            fprintf(fp, "%d) %s\n1. %s\n2. %s\n3. %s\n%d\n", i+1, question, a1, a2, a3, ca);
            free(question); free(a1); free(a2); free(a3);
        }
    }
    else printf("ERROR!");
}

void Help(){
    printf("\n\n                              POMOC");
    printf("\n -------------------------------------------------------------------------");
    printf("\n >> Kviz zapocinjete pritiskom na taster enter.");
    printf("\n >> Nakon sto ste pritisnuli enter, mozete zapoceti igranje kviza koji se sastoji");
    printf("\n    od 10 pitanja. Svako pitanje ide jedno ispod drugog. Nakon svakog pitanja ");
    printf("\n    imate ponudjene odgovore pod 1, 2 i 3. Moguce je unijeti samo jedan broj, tj.");
    printf("\n    samo jedan odgovor jer za svako pitanje postoji jedinstven odgovor.");
    printf("\n    Svako tacno pitanje nosi 1 bod. Ne postoje negativni bodovi.");
    printf("\n    Kada se zavrsi kviz, odnosno kada zavrsite svih 10 pitanja dobicete ");
    printf("\n    ukupan broj osvojenih bodova.");
    printf("\n >> Srecno!\n");
    printf("\n\n");
}

void Display(){
     printf("\t\t________________________________________");
     printf("\n\t\t\t  DOBRODOSLI ");
     printf("\n\t\t\t      U ");
     printf("\n\t\t\t    KVIZ ");
     printf("\n\t\t________________________________________");
     printf("\n\t\t > Pritisnite slovo P za pomoc ukoliko zelite da saznate pravila igre.");
     printf("\n\t\t > Pritisnite enter za zapocinjanje kviza.");
     printf("\n\t\t________________________________________\n\n");
     char option;
     option=toupper(getch());
      if(option=='P')
      {
          Help();
          getch();
      }
}

void playQuiz(){
    Display();  //prikaz pocetnog ekrana
    printf("ZAPOCELI STE KVIZ\n\n");
    FILE *fp=fopen("kviz.txt", "r");  // otvaranje fajla sa pitanjima
    char *question=(char*)calloc(size, sizeof(char)), *a1=(char*)calloc(MAX, sizeof(char)), *a2=(char*)calloc(MAX, sizeof(char)), *a3=(char*)calloc(MAX, sizeof(char));
    int in, ca, s=0;  // in - unos odgovora, ca - tacan odgovor, s - broj tacnih odgovora
    if(fp!=NULL){
        /*Iscitavanje pitanja i ponudjenih odgovora iz datoteke sa pitanjima
        ispis na standardni izlaz i unos tacnog odgovora*/
        while(!feof(fp)){
            fscanf(fp, "%[^\n]\n", question);
            fscanf(fp, "%[^\n]\n", a1);
            fscanf(fp, "%[^\n]\n", a2);
            fscanf(fp, "%[^\n]\n", a3);
            fscanf(fp, "%d\n", &ca);
            printf("\n%s\n%s\n%s\n%s\n\n", question, a1, a2, a3);
            printf("Unesite odgovor: ");
            in=chooseOption2(1,3);
            if(in==ca) s++;  // ako je uneseni odgovor tacan povecava se broj tacnih odgovora
        }
        fclose(fp);
    }
    else printf("ERROR!");
    printf("Broj tacnih odgovora je: %d.", s);
    free(question); free(a1); free(a2); free(a3);
    Sleep(5*1000);
}


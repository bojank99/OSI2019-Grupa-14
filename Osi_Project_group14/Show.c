#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <conio.h>
#include "Show.h"
#include "DataManage.h"
#include"functions.h"
#define _CRT_SECURE_NO_WARNINGS

// u headeru je vecina objasnjenja za funkcije


USER temp;

void head(char* naslov)
{
    printf("/n====================================================================================================\n");		//50 komada (valjda)
    printf("                                        %s\n");
    printf("====================================================================================================\n");
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
        c=getc();
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
}

void errorWrong()
{
    char c;
    short t=1;                                                          //promjenjiva koja kontrolise while petlju

    printf("\n\nPogresna lozinka!\n");


    //dok god je unos neodgovarajuci, petlja se ponavlja
    while(t)
    {
        printf("Pokusajte ponovo D/N: ");
        c=getc();
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
}


void showOptions(int* choice)
{
    printf("1) Prijavite se\n");
    printf("2) Registrujte se\n");
    printf("3) Nastavi kao gost\n");
    printf("Unos: ");
    scanf("%d", choice);
}

int userData(char* usrn,char* passw)
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

void logInForm(char* user)
{


    head("Prijava");

    printf("Korisnicko ime: ");
    gets(user);
    printf("Sifra: ");
}

int userLogIn()
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
        logInForm(user);

        //ovdje se pocinje unositi sifra

        int p=0;

        do
        {
            password[p++]=getch();
            if(password[p]!='\r')
                printf("*");
        }
        while(password[p-1]!='\r'  || p==21);

        password[p-1]='\0';

        //ovdje se provjerava da li je korisnik vec registrovan

        if (!(i = userData(user, password)))
            whileNotLoged = errorUnreg();
        else if (i == -1)
            whileNotLoged = errorWrong();
        else
            whileNotLoged = 0;
    }

    free(user);
    free(password);

    return i;
}

void chooseOption(char cityName[])
{
    int choice;

    showOptions(&choice);

    while (choice < 1 || choice>4)
    {
        newPage(cityName);
        printf("\n=== MOLIMO VAS DA IZABERETE NEKU OD POSTOJECIH OPCIJA ===\n\n");
        showOptions(&choice);
    }

    switch(choice)
    {
    case 1:
    {
        newPage(cityName);
        userLogIn();
    }
    break;
        //case 2: registerLayout();
    break;
        //case 3: guestLogIn();
    break;
    default:
        break;
    }


    fflush(stdin);

}


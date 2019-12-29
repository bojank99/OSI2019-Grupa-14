#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <conio.h>
#include "Show.h"
#include "DataManage.h"
#include "functions.h"
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


void showOptions(int* choice)
{
    printf("1) Prijavite se\n");
    printf("2) Registrujte se\n");
    printf("3) Nastavi kao gost\n");
    printf("Unos: ");
    scanf("%d", choice);
}

void passBackspace(int* k, char* password) {
	printf("\r");                                           //vracanje na pocetak linije
	printf("Sifra: ");
	for (int i = 0; i < *k - 1; printf("*"), i++);          //Ispisivanje jedne manje zvjezdice
	printf(" ");                                            //Blanko da prekrije zadnju
	printf("\r");
	printf("Sifra: ");
	for (int i = 0; i < *k - 1; printf("*"), i++);
	if (*k != 0) { (*k) -= 2; }
	else {                                                  //ako nema sta brisati
		password[0] = '\0'; (*k)--; printf("\rSifra: ");
	}
            //Uglavnom, dva puta pravi ispis i mijenja string password

}

int checkUserData(char* usrn,char* passw)
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
    scanf("%s",user);
    printf("Sifra: ");
}

int userLogIn(char cityName[])
{
    char* user = (char*)calloc(21, sizeof(char));
    char* password = (char*)calloc(21, sizeof(char));
    char c;
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

        int p=0;

        for (p = 0; p < 19; p++)
		{
			c = _getch();

			if (c == 13) { break; }
			else if (c == 010) { passBackspace(&p, password); continue; }
			password[p] = c;
			c = '*';
			printf("%c", c);
		}
		password[p] = '\0';

        //ovdje se provjerava da li je korisnik vec registrovan

        if (!(i = checkUserData(user, password)))
            whileNotLoged = errorUnreg();
        else if (i == -1)
            whileNotLoged = errorWrong();
        else
            whileNotLoged = 0;
    }
    if(i<1)
        i = 0;

    free(user);
    free(password);

    return i;
}

void chooseOption(char cityName[])
{
    int choice;
    short choiceLoop = 0, isLoged;


    //choiceLoop nam omogucava da se vratimo na pocetni ekran
    //ukoliko napustimo ekran za prijavu ili ekran za registraciju
    while(!choiceLoop)
    {
        newPage(cityName);
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
            isLoged = userLogIn(cityName);
            if(!isLoged) choiceLoop = 0;
            //else showEvents
        }
        break;
            //case 2: registerLayout();
        break;
            //case 3: guestLogIn();
        break;
        default:
            break;
        }
    }

    fflush(stdin);                              //pojma nemam sta je ovo, ostalo od dejana

}

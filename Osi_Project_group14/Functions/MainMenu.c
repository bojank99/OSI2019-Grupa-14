#include "MainMenu.h"



int checkType(char userType[])
{
    if (strcmp(userType, "guest")==0)
        return 1;
    else if(strcmp(userType, "user")==0)
        return 2;
    else
        return 3;
}

void showMainMenu(char userType[], char cityName[])
{
    int type=checkType(userType);
    int choice;
    short loop=1;

    while(loop)
    {
        newPage(cityName);
        switch(type)
        {
        case 1:
        {
            printf("==================================================\n");
            printf(" 1)Prikaz dogadjaja\n 2)Igranje kviza\n");
            printf("==================================================\n");
            printf("\nUnos: ");

            do
            {
                scanf("%d", &choice);
            }
            while(choice <1 || choice>2);

            switch(choice)
            {
            case 1: /*prikaz dogadjaja*/
                break;
            case 2: /*igranje kviza*/
                break;
            default :
                ;
            }




        }
        break;
        case 2:
        {
            printf("==================================================\n");
            printf(" 1)Prikaz dogadjaja\n 2)Igranje kviza\n 3)Kreiraj dogadjaj\n 4)Brisanje kategorije\n");
            printf("==================================================\n");
            printf("\nUnos: ");
            do
            {
                scanf("%d", &choice);
            }
            while(choice <1 || choice>4);
            switch(choice)
            {
            case 1: /*prikaz dogadjaja*/
                break;
            case 2: /*igranje kviza*/
                break;
            case 3: /*kreiraj dogadjaj*/
                break;
            case 4: //brisanje kategorije
                break;
            default:
                ;
            }
        }
        break;
        case 3:
        {
            printf("==================================================\n");
            printf(" 1)Prikaz dogadjaja\n 2)Igranje kviza\n 3)Kreiraj dogadjaj\n 4)Kreiraj kviz\n 5)Brisanje kategorije\n");
            printf(" 6)Brisanje komentara\n 7)Brisanje dogadjaja\n 8)Odobravanje komentara\n 9)Odobravanje korisnika\n")
            printf("==================================================\n");
            printf("\nUnos: ");
            do
            {
                scanf("%d", &choice);
            }
            while(choice <1 || choice>9);
            switch(choice)
            {
            case 1: /*prikaz dogadjaja*/
                break;
            case 2: /*igranje kviza*/
                break;
            case 3: /*kreiraj dogadjaj*/
                break;
            case 4: /*kreiraj kviz*/
                break;
            case 5: //beisanje kategorije
                break;
            case 6: //brisanje komentara
                break;
            case 7: //brisanje dogadjaja
                break;
            case 8: //odobravanje komentara
                break;
            case 9: //odobravanje korisnika
                break;
            default :
                ;
            }
        }

        default:
            ;
        }
    }
}



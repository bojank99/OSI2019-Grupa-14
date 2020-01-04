#include "MainMenu.h"



int checkType(USER* user)
{
    if (strcmp(user->type, "guest")==0)
        return 1;
    else if(strcmp(user->type, "user")==0)
        return 2;
    else
        return 3;
}

void showMainMenu(USER* user)
{
    int type=checkType(user);
    int choice;
    switch(type)
    {
    case 1:
    {
        printf("==================================================\n");
        printf(" 1)Prikazi dogadjaje\n 2)Igranje kviza\n");
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
        printf(" 1)Prikazi dogadjaje\n 2)Igranje kviza\n 3)Napravi dogadjaj\n");
        printf("==================================================\n");
        printf("\nUnos: ");
        do
        {
            scanf("%d", &choice);
        }
        while(choice <1 || choice>3);
        switch(choice)
        {
        case 1: /*prikaz dogadjaja*/
            break;
        case 2: /*igranje kviza*/
            break;
        case 3: /*kreiraj dogadjaj*/
            break;
        default:
            ;
        }
    }
    break;
    case 3:
    {
        printf("==================================================\n");
        printf(" 1)Prikazi dogadjaje\n 2)Igranje kviza\n 3)Kreiraj dogadjaj\n 4)Kreiraj kviz\n");
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
        case 4: /*kreiraj kviz*/
        default :
            ;
        }
    }

    default:
        ;
    }
}



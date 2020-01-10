#include "MainMenu.h"

void menuUserApprove()
{
    char approveChoice;
    char* approveUserName = (char*)calloc(21,sizeof(char));
    UNREGUSER user;
    allocUnregUser(&user);
    FILE *fp;
    openUnregUserData(&fp,"r");
    printf("korisnici za odobravanje:\n");
    while(loadUnregUser(&user,fp))
    {
        printf("%s %s %s %s\n", user.name, user.surname, user.base.userName, user.email);
    }
    fclose(fp);
    printf("===============================================\n");
    printf("1) Odobri sve\n2) Odobri samo jednog korisnika\n");
    printf("===============================================\n");
    approveChoice=getch();
    fflush(stdin);
    switch(approveChoice)
    {
    case '1':
        approveAllUsers();
        break;
    case '2':
    {
        printf("Unesite korisnicko ime: ");
        gets(approveUserName);
        fflush(stdin);
        if(!approveUser(approveUserName))
            printf("Greska!");

    }
    break;
    default:
        printf("Pogresan unos\n");
    }
}
int checkType(char userType[])
{
    if (strcmp(userType, "guest")==0)
        return 1;
    else if(strcmp(userType, "user")==0)
        return 2;
    else
        return 3;
}

void showMainMenu(char cityName[], char userType[], char userName[])
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
            printf(" 1)Prikaz dogadjaja\n 2)Igranje kviza\n 3)Vrati se\n");
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
            case 3: return;
            default :
                ;
            }




        }
        break;
        case 2:
        {
            printf("==================================================\n");
            printf(" 1)Prikaz dogadjaja\n 2)Igranje kviza\n 3)Kreiraj dogadjaj\n 4)Brisanje kategorije\n");
            printf(" 5)Odjavi se\n");
            printf("==================================================\n");
            printf("\nUnos: ");
            do
            {
                scanf("%d", &choice);
            }
            while(choice <1 || choice>5);
            switch(choice)
            {
            case 1: /*prikaz dogadjaja*/
                break;
            case 2: /*igranje kviza*/
                break;
            case 3: addEvent();
                break;
            case 4: //brisanje kategorije
                break;
            case 5: return;
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
            printf(" 6)Brisanje komentara\n 7)Brisanje dogadjaja\n 8)Odobravanje komentara\n 9)Odobravanje korisnika\n");
            printf(" 10) Odjavi se\n");
            printf("==================================================\n");
            printf("\nUnos: ");
            do
            {
                scanf("%d", &choice);
            }
            while(choice <1 || choice>10);
            switch(choice)
            {
            case 1: /*prikaz dogadjaja*/
                break;
            case 2: /*igranje kviza*/
                break;
            case 3: {fflush(stdin); addEvent();}
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
            case 9: {
                        newPage(cityName);
                        menuUserApprove();
            }
                break;
            case 10: return;
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



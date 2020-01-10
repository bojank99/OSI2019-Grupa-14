#include "MainMenu.h"
#include<stdlib.h>
#include<conio.h>

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

void menuCommentApprovel()
{
    char approveUserName[21];
    char eventId[6];
    char approveChoice;
    COMMENT comm;
    createComment(&comm);
    FILE *fp;
    openUnApprovedComments(&fp,"r");
    printf("Komentari za odobravanje:\n");
    while(loadComment(&comm,fp))
    {
        printf("%s %s %s\n", comm.comEventID,comm.comUsername,comm.commentText);
        freeComment(&comm);
        createComment(&comm);
    }
    fclose(fp);
    printf("===============================================\n");
    printf("1) Odobri sve\n2) Odobri samo jedan komentar\n");
    printf("===============================================\n");
    approveChoice=getch();
    fflush(stdin);
    switch(approveChoice)
    {
    case '1':
        approveAllComments();
        break;
    case '2':
    {
        printf("Unesite ID dogadjaja: ");
        gets(eventId); fflush(stdin);
        printf("Unesite korisnicko ime: ");
        gets(approveUserName);
        fflush(stdin);
        if(!approveComment(eventId,approveUserName))
            printf("Greska!");
    }
    break;
    default:
        printf("Pogresan unos\n");
    }
    freeComment(&comm);
}

void menuDeleteCategory(char* cityName)
{
    newPage(cityName);
    int n=0, i;
    char **s;
    printf("Postojece kategorije:\n");
    s=readCategories(&n); // iscitamo sve kategorije i ispisemo ih
    for(i=0; i<n; i++)
        printf("%d. %s\n", i+1, s[i]);
    printf("Izaberite opciju: ");
    i=chooseOption2(1, n);  // izbor opcije
    fflush(stdin);
    if(!deleteCategory(s[i-1]))
    {
        printf("Nemoguce izbrisati kategoriju!\n");
        for(i=0; i<n; i++)
        free(s[i]);
        free(s);
        printf("Kliknite za izlaz\n");
        getch();
        return;
    };
    for(i=0; i<n; i++)
        free(s[i]);
    free(s);
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
                fflush(stdin);
            }
            while(choice <1 || choice>3);

            switch(choice)
            {
            case 1: {
                printCurrentEvents(cityName,userType,userName);
            }
                break;
            case 2: playQuiz();
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
                fflush(stdin);
            }
            while(choice <1 || choice>5);
            switch(choice)
            {
            case 1: printCurrentEvents(cityName,userType,userName);
                break;
            case 2: playQuiz();
                break;
            case 3: {fflush(stdin); addEvent(cityName,userName);}
                break;
            case 4: menuDeleteCategory(cityName);
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
            printf(" 1)Prikaz dogadjaja\n 2)Igranje kviza\n 3)Kreiraj dogadjaj\n 4)Brisanje kategorije\n");
            printf(" 5)Odobravanje komentara\n 6)Odobravanje korisnika\n");
            printf(" 7)Odjavi se\n");
            printf("==================================================\n");
            printf("\nUnos: ");
            do
            {
                scanf("%d", &choice);
                fflush(stdin);
            }
            while(choice <1 || choice>8);
            switch(choice)
            {
            case 1:
                {
                    printCurrentEvents(cityName,userType,userName);

                }
                break;
            case 2: {newPage(cityName); playQuiz();}
                break;
            case 3: {fflush(stdin); addEvent(cityName,userName);}
                break;
            case 4: menuDeleteCategory(cityName);
                break;
            case 5: {
                newPage(cityName);
                menuCommentApprovel();
            }
                break;
            case 6: {
                newPage(cityName);
                menuUserApprove();
            }
                break;
            case 7: return;
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



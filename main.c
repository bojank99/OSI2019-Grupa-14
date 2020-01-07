#include <stdio.h>
#include <stdlib.h>
#include <string.h>
void filtering(FILE* eventFile)
{
    char eventID[1000];
    char headline[1000];
    char category[1000];
    char description[10000];
    char date[1000];
    char user[1000];
    char kat[1000],dat[1000];//Stavljam pomocne za filtriranje
    int i,pom=0;//pom cu uvecati ako je dobar unos
    do
    {
        printf("Izaberite opciju za filtriranje:1(po kategoriji) 2(datumu) 3(kategoriji i datumu)");
        scanf("%d",&i);
        if(i==1)
        {
            printf("Unesite kategoriju:");
            scanf("%s",kat);
            pom++;
        }
        else if(i==2)
        {
            printf("Unesite datum:");
            scanf("%s",dat);
            pom++;
        }
        else if(i==3)
        {
            printf("Unesite datum:");
            scanf("%s",dat);
            printf("Unesite kategoriju:");
            scanf("%s",kat);
            pom++;
        }
        else
            printf("Greska! Unesite ponovo!\n");
    }
    while(pom==0);


    while(fscanf(eventFile,"%s %s %s| %s| %s %s %d\n",eventID, user, headline, description, date, category, &finished)!=EOF)
    {
        if(i==1)
        {
            if(strcmp(category,kat)==0)
                printf("%s %s %s %s %s\n",user, headline, description, date, category);
        }
        else if(i==2)
        {
            if(strcmp(date,dat)==0)
                printf("%s %s %s %s %s\n",user, headline, description, date, category);
        }
        else if(i==3)
        {
            if((strcmp(category,kat)==0)&&(strcmp(date,dat)==0))
                printf("%s %s %s %s %s\n",user, headline, description, date, category);
        }
    }
}


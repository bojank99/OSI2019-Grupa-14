#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <string.h>

typedef struct Event {
	char* eventID;
	char* headline;
	char* category;
	char* description;
	char* date;
	char* user;
	int finished;					//kao bool
	struct Event* next;
}EVENT;


typedef struct comment{                     //struktura za komentar
    char* comEventID;
    char* comUsername;
    char* commentText;
}COMMENT;



void addComment(int eventsId,char* userName, char* comment)
{
    FILE *filComm;
    filComm = fopen("Data/Komentari.txt","a");
    if(filComm!=NULL)
    {
        fprintf(filComm, "%d,%s,%s\n",eventsId,userName,comment);
    }
    fclose(filComm);

}



int loadEvent(EVENT* eve, FILE* eventFile) {
	int i = 0;
	char tmp;
	if (!feof(eventFile)) {
		fscanf(eventFile, "%s %s" , eve->eventID, eve->user);
		while ((tmp = fgetc(eventFile)) != '|' && tmp != EOF){
			if (tmp != '\n') {
				eve->headline[i] = tmp;
				i++;
			}
		}fgetc(eventFile); i = 0;
		while ((tmp = fgetc(eventFile)) != '|' && tmp != EOF) {
			if (tmp != '\n') {
				eve->description[i] = tmp;
				i++;
			}
		}
		fscanf(eventFile, "%s %s ", eve->date, eve->category);
	}
	return 1;
}

void printDetails(char *eventID, EVENT* eve, COMMENT* com, FILE* eventFile, FILE* comFile)
{
    while(loadEvent(eve,eventFile))
    {
        if(strcmp(eve->eventID,eventID)==0)
        {
            printf("Korisnik koji je kreirao dogadjaj: %s\n",eve->user);
            printf("Naziv dogadjaja: %s\n",eve->headline);
            printf("Datum: %s\n",eve->date);
            printf("Kategorija: %s",eve->category);
            printf("Opis: %s\n",eve->description);
        }
    }

    while(fscanf(comFile, "%s,%s,%[^\n]\n",com->comEventID,com->comUsername,com->commentText)!=EOF)
    {
        if(strcmp(com->comEventID,eventID)==0)
        {
            printf("%s:",com->comUsername);
            printf("%s\n",com->commentText);
        }
    }

}

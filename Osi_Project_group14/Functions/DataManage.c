#include "DataManage.h"
#include <stdlib.h>
#include <string.h>
#pragma warning(disable:4996)												//za VS, da se ignorisu upozorenja za fscanf i slicne f-je

void createUser(USER* temp)
{
	temp->userID = (char*)calloc(6, sizeof(char));
	temp->userName = (char*)calloc(20, sizeof(char));
	temp->password = (char*)calloc(20, sizeof(char));
	temp->type = (char*)calloc(11, sizeof(char));
	temp->next = 0;
}
void createEvent(EVENT* eve) {
	eve->eventID = (char*)calloc(7, sizeof(char));
	eve->headline = (char*)calloc(35, sizeof(char));
	eve->description = (char*)calloc(250, sizeof(char));
	eve->date = (char*)calloc(11, sizeof(char));
	eve->user = (char*)calloc(20, sizeof(char));
	eve->category = (char*)calloc(15, sizeof(char));
}


int openUserData(FILE** users, char* mode) {
	return ((*users) = fopen("Data/Users.txt", mode));
}
int openEventData(FILE** events, char* mode) {
	return ((*events) = fopen("Data/Events.txt",mode));
}


int loadUser(USER* user,  FILE* userFile) {
	if (!feof(userFile)) {
        fscanf(userFile, "%s %s %s %s", user->userID, user->userName, user->password, user->type);
        return 1;
	}
	return 0;
}
int loadEvent(EVENT* eve, FILE* eventFile) {
	int i = 0;
	char tmp;
	if (!feof(eventFile)) {
		fscanf(eventFile, "%s %s ", eve->eventID, eve->user);
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
		fscanf(eventFile, "%s %s %d", eve->date, eve->category, &eve->finished);
	}else{ return 0;}
	return 1;
}


void writeUser(USER* user,  FILE* userFile) {								//podrazumjevaju da su korisnici/dogadjaji konacno formirani
	fprintf(userFile, "%05d %s %s %s \n", atoi(user->userID), user->userName, user->password, user->type);
}
void writeEvent(EVENT* eve,  FILE* eventFile) {
	fprintf(eventFile, "%s %s %s| %s| %s %s %d \n",eve->eventID, eve->user, eve->headline, eve->description, eve->date, eve->category, eve->finished);
}

int appendUser(USER* user,FILE* userFile){                  //ova dodaje korisnika na kraj sa racunanjem id-a i ponovnim upisivanjem id-a na pocetak
    int id;
    char idC[6];

    strcpy(user->type,"user");

    if(!userFile){return 0;}
    rewind(userFile);
    getId(&id,userFile);
    id++;
    rewind(userFile);
    itoa(id,idC,10);
    strcpy(user->userID,idC);
    fprintf(userFile,"%05d",id);
    fseek(userFile,0,SEEK_END);
    writeUser(user,userFile);
    //fclose(userFile);
    return 1;
}

int deleteUser(char* userid) {												//NOTE: predpostavlja se da je fajl uspjesno otvoren
	FILE* userFile;
	USER* temp;
	USER* first = 0;														//head cvor liste
	USER* curr = 0;
	int found = 0; int id; int tmp = atoi(userid);							//kao bool, da li je nadjen takav korisnik

	if (!openUserData(&userFile, "r+")) { return 0; }
	getId(&id, userFile);

	if (tmp > id || tmp < 0) {												//ako ostanje nacin pravljenja id-a
		return 0;
	}

	while (!feof(userFile)) {												//poslije ce biti napisana posebna funkcija za formiranje/upis liste za event i user
		if (first == 0) {
			first = (USER*)calloc(1, sizeof(USER));
			createUser(first);
			loadUser(first, userFile);
			curr = first;
		}
		if (!strcmp(userid, curr->userID)) {
			found = 1;
			freeUser(curr);
			createUser(curr);
			loadUser(curr,userFile);
		}
		else {
			curr->next = (USER*)calloc(1, sizeof(USER));
			createUser(curr->next);
			loadUser(curr->next, userFile);
			curr = curr->next;
		}

	}

	curr=first;
	if (!first->next || !first) {
		return 0;
	}
	if (found) {
		fclose(userFile);
		openUserData(&userFile, "w+");
		fseek(userFile,0,SEEK_SET);
		fprintf(userFile, "%05d\n", id);
		while (curr) {
			temp = curr->next;
			writeUser(curr, userFile);
			freeUser(curr);
			free(curr);
			curr = temp;
		}
	}else{
        while(curr){
            temp = curr->next;
			freeUser(curr);
			free(curr);
			curr = temp;
        }
	}
	fclose(userFile);
	return found;
}


int deleteEvent(char* eventId)
{
	FILE* eventFile;
	int found = 0; int id; int tmp = atoi(eventId);
	EVENT* first = 0;
	EVENT* curr = 0;
	EVENT* temp = 0;

	openEventData(&eventFile,"r+");
	getId(&id, eventFile);

	if (tmp > id || tmp < 0) {											//ako ostanje id
		return 0;
	}

	while (!feof(eventFile)) {											//bice funkicja napisana za listu i to
		if (!first) {
			first = (EVENT*)calloc(1, sizeof(EVENT));
			createEvent(first);
			loadEvent(first, eventFile);
			curr = first;
		}
		if (!strcmp(eventId, curr->eventID)) {
			loadEvent(curr, eventFile);
			found = 1;
		}
		else {
			curr->next = (EVENT*)calloc(1, sizeof(EVENT));
			createEvent(curr->next);
			loadEvent(curr->next, eventFile);
			curr = curr->next;
		}
	}
	fclose(eventFile);
	curr = first;
	if (!first->next || !first) {
		return 0;
	}
	if (found) {
		openEventData(&eventFile, "w+");
		fprintf(eventFile, "%d\n", id);
		while (curr) {
			temp = curr->next;
			writeEvent(curr, eventFile);
			freeEvent(curr);
			free(curr);
			curr = temp;
		}
	}else{
        while(curr){
            temp = curr->next;
			freeEvent(curr);
			free(curr);
			curr = temp;
        }
	}
	fclose(eventFile);
	return found;
}


//Za unreg korisnike
int openUnregUserData(FILE** unregFile,char* mode){
    return ((*unregFile)=fopen("Data/Korisnicki_zahtjevi.txt",mode));
}

void allocUnregUser(UNREGUSER* unUser){
    createUser(&(unUser->base));
    unUser->name=(char*)calloc(20,sizeof(char));
    unUser->surname=(char*)calloc(25,sizeof(char));
    unUser->email=(char*)calloc(22,sizeof(char));
    unUser->next=0;
}

int loadUnregUser(UNREGUSER* unrUser,FILE* unregUserFile){
    if(!feof(unregUserFile)){
        return fscanf(unregUserFile,"%s %s %s %s %s",unrUser->base.userName,unrUser->base.password,
               unrUser->email,unrUser->name,unrUser->surname);
    }
    return 0;
}

void writeUnregUser(UNREGUSER* unrUser,FILE* unUserFile){
    fprintf(unUserFile,"%s %s %s %s %s\n",unrUser->base.userName,unrUser->base.password,
               unrUser->email,unrUser->name,unrUser->surname);
}

int deleteUnregUser(char* usrName){
    FILE* unUserFile;
    UNREGUSER* first = 0;
	UNREGUSER* curr = 0;
	UNREGUSER* temp = 0;
    UNREGUSER* prelast=0;           //da omoguci brisanje zadnjeg
    int found=0;

    if(!openUnregUserData(&unUserFile,"r+")){return 0;}

    while(!feof(unUserFile)){
        if(first==0){
            first=(UNREGUSER*)calloc(1,sizeof(UNREGUSER));
            allocUnregUser(first);
            loadUnregUser(first,unUserFile);
            curr=first;
        }
        if(!strcmp(usrName,curr->base.userName)){
            found=1;
            freeUnregUser(curr);
            allocUnregUser(curr);
            if(!loadUnregUser(curr,unUserFile)){
                prelast->next;
            }
        }else{
            curr->next=(UNREGUSER*)calloc(1,sizeof(UNREGUSER));
            allocUnregUser(curr->next);
            loadUnregUser(curr->next,unUserFile);
            prelast=curr;
            curr=curr->next;
        }
    }

    curr=first;
    if(!first->next && found){
        openUnregUserData(&unUserFile,"w+");
        fprintf(unUserFile,"");
        fclose(unUserFile);
    }
    if(found){
        fclose(unUserFile);
        openUnregUserData(&unUserFile,"w+");
        while(curr){
            temp=curr->next;
            writeUnregUser(curr,unUserFile);
            freeUnregUser(curr);
            free(curr);
            curr=temp;
        }
    }else{
        while(curr){
            temp=curr->next;
            freeUnregUser(curr);
            free(curr);
            curr=temp;
        }
    }
    fclose(unUserFile);
    return found;
}

int unregUserExist(char* username){                         //pomocna, ako negdje zatreba
    UNREGUSER* temp=(UNREGUSER*)calloc(1,sizeof(UNREGUSER));
    FILE* unUserFile;

    openUnregUserData(&unUserFile,"r");
    allocUnregUser(temp);
    while(loadUnregUser(temp,unUserFile)){
        if(!strcmp(username,temp->base.userName)){
            fclose(unUserFile);
            return 1;
        }
    }
    fclose(unUserFile);
    return 0;
}
int approveUser(char* username){
    UNREGUSER* temp=(UNREGUSER*)calloc(1,sizeof(UNREGUSER));
    FILE* unUserFile;
    FILE* userFile;

    openUnregUserData(&unUserFile,"r+");
    allocUnregUser(temp);
    while(loadUnregUser(temp,unUserFile)){
        if(!strcmp(username,temp->base.userName)){
            openUserData(&userFile,"r+");
            appendUser(&(temp->base),userFile);
            fclose(unUserFile);
            fclose(userFile);
            deleteUnregUser(temp->base.userName);
            return 1;
        }
    }
    fclose(unUserFile);
    return 0;
}
void approveAllUsers(){
    FILE* unUserFile;
    FILE* UserFile;
    UNREGUSER* temp=(UNREGUSER*)calloc(1,sizeof(UNREGUSER));

    openUnregUserData(&unUserFile,"r+");
    openUserData(&UserFile,"r+");
    allocUnregUser(temp);
    while(!feof(unUserFile)){
        loadUnregUser(temp,unUserFile);
        appendUser(&temp->base,UserFile);
    }
    fclose(UserFile);
    fclose(unUserFile);
    openUnregUserData(&unUserFile,"w");
    fprintf(unUserFile,"");
    fclose(unUserFile);

}
void freeUser(USER* user) {
	free(user->userID);
	free(user->userName);
	free(user->password);
	free(user->type);
}
void freeUnregUser(UNREGUSER* unUser){
    freeUser(&(unUser->base));
    free(unUser->name);
    free(unUser->surname);
    free(unUser->email);
}
void freeEvent(EVENT* eve) {
	free(eve->eventID);
	free(eve->category);
	free(eve->date);
	free(eve->description);
	free(eve->headline);
	free(eve->user);
}

void getId(int* id,FILE* opFile)
{
	rewind(opFile);
	fscanf(opFile, "%d ", id);
}

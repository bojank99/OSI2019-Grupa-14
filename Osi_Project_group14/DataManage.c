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
		return fscanf(userFile, "%s %s %s %s", user->userID, user->userName, user->password, user->type);
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
	}
	return 1;
}


void writeUser(USER* user,  FILE* userFile) {								//podrazumjevaju da su korisnici/dogadjaji konacno formirani
	fprintf(userFile, "%s %s %s %s \n", user->userID, user->userName, user->password, user->type);
}		
void writeEvent(EVENT* eve,  FILE* eventFile) {
	fprintf(eventFile, "%s %s %s| %s| %s %s %d \n",eve->eventID, eve->user, eve->headline, eve->description, eve->date, eve->category, eve->finished);
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
			loadUser(curr,userFile);
		}
		else {
			curr->next = (USER*)calloc(1, sizeof(USER));
			createUser(curr->next);
			loadUser(curr->next, userFile);
			curr = curr->next;
		}
	}
	
	curr = first;
	if (!first->next || !first) {
		return 0;
	}
	if (found) {
		fclose(userFile);
		openUserData(&userFile, "w+");
		fprintf(userFile, "%d\n", id);
		while (curr) {
			temp = curr->next;
			writeUser(curr, userFile);
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
	}
	fclose(eventFile);
	return found;
}


void freeUser(USER* user) {
	free(user->userID);
	free(user->userName);
	free(user->password);
	free(user->type);
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

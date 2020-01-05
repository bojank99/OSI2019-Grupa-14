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
int openCategoryIndex(FILE** fileI,char* mode){
    return ((*fileI)=fopen("Data/Index_Category.txt",mode));
}
int openDateIndex(FILE** fileI,char* mode){
    return ((*fileI)=fopen("Data/Index_Datum.txt",mode));
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
	fprintf(userFile, "%s %s %s %s\n", user->userID, user->userName, user->password, user->type);
}
void writeEvent(EVENT* eve,  FILE* eventFile) {
	fprintf(eventFile, "%s %s %s| %s| %s %s %d\n",eve->eventID, eve->user, eve->headline, eve->description, eve->date, eve->category, eve->finished);
}

int appendUser(USER* user,FILE* userFile){                  //ova dodaje korisnika na kraj sa racunanjem id-a i ponovnim upisivanjem id-a na pocetak
    int id;													//varijable za id
    char idC[6];

    strcpy(user->type,"user");								//stavlja se user kao type novog korisnika

    if(!userFile){return 0;}
    rewind(userFile);										//uzimanje id-a sa pocetka fajla
    getId(&id,userFile);
    id++;													//povecavanje najveceg id-a za 1
    rewind(userFile);
    itoa(id,idC,10);
    strcpy(user->userID,idC);
    fprintf(userFile,"%05d",id);							// upisuje se novi najveci id na pocetak fajla
    fseek(userFile,0,SEEK_END);								//ide na kraj fajla i tu samo upisuje info o novom korisniku
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
		openUserData(&userFile, "w");
		fprintf(userFile, "%d\n", id);
		while (curr) {
			temp = curr->next;
			writeUser(curr, userFile);
			freeUser(curr);
			free(curr);
			curr = temp;
		}
	}else{
        while(curr){
            temp=curr->next;
            freeUser(curr);
            free(curr);
            curr=temp;
        }
	}
	fclose(userFile);
	return found;
}


int deleteEvent(char* eventId, unsigned long int* spacing)       //spacing je broj mijesta koje zauzimaju svi char-ovi event-a(vazno za pomjeranje pozicija u indexima)
{
	FILE* eventFile;
	int found = 0; int id; int tmp = atoi(eventId);
	unsigned long int current;
	unsigned long int previous=0;                         //pamti se pocetak i kraj jednog zapisa, kad se nadje odredejn zapsi onda se izracuna spacing kao (pokazivac nakon ucitavanja)-(prije ucitavanja)
    EVENT* first = 0;
	EVENT* curr = 0;
	EVENT* temp = 0;

	openEventData(&eventFile,"r+");
	getId(&id, eventFile);

	if (tmp > id || tmp < 0) {											//ako ostanje id
		return 0;
	}

	while (!feof(eventFile)) {
		if (!first) {
			first = (EVENT*)calloc(1, sizeof(EVENT));
			createEvent(first);
			loadEvent(first, eventFile);
			curr = first;
			current=ftell(eventFile);
		}
		if (!strcmp(eventId, curr->eventID)) {
            (*spacing)=(ftell(eventFile)-previous-2);
			loadEvent(curr, eventFile);
			found = 1;
		}
		else {
            previous=current;
			curr->next = (EVENT*)calloc(1, sizeof(EVENT));
			createEvent(curr->next);
			loadEvent(curr->next, eventFile);
			curr = curr->next;
			current=ftell(eventFile);
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

int updateIndex(int id,int spacing,char* file){
    int n=0;
    INDEX* arrDate;
    FILE* indexFile;

    if(!(arrDate=readFromIndex(&n,file))){return 0;}

    indexFile=fopen(file,"w");

    for(int i=0;i<n;i++){
        if(atoi(arrDate[i].eventID)==id){i++;}                    //preskace se zapis koji ima indeks izbrisanog eventa
        else if(atoi(arrDate[i].eventID)>id){               //u Events fajlu samo events koji dolaze poslije izbrisanog u fajlu(oni sa vecim id-em) mijenjaju svoju poziciju u fajlu, oni prije ne
            arrDate[i].position-=spacing;                   //racuna se novi position na osnovu spacing-a
            fprintf(indexFile,"%s %s %d\n",arrDate[i].key,arrDate[i].eventID,arrDate[i].position);
        }else{
            fprintf(indexFile,"%s %s %d\n",arrDate[i].key,arrDate[i].eventID,arrDate[i].position);
        }
    }
    fclose(indexFile);
    return 1;
}

int removeEvent(char* eventId){
    unsigned long int spacing;

    if(deleteEvent(eventId,&spacing)){
        updateIndex(atoi(eventId),spacing+2,"Data/Index_Category.txt");         //+2 na spacing zbog novog reda
        updateIndex(atoi(eventId),spacing+2,"Data/Index_Datum.txt");
    }else{
        return 0;
    }
    return 1;
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


void makeDatum(const char *datum, int arr[3]){
    int i=0, j=0, k=0;
    char temp[5]={0};
    while(datum[i]){
        if(datum[i]!='.')
            temp[j++]=datum[i];
        else{
            temp[j]=0;
            arr[k++]=atoi(temp);
            j=0;
        }
        i++;
    }
}

int compareDatum (const char* datum1, const char* datum2){
    int arr1[3], arr2[3];
    makeDatum(datum1, arr1); makeDatum(datum2, arr2);
    return arr1[2]<arr2[2] ? -1 :
        (arr1[2]>arr2[2] ? 1 :
            (arr1[1]<arr2[1] ? -1 :
                (arr1[1]>arr2[1] ? 1:
                    (arr1[0]<arr2[0] ? -1 :
                        (arr1[0]>arr2[0] ? 1 : 0)))));
}

void printInIndex(INDEX *arr, INDEX *data, int n, const char* string, int (*compare)(const char*, const char*)){
    int i, flag=1;  // flag - promjenljiva koja sluzi za provjeru da li se obradio dogadjaj koji je poslat
    FILE *fp=fopen(string, "w");
    if(fp!=NULL){
        if(n==0){  // ako je datoteka bila prazna
            fprintf(fp, "%s %s %d\n", data->key, data->eventID, data->position);  // upis u datoteku
            flag=0; // oznacimo da je obradjen dati element
        }
        else
            for(i=0; i<n; i++) // ako datoteka nije bila prazna prodjemo kroz sve elemente koji su bili u datoteci
                if(!strcmp(data->eventID, arr[i].eventID)) continue;  // brisanje --> kako je ID jedinstven za sve dogadjaje, preskocicemo upis onog elementa koji ima isti ID
                else if((*compare)(data->key, arr[i].key)<=0 && flag && data->position){  // ako je element koji dodajemo 'manji ili jednak' od elementa koji je bio u datoteci
                    fprintf(fp, "%s %s %d\n", data->key, data->eventID, data->position);
                    fprintf(fp, "%s %s %d\n", arr[i].key, arr[i].eventID, arr[i].position);
                    flag=0;
                }
                else fprintf(fp, "%s %s %d\n", arr[i].key, arr[i].eventID, arr[i].position);  // ako je element koji dodajemo 'veci' od elemenata koji su bili u datoteci, samo prepisemo te elemente
        if(flag && data->position) fprintf(fp, "%s %s %d\n", data->key, data->eventID, data->position);  // ako element koji dodajemo nije upisan u datoteku, upisemo ga
        fclose(fp);
    }
    else printf("ERROR!");
    free(arr);
}

INDEX* readFromIndex(int* n, const char* string){
     INDEX *arr=NULL;
     FILE *fp=fopen(string, "r");
     INDEX temp={0};
     int u=20, pom;
     if(fp!=NULL){
        arr=(INDEX*)calloc(u, sizeof(INDEX));
        while((pom=fscanf(fp, "%s %s %d\n", temp.key, temp.eventID, &temp.position))==3){
            arr[(*n)++]=temp;
            if(*n==u) arr=(INDEX *)realloc(arr, (u*=2)*sizeof(INDEX));
        }
        arr=(INDEX *)realloc(arr, (*n)*sizeof(INDEX));
        fclose(fp);
    }
    else printf("ERROR!");
    return arr;
}

void sort (EVENT *event, int position){
    INDEX *data=(INDEX*)calloc(1, sizeof(INDEX)), *arr=NULL; // alociramo memoriju za novi element indeksne datoteke i definisemo pokazivac na niz elemenata koji se nalaze u indeksnoj datoteci
    int n=0;  // n - broj elemenata indekse datoteke

    strcpy(data->eventID, event->eventID);
    strcpy(data->key, event->date);
    data->position=position;
    //upisemo podatke u data, koji predstavlja element indekse datoteke
    arr=readFromIndex(&n, "Data/Index_Datum.txt");  //iscitavanje elementa iz datoteke
    printInIndex(arr, data, n , "Data/Index_Datum.txt", &compareDatum);  // upis elementa u datoteku

    arr=NULL; n=0;
    strcpy(data->key, event->category);  // promijenimo kljuc za novu indeksnu datoteku
    arr=readFromIndex(&n, "Data/Index_Category.txt");
    printInIndex(arr, data, n, "Data/Index_Category.txt", &strcmp);
    free(data);
}


void addComment(int eventsId, char* comment)
{
    FILE *filComm;
    filComm = fopen("Data/Komentari.txt","a");
    if(filComm!=NULL)
    {
        fprintf(filComm, "0,%d,%s\n",eventsId, comment);
    }
    fclose(filComm);
}



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
                prelast->next=0;
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
	return 1;
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
    while(loadUnregUser(temp,unUserFile)){								//prolazak kroz fajl
        if(!strcmp(username,temp->base.userName)){						//trazi se odgovarajuci korisnik, kad(ako) se nadje appenda se u Users
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
void approveAllUsers(){											//odobrava sve korisnike
    FILE* unUserFile;
    FILE* UserFile;
    UNREGUSER* temp=(UNREGUSER*)calloc(1,sizeof(UNREGUSER));		//ne treba lista, samo jedan u koji se ucitavaju podaci

    openUnregUserData(&unUserFile,"r+");
    openUserData(&UserFile,"r+");
    allocUnregUser(temp);
    while(!feof(unUserFile)){										//prolazi kroz fajl i appenda ih
        loadUnregUser(temp,unUserFile);
        appendUser(&temp->base,UserFile);
    }
    fclose(UserFile);
    fclose(unUserFile);
    openUnregUserData(&unUserFile,"w");								//brise sve iz fajla
    fprintf(unUserFile,"");
    fclose(unUserFile);

}
void freeUnregUser(UNREGUSER* unUser){
    freeUser(&(unUser->base));
    free(unUser->name);
    free(unUser->surname);
    free(unUser->email);
}

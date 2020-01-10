#include "DataManage.h"
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <windows.h>
#pragma warning(disable:4996)												//za VS, da se ignorisu upozorenja za fscanf i slicne f-je

void createUser(USER* temp)
{
    temp->userID = (char*)calloc(6, sizeof(char));
    temp->userName = (char*)calloc(20, sizeof(char));
    temp->password = (char*)calloc(20, sizeof(char));
    temp->type = (char*)calloc(11, sizeof(char));
    temp->next = 0;
}
void createEvent(EVENT* eve)
{
    eve->eventID = (char*)calloc(7, sizeof(char));
    eve->headline = (char*)calloc(35, sizeof(char));
    eve->description = (char*)calloc(250, sizeof(char));
    eve->date = (char*)calloc(11, sizeof(char));
    eve->user = (char*)calloc(20, sizeof(char));
    eve->category = (char*)calloc(20, sizeof(char));
    eve->time=(char*)calloc(6,sizeof(char));
    eve->location=(char*)calloc(30,sizeof(char));
}
void createComment(COMMENT* comm){
    comm->comEventID=(char*)calloc(6,sizeof(char));
    comm->comUsername=(char*)calloc(21,sizeof(char));
    comm->commentText=(char*)calloc(231,sizeof(char));
}

int openUserData(FILE** users, char* mode)
{
    return ((*users) = fopen("Data/Users.txt", mode));
}
int openEventData(FILE** events, char* mode)
{
    return ((*events) = fopen("Data/Events.txt",mode));
}
int openCategoryIndex(FILE** fileI,char* mode)
{
    return ((*fileI)=fopen("Data/Index_Category.txt",mode));
}
int openDateIndex(FILE** fileI,char* mode)
{
    return ((*fileI)=fopen("Data/Index_Datum.txt",mode));
}
int openCommentData(FILE** comments,char* mode){
    return((*comments)=fopen("Data/Komentari.txt",mode));
}
int openUnApprovedComments(FILE** comments,char* mode){
    return((*comments)=fopen("Data/Komentari_za_Odobravanje.txt",mode));
}


int loadUser(USER* user,  FILE* userFile)
{
    if (!feof(userFile))
    {
        return fscanf(userFile, "%s %s %s %s", user->userID, user->userName, user->password, user->type);
    }
    return 0;
}
int loadEvent(EVENT* eve, FILE* eventFile)
{
     int i = 0;
    char tmp;
    if (!feof(eventFile))
    {
        fscanf(eventFile, "%s %s ", eve->eventID, eve->user);
        while ((tmp = fgetc(eventFile)) != '|' && tmp != EOF)
        {
            if (tmp != '\n')
            {
                eve->headline[i] = tmp;
                i++;
            }
        }
        fgetc(eventFile);
        i = 0;
        while ((tmp = fgetc(eventFile)) != '|')
        {
                eve->description[i] = tmp;
                i++;
        }
        fscanf(eventFile, "%s %s %d %s ", eve->date, eve->category, &eve->finished,eve->time);
        i=0;
        while((tmp=fgetc(eventFile))!='\n' && tmp!=EOF){
            eve->location[i]=tmp;
            i++;
        }

    }
    else return 0;
    return 1;
}


void writeUser(USER* user,  FILE* userFile)  								//podrazumjevaju da su korisnici/dogadjaji konacno formirani
{
    if(!user->userName[0]){return;}
    fprintf(userFile, "%05d %s %s %s\n", atoi(user->userID), user->userName, user->password, user->type);
}
void writeEvent(EVENT* eve,  FILE* eventFile)
{
    if(!eve->user[0]){return;}
fprintf(eventFile, "%s %s %s| %s| %s %s %d %s %s\n",eve->eventID, eve->user, eve->headline,
            eve->description, eve->date, eve->category, eve->finished,eve->time,eve->location);}

int appendUser(USER* user,FILE* userFile)                   //ova dodaje korisnika na kraj sa racunanjem id-a i ponovnim upisivanjem id-a na pocetak
{
    int id;													//varijable za id
    char idC[6];

    strcpy(user->type,"user");								//stavlja se user kao type novog korisnika

    if(!userFile)
    {
        return 0;
    }
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

int deleteUser(char* userid)  												//NOTE: predpostavlja se da je fajl uspjesno otvoren
{
    FILE* userFile;
    USER* temp;
    USER* first = 0;														//head cvor liste
    USER* curr = 0;
    int found = 0;
    int id;
    int tmp = atoi(userid);							//kao bool, da li je nadjen takav korisnik

    if (!openUserData(&userFile, "r+"))
    {
        return 0;
    }
    getId(&id, userFile);

    if (tmp > id || tmp < 0)  												//ako ostanje nacin pravljenja id-a
    {
        return 0;
    }

    while (!feof(userFile))  												//poslije ce biti napisana posebna funkcija za formiranje/upis liste za event i user
    {
        if (first == 0)
        {
            first = (USER*)calloc(1, sizeof(USER));
            createUser(first);
            loadUser(first, userFile);
            curr = first;
        }
        if (!strcmp(userid, curr->userID))
        {
            found = 1;
            loadUser(curr,userFile);
        }
        else
        {
            curr->next = (USER*)calloc(1, sizeof(USER));
            createUser(curr->next);
            loadUser(curr->next, userFile);
            curr = curr->next;
        }
    }

    curr = first;
    if (!first->next || !first)
    {
        return 0;
    }
    if (found)
    {
        fclose(userFile);
        openUserData(&userFile, "w");
        fprintf(userFile, "%d\n", id);
        while (curr)
        {
            temp = curr->next;
            writeUser(curr, userFile);
            freeUser(curr);
            free(curr);
            curr = temp;
        }
    }
    else
    {
        while(curr)
        {
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
    int found = 0;
    int id;
    int tmp = atoi(eventId);
    unsigned long int current;
    unsigned long int previous=0;                         //pamti se pocetak i kraj jednog zapisa, kad se nadje odredejn zapsi onda se izracuna spacing kao (pokazivac nakon ucitavanja)-(prije ucitavanja)
    EVENT* first = 0;
    EVENT* curr = 0;
    EVENT* temp = 0;

    openEventData(&eventFile,"r+");
    getId(&id, eventFile);

    if (tmp > id || tmp < 0)  											//ako ostanje id
    {
        return 0;
    }

    while (!feof(eventFile))
    {
        if (!first)
        {
            first = (EVENT*)calloc(1, sizeof(EVENT));
            createEvent(first);
            loadEvent(first, eventFile);
            curr = first;
            current=ftell(eventFile);
        }
        if (!strcmp(eventId, curr->eventID))
        {
            (*spacing)=(ftell(eventFile)-previous-2);
            loadEvent(curr, eventFile);
            found = 1;
        }
        else
        {
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
    if (!first->next || !first)
    {
        return 0;
    }
    if (found)
    {
        openEventData(&eventFile, "w+");
        fprintf(eventFile, "%d\n", id);
        while (curr)
        {
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

int updateIndex(int id,int spacing,char* file)
{
    int n=0;
    INDEX* arrDate;
    FILE* indexFile;

    if(!(arrDate=readFromIndex(&n,file)))
    {
        return 0;
    }

    indexFile=fopen(file,"w");

    for(int i=0; i<n; i++)
    {
        if(atoi(arrDate[i].eventID)==id)
        {
            i++;   //preskace se zapis koji ima indeks izbrisanog eventa
        }
        else if(atoi(arrDate[i].eventID)>id)                //u Events fajlu samo events koji dolaze poslije izbrisanog u fajlu(oni sa vecim id-em) mijenjaju svoju poziciju u fajlu, oni prije ne
        {
            arrDate[i].position-=spacing;                   //racuna se novi position na osnovu spacing-a
            fprintf(indexFile,"%s %s %d\n",arrDate[i].key,arrDate[i].eventID,arrDate[i].position);
        }
        else
        {
            fprintf(indexFile,"%s %s %d\n",arrDate[i].key,arrDate[i].eventID,arrDate[i].position);
        }
    }
    fclose(indexFile);
    return 1;
}

int removeEvent(char* eventId)
{
    unsigned long int spacing;

    if(deleteEvent(eventId,&spacing))
    {
        updateIndex(atoi(eventId),367+2,"Data/Index_Category.txt");         //+2 na spacing zbog novog reda
        updateIndex(atoi(eventId),367+2,"Data/Index_Datum.txt");
    }
    else
    {
        return 0;
    }
    return 1;
}


void freeUser(USER* user)
{
    free(user->userID);
    free(user->userName);
    free(user->password);
    free(user->type);
}
void freeEvent(EVENT* eve)
{
    free(eve->eventID);
    free(eve->category);
    free(eve->date);
    free(eve->description);
    free(eve->headline);
    free(eve->user);
    free(eve->location);
    free(eve->time);
}

void freeComment(COMMENT* comm){
    free(comm->comEventID);
    free(comm->comUsername);
    free(comm->commentText);
}

void getId(int* id,FILE* opFile)
{
    rewind(opFile);
    fscanf(opFile, "%d ", id);
}


void makeDatum(const char *datum, int arr[3])
{
    int i=0, j=0, k=0;
    char temp[5]= {0};
    while(datum[i])
    {
        if(datum[i]!='.')
            temp[j++]=datum[i];
        else
        {
            temp[j]=0;
            arr[k++]=atoi(temp);
            j=0;
        }
        i++;
    }
}

int compareDatum (const char* datum1, const char* datum2)
{
    int arr1[3], arr2[3];
    makeDatum(datum1, arr1);
    makeDatum(datum2, arr2);
    return arr1[2]<arr2[2] ? -1 :
           (arr1[2]>arr2[2] ? 1 :
            (arr1[1]<arr2[1] ? -1 :
             (arr1[1]>arr2[1] ? 1:
              (arr1[0]<arr2[0] ? -1 :
               (arr1[0]>arr2[0] ? 1 : 0)))));
}

void printInIndex(INDEX *arr, INDEX *data, int n, const char* string, int (*compare)(const char*, const char*))
{
    int i, flag=1;  // flag - promjenljiva koja sluzi za provjeru da li se obradio dogadjaj koji je poslat
    FILE *fp=fopen(string, "w");
    if(fp!=NULL)
    {
        if(n==0)   // ako je datoteka bila prazna
        {
            fprintf(fp, "%s %s %d\n", data->key, data->eventID, data->position);  // upis u datoteku
            flag=0; // oznacimo da je obradjen dati element
        }
        else
            for(i=0; i<n; i++) // ako datoteka nije bila prazna prodjemo kroz sve elemente koji su bili u datoteci
                if(!strcmp(data->eventID, arr[i].eventID))
                    continue;  // brisanje --> kako je ID jedinstven za sve dogadjaje, preskocicemo upis onog elementa koji ima isti ID
                else if((*compare)(data->key, arr[i].key)<=0 && flag && data->position)   // ako je element koji dodajemo 'manji ili jednak' od elementa koji je bio u datoteci
                {
                    fprintf(fp, "%s %s %d\n", data->key, data->eventID, data->position);
                    fprintf(fp, "%s %s %d\n", arr[i].key, arr[i].eventID, arr[i].position);
                    flag=0;
                }
                else
                    fprintf(fp, "%s %s %d\n", arr[i].key, arr[i].eventID, arr[i].position);  // ako je element koji dodajemo 'veci' od elemenata koji su bili u datoteci, samo prepisemo te elemente
        if(flag && data->position)
            fprintf(fp, "%s %s %d\n", data->key, data->eventID, data->position);  // ako element koji dodajemo nije upisan u datoteku, upisemo ga
        fclose(fp);
    }
    else
        printf("ERROR!");
    free(arr);
}

INDEX* readFromIndex(int* n, const char* string)
{
    INDEX *arr=NULL;
    FILE *fp=fopen(string, "r");
    INDEX temp= {0};
    int u=20, pom;
    if(fp!=NULL)
    {
        arr=(INDEX*)calloc(u, sizeof(INDEX));
        while((pom=fscanf(fp, "%s %s %d\n", temp.key, temp.eventID, &temp.position))==3)
        {
            arr[(*n)++]=temp;
            if(*n==u)
                arr=(INDEX *)realloc(arr, (u*=2)*sizeof(INDEX));
        }
        arr=(INDEX *)realloc(arr, (*n)*sizeof(INDEX));
        fclose(fp);
    }
    else
        printf("ERROR!");
    return arr;
}

void sort (EVENT *event, int position)
{
    INDEX *data=(INDEX*)calloc(1, sizeof(INDEX)), *arr=NULL; // alociramo memoriju za novi element indeksne datoteke i definisemo pokazivac na niz elemenata koji se nalaze u indeksnoj datoteci
    int n=0;  // n - broj elemenata indekse datoteke

    strcpy(data->eventID, event->eventID);
    strcpy(data->key, event->date);
    data->position=position;
    //upisemo podatke u data, koji predstavlja element indekse datoteke
    arr=readFromIndex(&n, "Data/Index_Datum.txt");  //iscitavanje elementa iz datoteke
    printInIndex(arr, data, n, "Data/Index_Datum.txt", &compareDatum);   // upis elementa u datoteku

    arr=NULL;
    n=0;
    strcpy(data->key, event->category);  // promijenimo kljuc za novu indeksnu datoteku
    arr=readFromIndex(&n, "Data/Index_Category.txt");
    printInIndex(arr, data, n, "Data/Index_Category.txt", &strcmp);
    free(data);
}


void addComment(int eventsId,char* userName, char* comment)
{
    FILE *filComm;
    filComm = fopen("Data/Komentari_za_Odobravanje.txt","a");
    if(filComm!=NULL)
    {
        fprintf(filComm, "%d,%s,%s\n",eventsId,userName,comment);
    }
    fclose(filComm);
}



int openUnregUserData(FILE** unregFile,char* mode)
{
    return ((*unregFile)=fopen("Data/Korisnicki_zahtjevi.txt",mode));
}

void allocUnregUser(UNREGUSER* unUser)
{
    createUser(&(unUser->base));
    unUser->name=(char*)calloc(20,sizeof(char));
    unUser->surname=(char*)calloc(25,sizeof(char));
    unUser->email=(char*)calloc(22,sizeof(char));
    unUser->next=0;
}

int loadUnregUser(UNREGUSER* unrUser,FILE* unregUserFile)
{
    if(!feof(unregUserFile))
    {
        memset(unrUser->base.userName,0,20);
        memset(unrUser->base.password,0,20);
        memset(unrUser->email,0,20);
        memset(unrUser->name,0,20);
        memset(unrUser->surname,0,25);
        fscanf(unregUserFile,"%s %s %s %s %s\n",unrUser->base.userName,unrUser->base.password,
                      unrUser->email,unrUser->name,unrUser->surname);
        return 1;
    }
    return 0;
}

void writeUnregUser(UNREGUSER* unrUser,FILE* unUserFile)
{
    fprintf(unUserFile,"%s %s %s %s %s\n",unrUser->base.userName,unrUser->base.password,
            unrUser->email,unrUser->name,unrUser->surname);
}

int deleteUnregUser(char* usrName){
    int found=0;
    UNREGUSER unUser;
    FILE* unregData;
    FILE* newData=fopen("Data/tempData.txt","w");

    openUnregUserData(&unregData,"r");
    allocUnregUser(&unUser);

    while(loadUnregUser(&unUser,unregData)){
        if(!strcmp(unUser.base.userName,usrName)){
            found=1;
        }
        else{
            writeUnregUser(&unUser,newData);
        }
    }
    fclose(unregData);
    fclose(newData);
    remove("Data/Korisnicki_zahtjevi.txt");
    rename("Data/tempData.txt","Data/Korisnicki_zahtjevi.txt");
    return found;
}


int unregUserExist(char* username)                          //pomocna, ako negdje zatreba
{
    UNREGUSER* temp=(UNREGUSER*)calloc(1,sizeof(UNREGUSER));
    FILE* unUserFile;

    openUnregUserData(&unUserFile,"r");
    allocUnregUser(temp);
    while(loadUnregUser(temp,unUserFile))
    {
        if(!strcmp(username,temp->base.userName))
        {
            fclose(unUserFile);
            return 1;
        }
    }
    fclose(unUserFile);
    return 0;
}
int approveUser(char* username)
{
    UNREGUSER* temp=(UNREGUSER*)calloc(1,sizeof(UNREGUSER));
    FILE* unUserFile;
    FILE* userFile;

    openUnregUserData(&unUserFile,"r+");
    allocUnregUser(temp);
    while(loadUnregUser(temp,unUserFile)) 								//prolazak kroz fajl
    {
        if(!strcmp(username,temp->base.userName)) 						//trazi se odgovarajuci korisnik, kad(ako) se nadje appenda se u Users
        {
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
void approveAllUsers() 											//odobrava sve korisnike
{
    FILE* unUserFile;
    FILE* UserFile;
    UNREGUSER* temp=(UNREGUSER*)calloc(1,sizeof(UNREGUSER));		//ne treba lista, samo jedan u koji se ucitavaju podaci

    openUnregUserData(&unUserFile,"r+");
    openUserData(&UserFile,"r+");
    allocUnregUser(temp);
    while(!feof(unUserFile)) 										//prolazi kroz fajl i appenda ih
    {
        loadUnregUser(temp,unUserFile);
        appendUser(&temp->base,UserFile);
    }
    fclose(UserFile);
    fclose(unUserFile);
    openUnregUserData(&unUserFile,"w");								//brise sve iz fajla
    fprintf(unUserFile,"");
    fclose(unUserFile);

}
void freeUnregUser(UNREGUSER* unUser)
{
    freeUser(&(unUser->base));
    free(unUser->name);
    free(unUser->surname);
    free(unUser->email);
}

int deleteCategory(char* categoryName)
{
    if(strcmp("Izlozbe",categoryName) &&
            strcmp("Koncerti",categoryName) &&
            strcmp("Promocije",categoryName))
    {
        char *readCategory = (char*)calloc(10,sizeof(char));
        FILE *category,*newFile;
        category=fopen("Data/Kategorije.txt","r");
        newFile=fopen("Data/Kopija.txt","w");
        if(category!=NULL && newFile!=NULL)
        {
            for(int i = 0; i<3; i++)
            {
                fscanf(category,"%s",readCategory);
                fprintf(newFile,"%s\n",readCategory);
            }
            while((fscanf(category,"%s",readCategory))!=EOF)
                if(strcmp(readCategory,categoryName))
                    fprintf(newFile,"%s\n",readCategory);
        }
        fclose(category);
        fclose(newFile);
        remove("Data/Kategorije.txt");
        rename("Data/Kopije.txt","Data/Kategorije.txt");
    }
    else
        return 0;
    return 1;
}

int loadComment(COMMENT* comm,FILE* commFile){
    if(!feof(commFile)){
        return (fscanf(commFile,"%s,%[^,],%[^\n]",comm->comEventID,comm->comUsername,comm->commentText));
    }
    return 0;
}

int insertComment(COMMENT* comm){                                       //upisuje komentar u Komentari.txt da su svi grupisani
    int found=0;
    COMMENT temp;
    createComment(&temp);
    FILE* newComments=fopen("Data/newComments.txt","w");                     //temp fajl, koji ce sadrzati novi komentar i sve prethodne
    FILE* comments;

    if(!openCommentData(&comments,"r+"))
    {return 0;}
    while(loadComment(&temp,comments))
        {
        if(!strcmp(temp.comEventID,comm->comEventID) && !found){        //kada se naidje na komentar sa istim event id-om(i nije vec upisan),
            found=1;                                                        //prvo se upisuje novi pa onda svi stari
            writeComment(comm,newComments);
            writeComment(&temp,newComments);
        }else{
            writeComment(&temp,newComments);
        }
    }
    if(!found){                                                         //ako uopste nema komentara sa istim eventId-em onda se samo dodaje na kraj
        fseek(newComments,0,SEEK_END);
        writeComment(comm,newComments);
    }
    fclose(comments);
    fclose(newComments);
    if(!remove("Data/Komentari.txt") && !rename("Data/newComments.txt","Data/Komentari.txt")){         //brise se stari a preimenuje se novi fajl
        freeComment(comm);
        return 1;
    }
    freeComment(comm);
    return 0;
}


int deleteUnApprovedComment(char* eventID, char* username){                         //slicna prica kao i u insertu, samo se u novi fajl ne upisuje komentar sa istim
    int found=0;                                                                                //eventid-em i username-om
    FILE* newComments=fopen("Data/tempComments.txt","w");
    FILE* comments;
    COMMENT comm;
    createComment(&comm);

    openUnApprovedComments(&comments,"r+");
    while(loadComment(&comm,comments)){
        if(!strcmp(comm.comEventID,eventID) && !strcmp(username,comm.comUsername)){
            found=1;
        }else{
            writeComment(&comm,newComments);
        }
    }
    fclose(newComments);
    fclose(comments);
    if(found){
        remove("Data/Komentari_za_Odobravanje.txt");
        rename("Data/tempComments.txt","Data/Komentari1_za_Odobravanje.txt");
        freeComment(&comm);
        return 1;
    }else if(!found){
        remove("Data/tempComments.txt");
        freeComment(&comm);
        return 0;
    }
}

int approveComment(char* eventID, char* username){                                      //prvo nalazi odgovarajuci komentar, pa onda poziva gore navedene funkcije
    int found=0;
    FILE* unComments;
    COMMENT comm;
    createComment(&comm);

    openUnApprovedComments(&unComments,"r+");
    while(!found && loadComment(&comm,unComments)){
        if(!strcmp(comm.comEventID,eventID) && !strcmp(username,comm.comUsername)){
            found=1;
        }
    }
    fclose(unComments);
    if(found && insertComment(&comm) && deleteUnApprovedComment(eventID,username)){
        freeComment(&comm);
        return 1;
    }
    return 0;
}

int approveAllComments(){                                  //prolazi kroz citav fajl neodobrenih, i za svaki ucitani poziva insert
    COMMENT comm;
    FILE* unComms;
    createComment(&comm);

    if(!openUnApprovedComments(&unComms,"r+")){return 0;}
    while(loadComment(&comm,unComms)){
        if(!insertComment(&comm)){
            freeComment(&comm);
            return 0;
        }
    }
    fclose(unComms);
    openUnApprovedComments(&unComms,"w");                   //ovjde samo pravi prazan fajl
    fprintf(unComms,"");
    fclose(unComms);
    freeComment(&comm);
    return 1;
}

void writeComment(COMMENT* comm,FILE* commFile){                            //postojao je problem, ucitavalo se komentar bez teksta pa sam rijesio ovako                                              //pa da ispise bez novog reda na pocetku, inaceispisuje prvo novi red
        if(!comm->commentText[0]){return;}
        fprintf(commFile,"%s %s %s\n",comm->comEventID,comm->comUsername,comm->commentText);
}

void deleteComment(int id, char* userName, char* comment)
{
    int readId;
    char *readUserName = (char*)calloc(21,sizeof(char));
    char *readComment = (char*)calloc(150,sizeof(char));

    FILE *commentFile,*newFile;
    commentFile=fopen("Data/Komentari.txt","r");
    newFile=fopen("Data/Kopija.txt","w");
    if(commentFile!=NULL && newFile!=NULL)
    {
        while((fscanf(commentFile,"%d,%[^,],%[^\n]",&readId,readUserName,readComment))!=EOF)
            if(!(id == readId && !strcmp(userName,readUserName) && !strcmp(comment,readComment)))
                fprintf(newFile,"%d,%s,%s\n",readId,readUserName,readComment);
    }
    fclose(commentFile);
    fclose(newFile);
    remove("Data/Komentari.txt");
    rename("Data/Kopija.txt","Data/Komentari.txt");
}


char* inputString(){
    int i=0, u=20;
    char c, *pom=(char*)calloc(u, sizeof(char));
    do{
        scanf("%c", &c);
        pom[i++]=c;
        if(i==u) pom=(char*)realloc(pom, (u*=2)*sizeof(char));
    }while(c!='\n');
    pom=(char*)realloc(pom, i*sizeof(char));
    pom[i-1]=0;
    return pom;
}

int checkForAlpha(char* str){
    int i=0;
    while(str[i])
        if(isalpha(str[i++])) return 1;
    return 0;
}

int chooseOption2(int begin, int end){
    char *temp;
    int n;
    temp=inputString();  //unese se opcija kao string
    n=atoi(temp);  // prebaci se u cijeli broj
    if(!checkForAlpha(temp) && n>=begin && n<=end) return n;  // ako nema slova unutar unosa i ako je unutar granica vrati se taj unos
    else printf("Pogresan unos!\n");
    do{  // radimo isto dok se ne unese dobra vrijednost
        printf("Unesite ponovo: ");
        free(temp);
        temp=inputString();
        n=atoi(temp);
        if(!checkForAlpha(temp) && n>=begin && n<=end){
            free(temp);
            return n;
        }
        else printf("Pogresan unos!\n");
    }while(1);
}

void addCategory(){
    FILE *fp=fopen("Data/Kategorije.txt", "r+");
    char temp[15]={0};
    int n;
    if(fp!=NULL){
        printf("Trenutne kategorije: \n");  // ispisemo trenutne kategorije
        while(fscanf(fp, "%s\n", temp)==1)  // iscitavamo jednu po jednu i ispisujemo ih
            printf("%s\n", temp);
        printf("Unesite kategoriju koju zelite dodati: ");
        scanf("%s", temp);
        getchar();
        printf("Da li zelite dodati kategoriju: '%s'?\n1. Da\n2. Ne\nUnesite opciju: ", temp);
        n=chooseOption2(1, 2);  // izbor opcije
        if(n==1) printf("Uspjesno dodata kategorija.\n"), fprintf(fp, "%s\n", temp);  // upis na kraj datoteke
        else printf("Kategorija nije dodata.\n");
        fclose(fp);
    }
    else printf("ERROR!");
    Sleep(5*1000);
}

char** readCategories(int *n){
    FILE *fp=fopen("Data/Kategorije.txt", "r");  // otvaranje datoteke
    int u=10;  // u --> pomocna promjenljiva koja sluzi za realokaciju memorije
    char** cat=(char**)calloc(u, sizeof(char*)), temp[30]={0};  // cat --> niz stringova; temp --> pomocna promjenljiva
    if(fp!=NULL){
        while(fscanf(fp, "%s\n", temp)==1){  // iscitavanje kategorija u pomocnu promjenljivu
            cat[*n]=(char*)calloc(strlen(temp)+1, sizeof(char));
            strcpy(cat[(*n)++], temp);  // kopiranje kategorija u niz
            if(*n==u) cat=(char**)realloc(cat, (u*=2)*sizeof(char*));  // ako smo dosli do granice, realociramo memoriju
        }
        fclose(fp);
    }
    else printf("ERROR!");
    cat=(char**)realloc(cat, *n*sizeof(char*));  // realociramo memoriju
    return cat;
}

void inputCategory(EVENT *data){
    int n=0, i;
    char **s;
    printf("Postojece kategorije:\n");
    s=readCategories(&n); // iscitamo sve kategorije i ispisemo ih
    for(i=0; i<n; i++)
        printf("%d. %s\n", i+1, s[i]);
    printf("%d. Dodaj novu kategoriju\n", n+1);  // postoji opcija dodavanja nove kategorije ako zeljena vec ne postoji
    printf("Izaberite opciju: ");
    i=chooseOption2(1, n+1);  // izbor opcije
    if(i<=n) strcpy(data->category, s[i-1]);  // ako se ne dodaje nova, kopira se kategorija
    else{
        for(i=0; i<n; i++)  // oslobodi se memorija
            free(s[i]);
        free(s);
        newPage("BanjaLuka");  //ocisti se ekran
        addCategory();  //dodavanje nove kategorije
        newPage("BanjaLuka");
        printf("Postojece kategorije:\n");  // ponovo se ispisu kategorije i izabere jedna
        n=0;
        s=readCategories(&n);
        for(i=0; i<n; i++)
            printf("%d. %s\n", i+1, s[i]);
        printf("Izaberite opciju: ");
        i=chooseOption2(1, n);
        strcpy(data->category, s[i-1]);
    }
    for(i=0; i<n; i++)  //oslobadjanje memorije
        free(s[i]);
    free(s);
}

char* currentDate(){
    time_t currTime;
    struct tm *info;
    char *tday=(char*)calloc(12, sizeof(char));
    time(&currTime);
    info=localtime(&currTime);
    strftime(tday, 12, "%d.%m.%Y.", info);
    return tday;
}

int isFinished(char* date){
    int n;
    char *tday=currentDate();
    n=compareDatum(tday, date);
    free(tday);
    return (n<=0) ? 0 : 1;
}

void inputEventData(EVENT *data){
    char* temp;
    printf("NAPOMENA: U zagradi pise maksimalan broj karaktera za odredjenu osobinu, ako se unese vise, visak karaktera ce se ignorisati.\n");
    /* Unese se podataka, realocira memorija na granicu,
    kopira u promjenljivu date i oslobodi memorija,
    tako za naziv, opis i datum*/
    printf("Unesite naziv dogadjaja (34): ");
    temp=inputString();
    temp=(char*)realloc(temp, 35*sizeof(char));
    temp[34]=0;
    strcpy(data->headline, temp);
    free(temp);

    printf("Unesite opis dogadjaja (249): ");
    temp=inputString();
    temp=(char*)realloc(temp, 250*sizeof(char));
    temp[249]=0;
    strcpy(data->description, temp);
    free(temp);

    printf("Unesite datum (dd.mm.gggg.): ");
    temp=inputString();
    temp=(char*)realloc(temp, 12*sizeof(char));
    temp[11]=0;
    strcpy(data->date, temp);
    free(temp);

    printf("Unesite vrijeme (hh:mm): ");
    temp=inputString();
    temp=(char*)realloc(temp, 6*sizeof(char));
    temp[5]=0;
    strcpy(data->time, temp);
    free(temp);

    printf("Unesite lokaciju(29): ");
    temp=inputString();
    temp=(char*)realloc(temp, 30*sizeof(char));
    temp[29]=0;
    strcpy(data->location, temp);
    free(temp);

    inputCategory(data);  // pozivanje funkcije za unos kategorije
    data->finished=(!isFinished(data->date)) ? 0 : 1;
}

char* generateIndex(int index){
    char* newIndex=(char*)calloc(7, sizeof(char)), pom[7]={0}; // alocira se memorija za novi index i napravi pomocna promjenljiva za posljednji indeks u datoteci
    int i, length=0;
    itoa(index, pom, 10);  // pretvorimo indeks u string
    length=strlen(pom);
    for(i=0; i<6-length; i++)  // ako ima manje od 6 cifara dodamo nule na pocetak
        strcat(newIndex, "0");
    strcat(newIndex, pom);
    return newIndex;
}

int isEventValid(char* date){
    char* fday=currentDate(), arr[5]={0};  //dobijemo danasnji datum, arr --> pomocna promjenljiva
    int i;
    for(i=0; i<4; i++)
        arr[i]=fday[i+6];  // kopiramo godinu
    itoa(atoi(arr)+5, arr, 10);  // povecamo za 5
    for(i=0; i<4; i++)  // vratimo u string
        fday[i+6]=arr[i];
    return (compareDatum(fday, date)<=0) ? 0 : 1;  // uporedimo datume
}

void everyIndexPosition(INDEX *arr, int n, const char* string){
    FILE *fp=fopen(string, "w");
    int i;
    if(fp!=NULL){
        for(i=0; i<n; i++)
            fprintf(fp, "%s %s %d\n", arr[i].key, arr[i].eventID, arr[i].position+1); // povecamo poziciju za 1
        fclose(fp);
    }
    else printf("ERROR!\n");
}

void updateIndex3(int index){
    INDEX *arr;
    int n=0;
    char *older=(char*)calloc(4, sizeof(char)), *newer=(char*)calloc(4, sizeof(char));  // older --> stari indeks kao string, newer --> novi indeks kao string
    itoa(index, older, 10); itoa(index+1, newer, 10);
    if(strlen(older)==strlen(newer)){  //ako su iste duzine vratimo se u funkciju
        free(older); free(newer);
    }
    else{  // ako nisu iste duzine azuriramo datoteke
        arr=readFromIndex(&n, "Data/Index_Category.txt");
        everyIndexPosition(arr, n, "Data/Index_Category.txt");
        free(arr); n=0;
        arr=readFromIndex(&n, "Data/Index_Datum.txt");
        everyIndexPosition(arr, n, "Data/Index_Datum.txt");
        free(arr); free(older); free(newer);
    }
}

void addEvent(char* cityName, char* userName){
    FILE *fp;
    openEventData(&fp, "r+");
    int index;
    char *temp;
    EVENT data;
    createEvent(&data);
    newPage(cityName);
    strcpy(data.user, userName);
    inputEventData(&data);  // unesemo podatke o dogadjaju
    if(!isEventValid(data.date)) {  // ako nije moguce dodati dogadjaj ispisemo poruku i izadjemo
        printf("Dogadjaj se desava za vise od 5 godina i nije ga moguce dodati.\n");
        freeEvent(&data);
        Sleep(5*1000);
        fclose(fp);
        return;
    }
    else{
        if(fp!=NULL){
            newPage(cityName);  // ocistimo ekran
            printf("Naziv dogadjaja: %s\n", data.headline);
            printf("Opis dogadjaja: %s\n", data.description);
            printf("Datum odrzavanja: %s\n", data.date);
            printf("Vrijeme odrzavanja: %s\n", data.time);
            printf("Lokacija odrzavanja: %s\n", data.location);
            printf("Vrsta dogadjaja: %s\n", data.category);
            printf("Da li zelite sacuvati dogadjaj sa prethodnim podacima?\n1. Da\n2. Ne\nUnesite izbor: ");  // potvrda sacuvavanja
            index=chooseOption2(1, 2);  // indeks se koristi kao opcija, posto nam dalje ne treba
            if(index==1){  // ako se zeli sacuvati predjemo na kraj datoteke, upisemo element, zatim dodamo element u indeksne datoteke
                if(fscanf(fp, "%d\n", &index)==1){  // ako datoteka nije prazna, uzmemo posljednji indeks, napravimo novi i dodijelimo ga dogadjaju
                    updateIndex3(index); // ako je doslo do povecanja broja cifara indeksa koji ce se upisati, moramo azurirati indekse datoteke
                    index++;
                    temp=generateIndex(index);
                    strcpy(data.eventID, temp);
                    rewind(fp);
                    fprintf(fp, "%d", index);
                }
                else{  // ako je datoteka bila prazna, tj. ako nije bilo dogadjaja indeksi krecu od 1
                    index=1;
                    temp=generateIndex(index);
                    strcpy(data.eventID, temp);
                    rewind(fp);
                    fprintf(fp, "%d     \n", index);
                }
                fseek(fp, 0, SEEK_END);
                sort(&data, ftell(fp));
                index=fprintf(fp, "%s %s %s| %s| %s %s %d %s %s", data.eventID, data.user, data.headline, data.description, data.date,
                              data.category, data.finished, data.time, data.location);
                free(temp);
                temp=(char*)calloc(368-index,sizeof(char));  // ukupan broj + 1 za /0 na kraju
                for(int i=0; i<368-index; i++)
                    temp[i]=' ';
                fprintf(fp, "%s\n", temp);
                printf("Uspjesno dodat dogadjaj.");
            }
            else printf("Dogadjaj nije dodat.");
            free(temp);
            fclose(fp);
        }
        else printf("ERROR!\n");
    }
    freeEvent(&data);
    Sleep(5*1000); // pauza 5 sec
    newPage("Banjaluka");
}

int findPosition(char *eventID){
    FILE *fp=fopen("Data/Index_Datum.txt", "r");  //otvorimo indeksnu datoteku
    int position=0;
    INDEX temp;
    if(fp!=NULL){
        while(fscanf(fp, "%s %s %d\n", temp.key, temp.eventID, &temp.position)==3)  // prolazimo kroz sve elemente indeksne datoteke
            if(!strcmp(eventID, temp.eventID)){  // kada naidjemo na dogadjaj koji nam treba zapamtimo poziciju
                position=temp.position;
                break;
            }
        fclose(fp);
    }
    else printf("ERROR!\n");
    return position;
}

void editEvent (EVENT *data){
    newPage("Banjaluka");
    FILE *fp;
    INDEX *arr, data1;  // arr --> niz indeksa, data1 ---> pomocna promjenljiva
    EVENT tmp;  // pomocna promjenljiva
    int n, flag=0, position, neww, old, i;
    /* flag --> pomocna promjenljiva koja oznacava da li se mijenjao kljuc indekse datoteke, position --> pozicija u glavnoj datoteci;
    neww --> pozicija nakon dodavanja u glavnu datoteku, old --> pozicija prije dodavanja u glavnu datoteku*/
    char* temp, *novi;  // pomocne promjenljive
    createEvent(&tmp);  //alociramo memoriju
    printf("NAPOEMNA: U zagradi pise maksimalan broj karaktera za odredjenu osobinu, ako se unese vise, visak karaktera ce se ignorisati.\n");
    do{
        printf("Podaci koji se mogu mijenjati:\n");  // ispisuemo podatke koji se mogu mijenjati
        printf("1. Naziv dogadjaja: %s\n", data->headline);
        printf("2. Opis dogadjaja: %s\n", data->description);
        printf("3. Datum: %s\n", data->date);
        printf("4. Vrijeme: %s\n", data->time);
        printf("5. Lokacija: %s\n", data->location);
        printf("Izaberite sta zelite mijenjati: ");
        n=chooseOption2(1,5); //izaberemo sta zelimo da se promijeni, zatim to promijenimo
        if(n==1){
            printf("Unesite naziv dogadjaja (34): ");
            temp=inputString();
            temp=(char*)realloc(temp, 35*sizeof(char));
            temp[34]=0;
            strcpy(data->headline, temp);
            free(temp);
        }
        else if(n==2){
            printf("Unesite opis dogadjaja (249): ");
            temp=inputString();
            temp=(char*)realloc(temp, 250*sizeof(char));
            temp[249]=0;
            strcpy(data->description, temp);
            free(temp);
        }
        else if(n==3){
            printf("Unesite datum (dd.mm.gggg.): ");
            temp=inputString();
            temp=(char*)realloc(temp, 12*sizeof(char));
            temp[11]=0;
            strcpy(data->date, temp);
            free(temp);
            flag=1;  // oznacimo ako se mijenja kljuc
        }
        else if(n==4){
            printf("Unesite vrijeme (hh:mm): ");
            temp=inputString();
            temp=(char*)realloc(temp, 6*sizeof(char));
            temp[5]=0;
            strcpy(data->description, temp);
            free(temp);
        }
        else{
            printf("Unesite lokaciju(29): ");
            temp=inputString();
            temp=(char*)realloc(temp, 30*sizeof(char));
            temp[29]=0;
            strcpy(data->description, temp);
            free(temp);
        }
        printf("Da li zelite mijenjati jos nesto?\n1. Da\n2. Ne\nIzaberite opciju: ");  //provjerimo da li se zeli mijenjati jos nesto
        n=chooseOption2(1,2);
    }while(n==1);  // petlja se okrece dok god se zeli nesto promijeniti
    printf("Da li zelite sacuvati promjene?\n1. Da\n2. Ne\nIzaberite opciju: ");  // provjera da li se zeli data promjena sacuvati
    n=chooseOption2(1,2);
    if(n==1){
        if(!isEventValid(data->date)){
            printf("Dogadjaj se desava za vise od 5 godina i nije moguce sacuvati promjene.\n");
            freeEvent(&tmp);
            Sleep(5*1000);
            newPage("Banjaluka");
            return;
        }
        position=findPosition(data->eventID);  // pronadjemo poziciju
        if(position==0) printf("ERROR!\n");  // pozicija ne moze biti pocetak fajla, doslo do greske
        else{
            if(flag==1){  // ako je doslo do promjene nekog podatka koji ima i u indeksonj datoteci azuriramo indeksnu datoteku
                n=0;
                strcpy(data1.eventID, data->eventID);
                strcpy(data1.key, data->date);
                data1.position=position;
                arr=readFromIndex(&n, "Data/Index_Datum.txt");
                printInIndex(arr, &data1, n, "Data/Index_Datum.txt", compareDatum);
            }
            openEventData(&fp, "r+");  //otvorimo glavnu datoteku
            if(fp!=NULL){
                fseek(fp, position, SEEK_SET);  // pozicioniramo se na zeljeni dogadjaj
                loadEvent(&tmp, fp);  // iscitamo dogadjaj
                old=ftell(fp)-2;  // sacuvamo krajnju poziciju tog dogadjaja prije promjene
                fseek(fp, position, SEEK_SET);
                fprintf(fp, "%s %s %s| %s| %s %s %d %s %s", data->eventID, data->user, data->headline, data->description, data->date,
                        data->category, data->finished, data->time, data->location);  // upisemo promjenu u datoteku
                neww=ftell(fp);  // izracunamo novu poziciju
                if(neww<old){
                    n=20;
                    novi=(char*)calloc(old-neww+1, sizeof(char));
                    for(i=0; i<old-neww; i++) // ako je duzina novog dogadjaja manja od starog ,napravimo niz razmaka koji ce pokriti zaostale podatke starog dogadjaja
                        novi[i]=' ';
                    fseek(fp, position, SEEK_SET);
                    fprintf(fp, "%s %s %s| %s| %s %s %d %s %s %s", data->eventID, data->user, data->headline,
                            data->description, data->date, data->category, data->finished, data->time, data->location, novi);
                }
                fclose(fp);
            }
            else printf("ERROR!\n");
        }
        printf("Uspjesno promijenjen dogadjaj.\n");
    }
    else printf("Niste sacuvali dogadjaj.\n");
    freeEvent(&tmp);
    Sleep(5*1000);
    newPage("Banjaluka");
}

void stdprint(INDEX *arr, int n){
    FILE *fp;
    EVENT data;
    createEvent(&data);
    openEventData(&fp, "r");
    if(fp!=NULL){
        printf("---------------------------------------------------------------------------------\n");
        printf("RB.          Naziv dogadjaja                     Kategorija             Datum\n");
        printf("---------------------------------------------------------------------------------\n");
        for(int i=0; i<n; i++){
            fseek(fp, arr[i].position, SEEK_SET);
            loadEvent(&data, fp);
            printf("%d.     %-34s     %-19s     %-10s\n", i+1, data.headline, data.category, data.date);
        }
        fclose(fp);
    }
    else printf("ERROR!\n");
    freeEvent(&data);
}

void printCurrentEvents(char* cityName, char* userType, char*userName){
    newPage(cityName);
    char*date=currentDate();
    INDEX *arr=NULL;
    int choice;
    int i;
    FILE *fp=fopen("Data/Index_Datum.txt", "r");
    INDEX temp={0};
    int u=20,n=0;
    if(fp!=NULL){
        arr=(INDEX*)calloc(u, sizeof(INDEX));
        while(fscanf(fp, "%s %s %d\n", temp.key, temp.eventID, &temp.position)==3){
            if(!compareDatum(date, temp.key))
                arr[n++]=temp;
            if(n==u) arr=(INDEX *)realloc(arr, (u*=2)*sizeof(INDEX));
        }
        arr=(INDEX *)realloc(arr, n*sizeof(INDEX));
        fclose(fp);
    }
    else printf("ERROR!");
    free(date);
    if(arr==NULL){
        printf("Nema dogadjaja koji se odrzavaju danas.\n");
        return;
    }
    printf("Danasnji dogadjaji:\n");
    stdprint(arr, n);

    printf("\n1) Detaljan prikaz dogadjaja\n2) Prikaz proslih dogadjaja\n3) Prikaz buducih dogadjaja\n");
    printf("4) Prikaz po kategoriji\n5) Nazad\n");
    choice = chooseOption2(1,5);
    fflush(stdin);

    switch(choice)
    {
        case 1: {
            printf("Izaberite redni br. dogadjaja: ");
            i = chooseOption2(1,n);
            printDetails(arr[i-1].eventID, cityName,userType,userName);
        }
        break;
        case 2: printPreviousEvents(cityName,userType,userName);
        break;
        case 3: printFutureEvents(cityName,userType,userName);
        break;
        case 4: printCategoryEvents(cityName,userType,userName);
        break;
        case 5:{
            free(arr);
            return;
        }
    }
    free(arr);
}

void printPreviousEvents(char* cityName, char* userType,char *userName){
    newPage(cityName);
    char*date=currentDate();
    INDEX *arr=NULL;
    FILE *fp=fopen("Data/Index_Datum.txt", "r");
    INDEX temp={0};
    int u=20,n=0,choice, i;
    if(fp!=NULL){
        arr=(INDEX*)calloc(u, sizeof(INDEX));
        while(fscanf(fp, "%s %s %d\n", temp.key, temp.eventID, &temp.position)==3){
            if(compareDatum(date, temp.key)==1)
                arr[n++]=temp;
            if(n==u) arr=(INDEX *)realloc(arr, (u*=2)*sizeof(INDEX));
        }
        arr=(INDEX *)realloc(arr, n*sizeof(INDEX));
        fclose(fp);
    }
    else printf("ERROR!");
    free(date);
    if(arr==NULL){
        printf("Nema dogadjaja proslih danas.\n");
        return;
    }
    printf("Prosli dogadjaji: \n");
    stdprint(arr, n);
    printf("\n1) Detaljan prikaz dogadjaja\n2) Prikaz danasnjih dogadjaja\n3) Prikaz buducih dogadjaja\n");
    printf("4) Prikaz po kategoriji\n5) Nazad\n");
    choice = chooseOption2(1,5);
    fflush(stdin);

    switch(choice)
    {
        case 1: {
            printf("Izaberite redni br. dogadjaja: ");
            i = chooseOption2(1,n);
            printDetails(arr[i-1].eventID, cityName, userType,userName);
        }
        break;
        case 2: printCurrentEvents(cityName,userType,userName);
        break;
        case 3: printFutureEvents(cityName,userType,userName);
        break;
        case 4: printCategoryEvents(cityName,userType,userName);
        break;
        case 5:{
            free(arr);
            return;
        }
    }
    free(arr);
}

void printFutureEvents(char* cityName, char* userType,char*userName){
    newPage(cityName);
    char*date=currentDate();
    INDEX *arr=NULL;
    FILE *fp=fopen("Data/Index_Datum.txt", "r");
    INDEX temp={0};
    int u=20,n=0,choice, i;
    if(fp!=NULL){
        arr=(INDEX*)calloc(u, sizeof(INDEX));
        while(fscanf(fp, "%s %s %d\n", temp.key, temp.eventID, &temp.position)==3){
            if(compareDatum(date, temp.key)==-1)
                arr[n++]=temp;
            if(n==u) arr=(INDEX *)realloc(arr, (u*=2)*sizeof(INDEX));
        }
        arr=(INDEX *)realloc(arr, n*sizeof(INDEX));
        fclose(fp);
    }
    else printf("ERROR!");
    free(date);
    if(arr==NULL){
        printf("Nema dogadjaja buducih danas.\n");
        return;
    }
    printf("Buduci dogadjaji: \n");
    stdprint(arr, n);
    printf("\n1) Detaljan prikaz dogadjaja\n2) Prikaz proslih dogadjaja\n3) Prikaz danasnjih dogadjaja\n");
    printf("4) Prikaz po kategoriji\n5) Nazad\n");
    choice = chooseOption2(1,5);
    fflush(stdin);

    switch(choice)
    {
        case 1: {
            printf("Izaberite redni br. dogadjaja: ");
            i = chooseOption2(1,n);
            printDetails(arr[i-1].eventID, cityName,userType,userName);
        }
        break;
        case 2: printPreviousEvents(cityName,userType,userName);
        break;
        case 3: printCurrentEvents(cityName,userType,userName);
        break;
        case 4: printCategoryEvents(cityName,userType,userName);
        break;
        case 5: {
            free(arr);
            return;
        }
    }
    free(arr);
}

char* chooseCategory(){
    int n=0, i;
    char**s=readCategories(&n), *temp=(char*)calloc(15,sizeof(char));
    for(i=0; i<n; i++)
        printf("%d.%s\n", i+1, s[i]);
    printf("Odaberite kategoriju: ");
    i=chooseOption2(1,n);
    strcpy(temp, s[i-1]);
    for(i=0; i<n; i++)
        free(s[i]);
    free(s);
    return temp;
}

void printCategoryEvents(char* cityName, char* userType,char* userName){
    newPage(cityName);
    INDEX *arr=NULL;
    FILE *fp=fopen("Data/Index_Category.txt", "r");
    INDEX temp={0};
    int u=20,n=0,choice, i;
    char *cat=chooseCategory();
    if(fp!=NULL){
        arr=(INDEX*)calloc(u, sizeof(INDEX));
        while(fscanf(fp, "%s %s %d\n", temp.key, temp.eventID, &temp.position)==3){
            if(!strcasecmp(cat, temp.key))
                arr[n++]=temp;
            if(n==u) arr=(INDEX *)realloc(arr, (u*=2)*sizeof(INDEX));
        }
        arr=(INDEX *)realloc(arr, n*sizeof(INDEX));
        fclose(fp);
    }
    else printf("ERROR!");
    if(arr==NULL){
        printf("Nema dogadjaja '%s' kategorije.\n", cat);
        return;
    }
    printf("Dogadjaji '%s' kategorije: \n", cat);
    stdprint(arr, n);
    free(cat);

    printf("\n1) Detaljan prikaz dogadjaja\n2) Prikaz proslih dogadjaja\n3) Prikaz sadasnjih dogadjaja\n");
    printf("4) Prikaz buducih dogadjaja\n5) Nazad\n");
    choice = chooseOption2(1,5);
    fflush(stdin);

    switch(choice)
    {
        case 1: {
            printf("Izaberite redni br. dogadjaja: ");
            i = chooseOption2(1,n);
            printDetails(arr[i-1].eventID, cityName,userType,userName);
        }
        break;
        case 2: printPreviousEvents(cityName,userType,userName);
        break;
        case 3: printCurrentEvents(cityName,userType,userName);
        break;
        case 4: printFutureEvents(cityName,userType,userName);
        break;
        case 5: {
            free(arr);
            return;
        }
    }
    free(arr);
}

void printDetails(char *eventID, char *cityName,char* userType, char* userName)
{
    newPage(cityName);
    EVENT eve;
    createEvent(&eve);
    COMMENT com;
    createComment(&com);
    FILE *fp;
    openEventData(&fp,"r");
    int n,i;
    fscanf(fp, "%d\n", &n);
    while(loadEvent(&eve,fp))
    {
        if(strcmp(eve.eventID,eventID)==0)
        {
            printf("Korisnik koji je kreirao dogadjaj: %s\n",eve.user);
            printf("Naziv dogadjaja: %s\n",eve.headline);
            printf("Datum: %s\n",eve.date);
            printf("Kategorija: %s\n",eve.category);
            printf("Opis: %s\n",eve.description);
        }
    }

    fclose(fp);
    openCommentData(&fp,"r");
    while(fscanf(fp, "%s,%s,%[^\n]\n",com.comEventID,com.comUsername,com.commentText)!=EOF)
    {
        if(strcmp(com.comEventID,eventID)==0)
        {
            printf("%s:",com.comUsername);
            printf("%s\n",com.commentText);
        }
    }
    fclose(fp);

    if(!strcmp(userType,"admin"))
    {
        printf("\n1)Izmjeni dogadjaj\n2)Izbrisi dogadjaj\n3)Dodaj komentar\n4)Brisi komentar\n5)Nazad\n");
        i=chooseOption2(1,5);
        fflush(stdin);
        switch(i)
        {
        case 1: editEvent(&eve);
            break;
        case 2: removeEvent(eve.eventID);
            break;
        case 3: {
            char*kom = (char*)calloc(200,sizeof(char));
            printf("\nNapisi komentar\n");
            gets(kom);
            fflush(stdin);
            addComment(atoi(eve.eventID),userName,kom);
        }
            break;
        case 4:
            {
                COMMENT comm,pom[15];
                createComment(&comm);
                FILE *fp;
                int j = 0;
                openCommentData(&fp, "r");
                while(loadComment(&com,fp))
                {
                    if(strcmp(eventID,com.comEventID))
                    {
                        printf("%d) %s , %s\n",++j, comm.comUsername, comm.commentText);
                        createComment(&pom[j-1]);
                        pom[j-1] = comm;
                    }
                }

                printf("\nIzaberi komentar: ");
                int h;
                scanf("%d",&h);
                fflush(stdin);

                deleteComment(atoi(pom[h-1].comEventID),pom[h-1].comUsername,pom[h-1].commentText);
            }
            break;
        case 5: return;
            break;
        default: break;
        }
    }
    else if (!strcmp(userType,"user"))
    {
        printf("\n1)Izmjeni dogadjaj\n2)Izbrisi dogadjaj\n3)Dodaj komentar\n4)Nazad\n");
        i=chooseOption2(1,5);
        fflush(stdin);
        switch(i)
        {
        case 1: editEvent(&eve);
            break;
        case 2: removeEvent(eve.eventID);
            break;
        case 3: {
            char*kom = (char*)calloc(200,sizeof(char));
            printf("\nNapisi komentar\n");
            gets(kom);
            fflush(stdin);
            addComment(atoi(eve.eventID),userName,kom);
        }
            break;
        case 4: return;
            break;
        default: break;
        }
    }
    else
    {
        printf("\n1)Nazad\n");
        scanf("%d",&i);
        fflush(stdin);
    }

    freeEvent(&eve);
    freeComment(&com);
}

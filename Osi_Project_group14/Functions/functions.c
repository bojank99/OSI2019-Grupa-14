#include"functions.h"

void writeCity(FILE *fp)
{
    //NAPOMENA, moramo dodati sve podrazumijevane fajlove pri prvom pokretanju app
    rewind(fp);

    char cityName[20];
    printf("Naziv grada: ");
    gets(cityName);

    fputs("1\n",fp);
    fputs(cityName,fp);

    FILE *firstFile;

    firstFile = fopen("Data/Events.txt","w");
    fclose(firstFile);

    firstFile = fopen("Data/Index_Category.txt","w");
    fclose(firstFile);

    firstFile = fopen("Data/Index_Datum.txt","w");
    fclose(firstFile);

    firstFile = fopen("Data/Kategorije.txt","w");
    fputs("Izlozbe\nKoncerti\nPromocije\n",firstFile);
    fclose(firstFile);

    firstFile = fopen("Data/Komentar_za_Odobravanje.txt","w");
    fclose(firstFile);

    firstFile = fopen("Data/Komentari.txt","w");
    fclose(firstFile);

    firstFile = fopen("Data/Korisnicki_zahtjevi.txt","w");
    fclose(firstFile);

    firstFile = fopen("Data/Users.txt","w");
    fputs("00001\n00001 Admin admin admin\n",firstFile);
    fclose(firstFile);

    printf("\nNapravite kviz\n");
    inputQuizQuestions();

}


void isThereCity(char cityName[20])
{
    int c;                                  //promjenjiva koja ocita prvu vrijednost iz datoteke
    FILE *fp;

    fp=fopen("Data/Ime_Grada.txt","r+");

    if(fp!=NULL)
        c=fgetc(fp);
    else printf("Greska");

    if(c=='0')                              //ako je prvi ulazak u datoteku upise novi grad, u suprotnom ispise naziv grada
        writeCity(fp);

    rewind(fp);
    fgetc(fp);
    fgetc(fp);
    fgets(cityName,20,fp);

    fclose(fp);
}


void newPage(char cityName [])
{
    #if defined(__linux__) || defined(__unix__) || defined(__APPLE__)
        system("clear");
    #endif

    #if defined(_WIN32) || defined(_WIN64)
        system("cls");
    #endif
    printf("=== Grad %s ===\n",cityName);
}

void deleteCity()
{
    FILE *newFile1;

    remove("Data/Events.txt");
    remove("Data/Ime_Grada.txt");
    remove("Data/Index_Category.txt");
    remove("Data/Index_Datum.txt");
    remove("Data/Kategorije.txt");
    remove("Data/Komentar_za_Odobravanje.txt");
    remove("Data/Komentari.txt");
    remove("Data/Korisnicki_zahtjevi.txt");
    remove("Data/Users.txt");

    newFile1=fopen("Data/Events.txt","w");
    fclose(newFile1);

    newFile1=fopen("Data/Ime_Grada.txt","w");
    fputc('0',newFile1);
    fclose(newFile1);

    newFile1=fopen("Data/Index_Category.txt","w");
    fclose(newFile1);

    newFile1=fopen("Data/Index_Datum.txt","w");
    fclose(newFile1);

    newFile1=fopen("Data/Kategorije.txt","w");
    fputs("Izlozbe\nKoncerti\nPromocije\n",newFile1);
    fclose(newFile1);

    newFile1=fopen("Data/Komentar_za_Odobravanje.txt","w");
    fclose(newFile1);

    newFile1=fopen("Data/Komentari.txt","w");
    fclose(newFile1);

    newFile1=fopen("Data/Korisnicki_zahtjevi.txt","w");
    fclose(newFile1);

    newFile1=fopen("Data/Users.txt","w");
    fputs("00001\n00001 Admin admin admin\n",newFile1);
    fclose(newFile1);
}


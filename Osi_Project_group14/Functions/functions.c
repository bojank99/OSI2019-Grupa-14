#include"functions.h"

void writeCity(FILE *fp)
{

    rewind(fp);

    char cityName[20];
    printf("Naziv grada: ");
    gets(cityName);

    fputs("1\n",fp);
    fputs(cityName,fp);

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
    FILE *newFile1,*newFile2,*newFile3;
    FILE *newFile4,*newFile5,*newFile6;
    FILE *newFile7,*newFile8,*newFile9;

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
    fputc('0',newFile1);
    fclose(newFile1);

    newFile2=fopen("Data/Ime_Grada.txt","w");
    fputc('0',newFile2);
    fclose(newFile2);

    newFile3=fopen("Data/Index_Category.txt","w");
    fclose(newFile3);

    newFile4=fopen("Data/Index_Datum.txt","w");
    fclose(newFile4);

    newFile5=fopen("Data/Kategorije.txt","w");
    fclose(newFile5);

    newFile6=fopen("Data/Komentar_za_Odobravanje.txt","w");
    fclose(newFile6);

    newFile7=fopen("Data/Komentari.txt","w");
    fclose(newFile7);

    newFile8=fopen("Data/Korisnicki_zahtjevi.txt","w");
    fclose(newFile8);

    newFile9=fopen("Data/Users.txt","w");
    fputc('0',newFile9);
    fclose(newFile9);
}


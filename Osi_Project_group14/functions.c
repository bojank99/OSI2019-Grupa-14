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

    fp=fopen("Data\\Ime_Grada.txt","r+");

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
    system("cls");
    printf("=== Grad %s ===\n",cityName);
}


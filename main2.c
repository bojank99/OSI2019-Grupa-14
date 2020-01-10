#define pom 10
#define size 200
#define _GNU_SOURCE
#include <stdio.h>
#include <conio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include <ctype.h>
#define MAX 30


void pitaj(char opcija[size], const char opcija1[size], const char opcija2[size],
	const char opcija3[size], const char opcija4[size],const char tacno[size],FILE * fp,int *);

void pomoc()
{
    printf("\n\n                              POMOC");
    printf("\n -------------------------------------------------------------------------");
    printf("\n >> Kviz zapocinjete pritiskom na taster enter.");
    printf("\n >> Nakon sto ste pritisnuli enter, mozete zapoceti igranje kviza koji se sastoji");
    printf("\n    od 10 pitanja. Svako pitanje ide jedno ispod drugog. Nakon svakog pitanja ");
    printf("\n    imate ponudjene odgovore pod A, B, C i D. Moguce je unijeti samo jedno slovo,");
    printf("\n    jer za svako pitanje postoji jedinstven odgovor.");
    printf("\n    Svako tacno pitanje nosi 10 bodova. Ne postoje negativni bodovi.");
    printf("\n    Kada se zavrsi kviz, odnosno kada zavrsite svih 10 pitanja dobicete tacne odgovore na");
    printf("\n    svako vase pitanje kao i ukupan broj osvojenih bodova.");
    printf("\n >> Srecno!");
    printf("\n");
}

void Prikaz()
{
     printf("\n\t\t________________________________________");
     printf("\n\t\t\t  DOBRODOSLI ");
     printf("\n\t\t\t      U ");
     printf("\n\t\t\t    KVIZ ");
     printf("\n\t\t________________________________________");
     printf("\n\t\t > Pritisnite slovo P za pomoc ukoliko zelite da saznate pravila igre.                 ");
     printf("\n\t\t > Pritisnite enter za zapocinjanje kviza.               ");
     printf("\n\t\t________________________________________\n\n");
     char opcija;
     opcija=toupper(getch());
      if(opcija=='P')
      {
          pomoc();
          getch();
      }
}

int main()
{
    char opcija1[size], opcija2[size], opcija3[size], opcija4[size], opcija[size], tacno[size];
	char mat[256][256];
	int i,j,k;
	int niz[MAX],x,loc;
	time_t t;
	char linija[256];
	int rezultat = 0;

    FILE * fp;
    FILE *fp1;
    fp = fopen("kviz.txt","r");
    fp1 = fopen("novi.txt","w");
    if(fp == NULL)
    {
    	printf("\n\n\n \t\t\t Fajl se ne moze otvoriti! \t\t\t \n\n\n");
	}
	if(fp1 == NULL)
	{
		printf("\n\n\n \t\t\t Fajl novi.txt se ne moze napraviti! \t\t\t \n\n\n");
	}

    Prikaz();


	k = 0;

	for (i = 0; i < 30; i++)
	{
		niz[i] = 10 * i + 1;
	}

	for (j = 0; j < pom; j++)
	{
		srand((unsigned)time(&t));
		x = rand() % 3;
		loc = (3 * j) + x;

		for (i = niz[3 * j]; i < niz[loc]; i++)
		{
		fgets(mat[k],sizeof(mat[k]),fp);
			k++;
		}

		for (i = 1; i <= 10; i++)
		{
			fgets(linija,sizeof(linija),fp);
			if(i==1)
			{
			  printf("\n\n\n\n\n\nQ%d) %s",j+1,linija);
			  fprintf(fp1,"Q%d) %s",j+1,linija);

			}

				if(i==2)
				{
					fputs(linija,fp1);
				}

				if(i == 3)
			strncpy(opcija1,linija,30);

				if(i == 4)
			strncpy(opcija2,linija,30);

				if(i == 5)
			strncpy(opcija3,linija,30);

				if(i == 6)
			strncpy(opcija4,linija,30);

			if (i >1&&i<=7)
			{
				printf("%s",linija);

			}
			if (i == 8)
			strncpy(tacno,linija,30);


		}


		pitaj(opcija, opcija1, opcija2, opcija3, opcija4, tacno,fp1,&rezultat);

		if(j!=9)
		{

		if (niz[loc] + 10 < niz[3 * (j + 1)])
		  {
			for (i = niz[loc] + 10; i < niz[3 * (j + 1)]; i++)
			{
			fgets(mat[k],sizeof(mat[k]),fp);
				k++;
			}
		  }
		}


		system("cls");
	}

	fprintf(fp1," \n  Dobili ste ukupno %d od 100 ",rezultat);

	fclose(fp);

	fclose(fp1);

	fp1 = fopen("novi.txt","r");

	while(fgets(linija,sizeof(linija),fp1))
	{
		printf(" %s",linija);
	}

	fclose(fp1);
	system("pause");
}



	void pitaj(char opcija[size], const char opcija1[size], const char opcija2[size], const char opcija3[size], const char opcija4[size],
	 const char tacno[size],FILE * fp,int *rezultat)
	{
		printf("Vas odgovor: ");
		scanf("%str",&opcija[0]);
		opcija[1] = '\0';
		strupr(opcija);

			char tacan_odgovor[size];
			char netacan_odgovor[size];


		if (opcija[0] == tacno[0])
		{
			if (opcija[0] == opcija1[0])
				fprintf(fp," \nTacno ste odgovorili na pitanje : %s  \n ", opcija1);
			if (opcija[0] == opcija2[0])
				fprintf(fp," \nTacno ste odgovorili na pitanje : %s  \n ", opcija2);
			if (opcija[0] == opcija3[0])
				fprintf(fp," \nTacno ste odgovorili na pitanje : %s  \n ", opcija3);
			if (opcija[0] == opcija4[0])
				fprintf(fp," \nTacno ste odgovorili na pitanje : %s  \n ", opcija4);

				*rezultat += 10;

		}
		else
		{

			if (tacno[0] == opcija1[0])
				strncpy(tacan_odgovor, opcija1,24);
			if (tacno[0] == opcija2[0])
				strncpy(tacan_odgovor, opcija2,24);
			if (tacno[0] == opcija3[0])
				strncpy(tacan_odgovor, opcija3,24);
			if (tacno[0] == opcija4[0])
				strncpy(tacan_odgovor, opcija4,24);

			tacan_odgovor[24] = '\0';

			if (opcija[0] == opcija1[0])
				strncpy(netacan_odgovor, opcija1,24);
			if (opcija[0] == opcija2[0])
				strncpy(netacan_odgovor, opcija2,24);
			if (opcija[0] == opcija3[0])
				strncpy(netacan_odgovor, opcija3,24);
			if (opcija[0] == opcija4[0])
				strncpy(netacan_odgovor, opcija4,24);

			netacan_odgovor[24] = '\0';

			fprintf(fp,"   Odgovorili ste   : %s \n", netacan_odgovor);
			fprintf(fp," Tacan odgovor je : %s \n", tacan_odgovor);
		}

			fprintf(fp," -----------------------------------------------------------------------------\n");
			fprintf(fp," -----------------------------------------------------------------------------\n");
	}







#include <stdio.h>
#include <stdlib.h>

 void unos()
{
    static const char imefajla[] = "pitanja.txt";
   FILE *fp = fopen ( imefajla, "r" );
   if ( fp != NULL )
   {
      char linija [ 128 ];
      while ( fgets ( linija, sizeof linija, fp ) != NULL )
      {
         fputs ( linija, stdout);
      }
      fclose (fp);
   }
   else
   {
     printf("Greska");
   }
}

int main()
{
   unos();
}

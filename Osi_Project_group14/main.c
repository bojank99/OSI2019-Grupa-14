#include <stdio.h>
#include <stdlib.h>
#include"functions.h"
#include"Show.h"

int main()
{
    char cityName[20];
    short isLoged = 0;

    isThereCity(cityName);

    newPage(cityName);

    chooseOption(cityName);

    return 0;

}

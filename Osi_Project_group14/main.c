#include <stdio.h>
#include <stdlib.h>
#include "Functions/functions.h"
#include "Functions/Show.h"


int main()
{
    char cityName[20];
    //short isLoged = 0;

    isThereCity(cityName);

    chooseOption(cityName);

    return 0;

}

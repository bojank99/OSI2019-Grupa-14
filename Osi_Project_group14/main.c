#include <stdio.h>
#include <stdlib.h>
#include "Functions/functions.h"
#include "Functions/DataManage.h"


int main()
{
    char cityName[20];

    isThereCity(cityName);

    chooseOption(cityName);
    return 0;
}

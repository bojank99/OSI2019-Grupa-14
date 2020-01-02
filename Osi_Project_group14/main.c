#include <stdio.h>
#include <stdlib.h>
#include "Functions/functions.h"
#include "Functions/DataManage.h"


int main()
{
    FILE* unUserFile;
    UNREGUSER* temp=(UNREGUSER*)calloc(1,sizeof(UNREGUSER));
    allocUnregUser(temp);
    openUnregUserData(&unUserFile,"r+");
    loadUnregUser(temp,unUserFile);
    printf("%s %s %s %s %s",temp->base.userName,temp->base.password,
           temp->email,temp->name,temp->surname);
    return 0;

}

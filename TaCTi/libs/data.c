#include <stdio.h>
#include <stdlib.h>
#include "data.h"
#include "TDA_Lista.h"


void printString(const void *data)
{
    const char *text = (const char *)data;
    printf("%s\n", text);
}

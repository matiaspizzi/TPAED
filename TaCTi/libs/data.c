#include <stdio.h>
#include <stdlib.h>
#include "data.h"
#include "TDA_Lista.h"

tInput input = {{0},0,0};

void printString(const void *data)
{
    const char *text = (const char *)data;
    printf("%s\n", text);
}

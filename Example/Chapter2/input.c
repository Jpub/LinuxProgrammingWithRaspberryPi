#include <stdio.h>

char* input()
{
    static char *str, a[32];
    str = a;
    scanf("%s", a);

    return str;
}

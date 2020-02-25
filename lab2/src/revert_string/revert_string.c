#include "revert_string.h"

void RevertString(char *str)
{
	int n = 0;
    int i = 0;
    for (i = 0; str[i]!= 0; i++)
    {
        n++;
        //printf("%c\n", str[i]);
    }
    //printf("n: %d\n", n);
    for (i = 0; i < (n+1)/2; i++)
    {
        //printf("%c %c\n", str[i], str[n-i-1]);
        char between = str[i];
        str[i] = str[n-i-1];
        str[n-i-1] = between;
    }
}


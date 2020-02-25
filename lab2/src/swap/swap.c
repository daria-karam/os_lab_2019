#include "swap.h"

void Swap(char *left, char *right)
{
	char between = *left;
    *left = *right;
    *right = between;
}
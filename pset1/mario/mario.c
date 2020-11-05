//Problem Set 1, Mario World 1-1. Prints a right-aligned pyramid 

#include <cs50.h>
#include <stdio.h>

int main(void)
{
    int n;
    //Prompts user for a positive integer between 1 and 8, inclusive
    do
    {
        n = get_int("Height: ");
    }
    while (n < 1 || n > 8);
    //Create n rows
    for (int i = 0; i < n; i++)
    {
        //Print " " (n - 1 - i) times
        for (int j = n - 1; j > i; j--)
        {
            printf(" ");
        }
        //Print "#" (i + 1) times
        for (int k = -1; k < i; k++)
        {
            printf("#");
        }
        printf("\n");
    }
}
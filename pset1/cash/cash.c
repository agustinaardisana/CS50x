//Problem Set 1, Cash. Prints the minimum number of coins with which the imputed change can be made.

#include <cs50.h>
#include <stdio.h>
#include <math.h>

int main(void)
{
    int c = 0;
    float n;
    
//Prompt user for the change owed in dollars (n)
    do
    {
        n = get_float("Change owed: "); 
    }
    while (n < 0);
    
    //Convert imputed dollars (n) to cents (m) (multiply and round)
    n = round(n * 100);
    
    //Compute coins (keep track of remaining change and coins used with loops)
    while (n > 0)
    {
        if (n >= 25)
        { 
            n = n - 25;
            c++;
        }
        else if (n < 25 && n >= 10)
        {
            n = n - 10;
            c++;
        }
        else if (n < 10 && n >= 5)
        {
            n = n - 5;
            c++;
        }
        else
        {
            n = n - 1;
            c++;
        }
    }
    //Print the result number of coins (printf %i\n, coins)
    printf("Coins used: %d\n", c);
}
  





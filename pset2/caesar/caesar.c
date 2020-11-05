#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

//Get key
int main(int argc, string argv[])
{
    //Check for command-line argument
    if (argc == 2)
    {
        //iterates to make sure is a positive int
        for (int i = 0; i < strlen(argv[1]); i++)
        {
            //returns error message if is other than positive int
            if (isdigit(argv[1][i]) == false)
            {
                printf("Usage :./caesar Key\n");
                return 1;
            }
        }

        //Convert string to int
        int key = atoi(argv[1]);
        //Get plaintext
        string plaintext = get_string("plaintext:  ");
        printf("ciphertext:\n");

        for (int i = 0; i < strlen(plaintext); i++)
            //Encipher (If uppercase: rotate it, preserving case; If lowercase: rotate it, preserving case; If neither of them: print out character as is)
        {
            if (plaintext[i] >= 'A' && plaintext[i] <= 'Z')
            {
                printf("%c", (((plaintext[i] - 'A') + key) % 26) + 'A');
            }
            else if (plaintext[i] >= 'a' && plaintext[i] <= 'z')
            {
                printf("%c", (((plaintext[i] - 'a') + key) % 26) + 'a');
            }
            else
            {
                printf("%c", plaintext[i]);
            }
        }
        printf("\n");
        return 0 ;
    }
    else
    {
        printf("Usage :./caesar Key\n");
        return 1;
    }
}

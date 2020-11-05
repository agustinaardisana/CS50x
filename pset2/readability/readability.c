#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

int main(void)
{
    //get imput
    string text = get_string("Text: ");

    float letters = 0, words = 1, sentences = 0;

    for (int i = 0, n = strlen(text); i < n; i++)
    {
        //count the number of letter in a word (exclude spaces)
        if (isalpha(text[i]))
        {
            letters++;
        }
        //count words in a sentence (assume any sequence of char separated for one or more spaces is a word)
        if (isspace(text[i]))
        {
            words++;
        }
        //count number of sentences (assume that any ".","?" or "!" indicates a sentence)
        if (text[i] == '.' || text[i] == '?' || text[i] == '!')
        {
            sentences++;
        }
    }
    //calculate the Coleman-Liau index value (index = 0.0588 * L - 0.296 * S - 15.8 where L=avg n of letters/100 words && S= number of sentences/100 words)
    float l = letters / words * 100;
    float s = sentences / words * 100;
    float index = 0.0588 * l - 0.296 * s - 15.8;
    int indexi = round(index);

    //Output score "Grade x"(x= round to the nearest whole number for grades 1~16 or "Before grade 1/Grade 16+")
    if (indexi >= 1 && indexi <= 16)
    {
        printf("Grade %i\n", indexi);
    }
    else
    {
        if (indexi < 1)
        {
            printf("Before Grade 1\n");
        }
        if (indexi > 16)

        {
            printf("Grade 16+\n");

        }
    }
}







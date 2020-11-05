#include "helpers.h"
#include "math.h"

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    // iterate through i to get to each row and find values of R, G and B
    for (int i = 0; i < height; i++)
    {
        // iterate again to get to each column and find values of R, G and B
        for (int j = 0; j < width; j++)
        {
            //access pixels into the 2D array and obtain value of each colour
            int red = image[i][j].rgbtRed;
            int blue = image[i][j].rgbtBlue;
            int green = image[i][j].rgbtGreen;

            //calculate and round the average value of each pixel
            int average = round(((float)red + (float)blue + (float)green) / 3);

            //set the calculated average to be the new value of each pixel
            image[i][j].rgbtRed = average;
            image[i][j].rgbtBlue = average;
            image[i][j].rgbtGreen = average;

        }
    }
}

// Convert image to sepia
void sepia(int height, int width, RGBTRIPLE image[height][width])
{
    // iterate through i to get to each row and find values of R, G and B
    for (int i = 0; i < height; i++)
    {
        // iterate again to get to each column and find values of R, G and B
        for (int j = 0; j < width; j++)
        {
            //access pixels into the 2D array and obtain value of each colour
            int red = image[i][j].rgbtRed;
            int green = image[i][j].rgbtGreen;
            int blue = image[i][j].rgbtBlue;

            //convert each value to the sepia value and make sure it doesn't exceed 255
            int sepiaRed = round(.393 * red + .769 * green + .189 * blue);

            if (sepiaRed < 256)
            {
                image[i][j].rgbtRed = sepiaRed;
            }
            else
            {
                image[i][j].rgbtRed = 255;
            }

            int sepiaGreen = round(.349 * red + .686 * green + .168 * blue);

            if (sepiaGreen < 256)
            {
                image[i][j].rgbtGreen = sepiaGreen;
            }
            else
            {
                image[i][j].rgbtGreen = 255;

            }

            int sepiaBlue = round(.272 * red + .534 * green + .131 * blue);
            if (sepiaBlue < 256)
            {
                image[i][j].rgbtBlue = sepiaBlue;
            }
            else
            {
                image[i][j].rgbtBlue = 255;

            }

        }

    }
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    // iterate through i to get to each row and find values of R, G and B
    for (int i = 0; i < height; i++)
    {
        //iterate through the array until reaching the mid-point
        for (int j = 0; j < (width / 2); j++)
        {
            //Store elements in a temporary storage
            RGBTRIPLE temp = image [i][j];

            //swap and reinsert back into the opposite side
            image [i][j] = image[i][(width - (j + 1))];
            image[i][(width - (j + 1))] = temp;
        }
    }
    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    //Create variables
    int totalRed;
    int totalGreen;
    int totalBlue;
    float pixelCounter; // used to count pixels used in each rotation

    //create a temporary copy where the blurring occurs
    RGBTRIPLE newImage[height][width];

    for (int i = 0; i < width; i++)
    {
        for (int j = 0; j < height; j++)
        {
            totalRed = 0;
            totalGreen = 0;
            totalBlue = 0;
            pixelCounter = 0;

            //iterate through i and j and sum values of each pixel and the 8 surrounding ones
            for (int k = -1; k < 2; k++)
            {
                if (j + k < 0 || j + k > (height - 1))
                {
                    continue; //skips iteration if it falls outside the pic
                }

                for (int l = -1; l < 2; l++)
                {
                    if (i + l < 0 || i + l > (width - 1))
                    {
                        continue; //skips iteration if it falls outside the pic
                    }

                    totalRed += image[k + j][l + i].rgbtRed;
                    totalGreen += image[k + j][l + i].rgbtGreen;
                    totalBlue += image[k + j][l + i].rgbtBlue;
                    pixelCounter++;
                }
            }
            //average values (add individual r,g and b values and divide by total pixels)
            newImage[j][i].rgbtRed = round(totalRed / pixelCounter);
            newImage[j][i].rgbtGreen = round(totalGreen / pixelCounter);
            newImage[j][i].rgbtBlue = round(totalBlue / pixelCounter);
        }
    }
    //copies values from temporary table
    for (int i = 0; i < width; i++)
    {
        for (int j = 0; j < height; j++)
        {
            image[j][i].rgbtBlue = newImage[j][i].rgbtBlue;
            image[j][i].rgbtGreen = newImage[j][i].rgbtGreen;
            image[j][i].rgbtRed = newImage[j][i].rgbtRed;
        }
    }
}
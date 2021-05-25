#include "helpers.h"
#include <math.h>

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    float average;
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            average = (float)(image[i][j].rgbtBlue + image[i][j].rgbtGreen + image[i][j].rgbtRed) / 3;

            image[i][j].rgbtBlue = round(average);
            image[i][j].rgbtGreen = round(average);
            image[i][j].rgbtRed = round(average);
        }
    }
    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE temp;
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width / 2; j++)
        {
            temp = image[i][j];
            image[i][j] = image[i][width-j-1];
            image[i][width-j-1] = temp;
        }
    }
    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    int blue;
    int green;
    int red;
    int counter;

    RGBTRIPLE cimage[height][width];

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            cimage[i][j] = image[i][j];
        }
    }

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            blue = 0;
            green = 0;
            red = 0;
            counter = 1;

            if (j - 1 >= 0)
            {
                blue = cimage[i][j-1].rgbtBlue;
                green = cimage[i][j - 1].rgbtGreen;
                red = cimage[i][j-1].rgbtRed;
                counter++;

                if (i - 1 >= 0)
                {
                    blue = blue + cimage[i - 1][j-1].rgbtBlue;
                    green = green + cimage[i - 1][j - 1].rgbtGreen;
                    red = red + cimage[i - 1][j-1].rgbtRed;
                    counter++;
                }

                if (i + 1 < height)
                {
                    blue = blue + cimage[i + 1][j - 1].rgbtBlue;
                    green = green + cimage[i + 1][j - 1].rgbtGreen;
                    red = red + cimage[i + 1][j - 1].rgbtRed;
                    counter++;
                }
            }

            if (j + 1 < width)
            {
                blue =  blue + cimage[i][j + 1].rgbtBlue;
                green = green + cimage[i][j + 1].rgbtGreen;
                red = red + cimage[i][j + 1].rgbtRed;
                counter++;

                if (i - 1 >= 0)
                {
                    blue = blue + cimage[i - 1][j+1].rgbtBlue;
                    green = green + cimage[i - 1][j+1].rgbtGreen;
                    red = red + cimage[i - 1][j+1].rgbtRed;
                    counter++;
                }

                if (i + 1 < height)
                {
                    blue = blue + cimage[i + 1][j + 1].rgbtBlue;
                    green = green + cimage[i + 1][j + 1].rgbtGreen;
                    red = red + cimage[i + 1][j + 1].rgbtRed;
                    counter++;
                }
            }

            if (i - 1 >= 0)
            {
                blue = blue + cimage[i - 1][j].rgbtBlue;
                green = green + cimage[i - 1][j].rgbtGreen;
                red = red + cimage[i - 1][j].rgbtRed;
                counter++;
            }

            if (i + 1 < width)
            {
                blue = blue + cimage[i + 1][j].rgbtBlue;
                green = green + cimage[i + 1][j].rgbtGreen;
                red = red + cimage[i + 1][j].rgbtRed;
                counter++;
            }

            image[i][j].rgbtBlue = round((float)(blue + cimage[i][j].rgbtBlue) / counter);
            image[i][j].rgbtGreen = round((float)(green + cimage[i][j].rgbtGreen) / counter);
            image[i][j].rgbtRed = round((float)(red + cimage[i][j].rgbtRed)/ counter);
        }
    }
    return;
}

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE cimage[height + 2][width + 2];

    int Gx[3][3];
    int Gy[3][3];

    int Gx_blue;
    int Gx_green;
    int Gx_red;

    int Gy_blue;
    int Gy_red;
    int Gy_green;

    //copy picture with black borders

    for (int i = 0; i < height + 2; i++) //copy image
    {
        for (int j = 0; j < width + 2; j++)
        {
            if (i == 0 || i == height + 1 || j == 0 || j == width + 1)
            {
                cimage[i][j].rgbtBlue = 0;
                cimage[i][j].rgbtGreen = 0;
                cimage[i][j].rgbtRed = 0;
            }
            else
            {
                cimage[i][j] = image[i - 1][j - 1];
            }
        }
    }

    //create Gx and Gy convolution matrices

    Gx[0][0] = -1;
    Gx[1][0] = -2;
    Gx[2][0] = -1;

    Gy[0][0] = -1;
    Gy[0][1] = -2;
    Gy[0][2] = -1;

    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            if (j == 1)
            {
                Gx[i][j] = 0;
            }
            else if (j == 2)
            {
                Gx[i][j] = -1 * Gx[i][j - 2];
            }

            if (i == 1)
            {
                Gy[i][j] = 0;
            }
            else if (i == 2)
            {
                Gy[i][j] = -1 * Gy[i - 2][j];
            }
        }
    }

    //weigted sum
    for (int i = 0; i < height + 2; i++)
    {
        for (int j = 0; j < width + 2; j++)
        {
            Gx_blue = 0;
            Gx_green = 0;
            Gx_red = 0;

            Gy_blue = 0;
            Gy_red = 0;
            Gy_green = 0;

            if (i != 0 && i != height + 1 && j != 0 && j != width + 1)
            {
                for (int k = 0; k < 3; k++)
                {
                    for (int h = 0; h < 3; h++)
                    {
                        Gx_blue = Gx_blue + (Gx[k][h] * cimage[i - 1 + k][j - 1 + h].rgbtBlue);
                        Gx_green = Gx_green + (Gx[k][h] * cimage[i - 1 + k][j - 1 + h].rgbtGreen);
                        Gx_red = Gx_red + (Gx[k][h] * cimage[i - 1 + k][j - 1 + h].rgbtRed);

                        Gy_blue = Gy_blue + (Gy[k][h] * cimage[i - 1 + k][j - 1 + h].rgbtBlue);
                        Gy_green = Gy_green + (Gy[k][h] * cimage[i - 1 + k][j - 1 +h].rgbtGreen);
                        Gy_red = Gy_red + (Gy[k][h] * cimage[i - 1 + k][j - 1 + h].rgbtRed);

                    }
                }

                if (round((float)sqrt(pow(Gx_blue,2) + pow(Gy_blue,2))) > 255)
                {
                    image[i - 1][j - 1].rgbtBlue = 255;
                }
                else
                {
                    image[i - 1][j - 1].rgbtBlue = round((float)sqrt(pow(Gx_blue,2) + pow(Gy_blue,2)));
                }

                if (round((float)sqrt(pow(Gx_green,2) + pow(Gy_green,2))) > 255)
                {
                    image[i - 1][j - 1].rgbtGreen = 255;
                }
                else
                {
                    image[i - 1][j - 1].rgbtGreen = round((float)sqrt(pow(Gx_green,2) + pow(Gy_green,2)));
                }

                if (round((float)sqrt(pow(Gx_red,2) + pow(Gy_red,2))) > 255)
                {
                    image[i - 1][j - 1].rgbtRed = 255;
                }
                else
                {
                     image[i - 1][j - 1].rgbtRed = round((float)sqrt(pow(Gx_red,2) + pow(Gy_red,2)));
                }
            }
        }
    }

    return;
}

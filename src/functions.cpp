#include "functions.h"
#include <cmath>

// Convert image to grayscale
void grayscale(int height, int width, vector<vector<RGBTRIPLE>>& image)
{
    for (int i = 0; i < height; ++i)
    {
        for (int j = 0; j < width; ++j)
        {
            float average =
                round((image[i][j].rgbtBlue + image[i][j].rgbtGreen + image[i][j].rgbtRed) / 3.0);
            image[i][j].rgbtBlue = image[i][j].rgbtGreen = image[i][j].rgbtRed = static_cast<BYTE>(average);
        }
    }
    return;
}

// Reflect image horizontally
void reflect(int height, int width, vector<vector<RGBTRIPLE>>& image)
{
    for (int i = 0; i < height; ++i)
    {
        for (int j = 0; j < width / 2; ++j)
        {
            swap(image[i][j], image[i][width - j - 1]);
        }
    }
    return;
}

// Blur image
void blur(int height, int width, vector<vector<RGBTRIPLE>>& image)
{
    vector<vector<RGBTRIPLE>> temp(height, vector<RGBTRIPLE>(width));
    for (int i = 0; i < height; ++i)
    {
        for (int j = 0; j < width; ++j)
        {
            float averagered = 0, averageblue = 0, averagegreen = 0;
            int counter = 0;
            for (int k = i - 1; k <= i + 1; ++k)
            {
                for (int l = j - 1; l <= j + 1; l++)
                {
                    if (k >= 0 && k < height && l >= 0 && l < width)
                    {
                        counter++;
                        averagered += image[k][l].rgbtRed;
                        averageblue += image[k][l].rgbtBlue;
                        averagegreen += image[k][l].rgbtGreen;
                    }
                }
            }
            temp[i][j].rgbtRed = static_cast<BYTE>(round(averagered / counter));
            temp[i][j].rgbtGreen = static_cast<BYTE>(round(averagegreen / counter));
            temp[i][j].rgbtBlue = static_cast<BYTE>(round(averageblue / counter));
        }
    }

    for (int i = 0; i < height; ++i)
    {
        for (int j = 0; j < width; ++j)
        {
            image[i][j] = temp[i][j];
        }
    }
    return;
}

// Detect edges
void edges(int height, int width, vector<vector<RGBTRIPLE>>& image)
{
    vector<vector<RGBTRIPLE>> temp(height, vector<RGBTRIPLE>(width));
    vector<vector<int>> Gx = {{-1, -2, -1}, {0, 0, 0}, {1, 2, 1}};
    vector<vector<int>> Gy = {{-1, 0, 1}, {-2, 0, 2}, {-1, 0, 1}};
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; ++j)
        {
            float Gxred = 0;
            float Gxgreen = 0;
            float Gxblue = 0;
            float Gyred = 0;
            float Gygreen = 0;
            float Gyblue = 0;
            for (int k = -1; k <= 1; ++k)
            {
                for (int l = -1; l <= 1; ++l)
                {
                    int x = i + k;
                    int y = j + l;

                    if (x >= 0 && x < height && y >= 0 && y < width)
                    {
                        Gxred += image[x][y].rgbtRed * Gx[k + 1][l + 1];
                        Gxgreen += image[x][y].rgbtGreen * Gx[k + 1][l + 1];
                        Gxblue += image[x][y].rgbtBlue * Gx[k + 1][l + 1];

                        Gyred += image[x][y].rgbtRed * Gy[k + 1][l + 1];
                        Gygreen += image[x][y].rgbtGreen * Gy[k + 1][l + 1];
                        Gyblue += image[x][y].rgbtBlue * Gy[k + 1][l + 1];
                    }
                }
            }
            int red = round(sqrt(pow(Gxred, 2) + pow(Gyred, 2)));
            int green = round(sqrt(pow(Gxgreen, 2) + pow(Gygreen, 2)));
            int blue = round(sqrt(pow(Gxblue, 2) + pow(Gyblue, 2)));

            temp[i][j].rgbtRed = fmin(round(red), 255);
            temp[i][j].rgbtGreen = fmin(round(green), 255);
            temp[i][j].rgbtBlue = fmin(round(blue), 255);
        }
    }

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            image[i][j] = temp[i][j];
        }
    }
    return;
}
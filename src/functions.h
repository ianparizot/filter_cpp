#pragma once
#include <vector>
#include "bmp.h"
using namespace std;
// Convert image to grayscale
void grayscale(int height, int width, vector<vector<RGBTRIPLE>>& image);

// Reflect image horizontally
void reflect(int height, int width, vector<vector<RGBTRIPLE>>& image);

// Detect edges
void edges(int height, int width, vector<vector<RGBTRIPLE>>& image);

// Blur image
void blur(int height, int width, vector<vector<RGBTRIPLE>>& image);

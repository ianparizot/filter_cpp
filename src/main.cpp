#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>
#include <cstring>

#include "functions.h"

using namespace std;

int main(int argc, char** argv) {
    ofstream output;
    string filters("bger");
    // Check for correct number of arguments
    if (argc != 3 and argc != 4 and argc != 2) {
        cerr << "Usage: filter -(b, g, e and r) inputfile.bmp [outputfile.bmp]" << endl;
        exit(EXIT_FAILURE);
    }
    // Check for valid filter
    string filter = argv[1];
    if (not filter[0] == '-') {
        cerr << "Error 0: Invalid filter" << endl;
        exit(EXIT_FAILURE);
    }
    if (filter[1] == 'v' or filter == "--version") {
        cout << "filter 1.0.1" << endl << "Copyright (c) 2025 Ian Parizot" << endl << "https://github.com/ianparizot/" <<
        "MIT License" << endl;
        exit(EXIT_SUCCESS);
    }
    if (filter.length() < 2 or filters.find(filter[1]) == string::npos) {
        cerr << "ERROR 1: Invalid filter" << endl;
        exit(EXIT_FAILURE);
    }
    // Open the input image and making sure it exists
    ifstream input(argv[2], ios::binary);
    if (not input) {
        cerr << "ERROR 2: Impossible to open input image" << endl;
        exit(EXIT_FAILURE);
    }
    // Creating the output image
    if (argc == 3) {
        output.open("output.bmp", ios::binary);
    } else {
        output.open(argv[3], ios::binary);
    }
    // Check if creation is successful
    if (not output) {
        input.close();
        cerr << "ERROR 3: Could not create an output image" << endl;
        exit(EXIT_FAILURE);
    }
    // Now check the input image to make sure it's a bmp.
    BITMAPFILEHEADER bf;
    input.read(reinterpret_cast<char*>(&bf), sizeof(BITMAPFILEHEADER));
    BITMAPINFOHEADER bi;
    input.read(reinterpret_cast<char*>(&bi), sizeof(BITMAPINFOHEADER));
    if (bf.bfType != 0x4d42 || bf.bfOffBits != 54 || bi.biSize != 40 ||
        bi.biBitCount != 24 || bi.biCompression != 0) {
        input.close();
        output.close();
        cerr << "ERROR 4: Not a good BMP file" << endl;
        exit(EXIT_FAILURE);
    }
    // Now that we know it's a supported file, we are gonna get its dimension to see it
    // as a two dimensional vector
    int height(abs(bi.biHeight));
    int width(bi.biWidth);
    // Create a vector using the informations
    vector<vector<RGBTRIPLE>> image(height, vector<RGBTRIPLE>(width));
    // get padding informations
    int padding((4 - (width * sizeof(RGBTRIPLE)) % 4) % 4);
    // Iterate over infile's scanlines
    for (auto& ligne : image) {
        input.read(reinterpret_cast<char*>(ligne.data()), width * sizeof(RGBTRIPLE));
        input.seekg(padding, ios::cur);
    }
    if (filter == "-g") {
        grayscale(height, width, image);
    } else if (filter == "-r") {
        reflect(height, width, image);
    } else if (filter == "-b") {
        blur(height, width, image);
    } else if (filter == "-e") {
        edges(height, width, image);
    } else {
        cerr << "Internal ERROR" << endl; 
    }
    // Write the output file
    output.write(reinterpret_cast<char*>(&bf), sizeof(BITMAPFILEHEADER));
    output.write(reinterpret_cast<char*>(&bi), sizeof(BITMAPINFOHEADER));
    for (auto& line : image) {
        output.write(reinterpret_cast<char*>(line.data()), line.size() * sizeof(RGBTRIPLE));
        for (int i(0); i < padding; ++i) {
            output.put(static_cast<char>(0x00));
        }
    }
    input.close();
    output.close();
    return 0;
}
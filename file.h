//
// Created by a6c0424fa083 on 15.06.23.
//

#ifndef FILE_H
#define FILE_H

#include <fstream>
#include <sstream>
#include <string>
#include <vector>

#include "Color.h"

std::vector<Color8> readPPMImage(const std::string &filePath, uint64_t &width, uint64_t &height, uint16_t &max_color_value)
{
    std::ifstream file;
    file.open(filePath, std::ios::in | std::ios::binary);

    if (file.is_open())
    {
        //WRITE_LOG("Opened Image File Successfully!");
        std::string temp;
        char        _temp;
        //uint64_t temp_int;
        //uint64_t _width, _height;
        //uint16_t _max_color_value;
        //file >> format >> width >> height >> max_color_value;
        temp.clear();
        std::getline(file, temp);
        assert(temp == "P6");

        temp.clear();
        std::getline(file, temp);
        std::stringstream _width;
        _width << temp;
        _width >> width;
        //WRITE_LOG("Retrieved width of Image: %i", (int)width);

        temp.clear();
        std::getline(file, temp);
        std::stringstream _height;
        _height << temp;
        _height >> height;
        //WRITE_LOG("Retrieved height of Image: %i", (int)height);

        temp.clear();
        std::getline(file, temp);
        std::stringstream _max_color_value;
        _max_color_value << temp;
        _max_color_value >> max_color_value;
        assert(max_color_value < 256);

        //WRITE_LOG("Retrieved max color value of Image: %i", (int)max_color_value);

        // Ignore the '\n'
        //file.ignore(1);

        Color8 blank(255, 255, 255);

        std::vector<Color8> pixels(width * height, blank);
        //WRITE_LOG("Created a vector of 'Color8' called 'pixels' with all pixels being white");
        file.read(reinterpret_cast<char *>(pixels.data()), pixels.size() * sizeof(Color8));
        //WRITE_LOG("Read every pixel from file to 'pixels' vector");

        file.close();
        //WRITE_LOG("Closed Image File");
        return pixels;
    }
    else
    {
        return {};
    }
}

void visualizeRGBImage(const std::vector<Color8> &pixels, uint64_t width, uint64_t height, std::string spaces = "  ")
{
    //WRITE_LOG("Begin Printing 'pixels' vector to console");
    for (uint64_t y = 0; y < height; ++y)
    {
        std::cout << "|" << spaces;

        for (uint64_t x = 0; x < width; ++x)
        {
            const Color8 &pixel = pixels[y * width + x];

            if (pixel.red == 255 && pixel.green == 0 && pixel.blue == 0)
            {
                std::cout << '+' << spaces;
            }
            else if (pixel.red == 0 && pixel.green == 255 && pixel.blue == 0)
            {
                std::cout << '#' << spaces;
            }
            else if (pixel.red == 0 && pixel.green == 0 && pixel.blue == 255)
            {
                std::cout << ':' << spaces;
            }
            else if (pixel.red == 0 && pixel.green == 0 && pixel.blue == 0)
            {
                std::cout << '@' << spaces;
            }
            else
            {
                std::cout << '.' << spaces;
            }
        }
        std::cout << "|";
        std::cout << '\n';
    }
    //WRITE_LOG("Finished Printing 'pixels' vector to console");
}

#endif //FILE_H

#ifndef A_STAR_ALGORITHM_IMAGE_TO_MATRIX_H
#define A_STAR_ALGORITHM_IMAGE_TO_MATRIX_H

///@brief:  basic includes
#include <cstdint>
#include <vector>
#include <iostream>
#include <sstream>
#include <fstream>

///@brief:  this function converts the numbers (0 to 9) from a char to a uint8_t form.
///         should only be used by numbers from 0 to 9
uint8_t char_to_num(char num)
{
    return num - 48;
}

///@brief:  this function read in a .ppm image (Type: P2 or P3) and feds the pixel values in a matrix (2d vector)
void get_matrix_from_image(std::string &image_path, uint64_t &width, uint64_t &height, std::vector<std::vector<uint8_t>> &pixel_matrix)
{
    std::ifstream image;

    pixel_matrix.clear();
    width = 0;
    height = 0;

    uint8_t format;
    uint64_t pixel_number = 0;

    image.open(image_path, std::ios::in | std::ios::binary);

    if (image.is_open())
    {
        std::string line;
        uint64_t line_count = 1;

        std::getline(image, line);
        if (line == "P2")
        {
            format = 2;
        }
        else if (line == "P3")
        {
            format = 3;
        }
        else
        {
            throw std::invalid_argument("Invalid Format");
        }

        std::stringstream conversion;

        line_count++;
        std::getline(image, line);
        conversion << line;
        conversion >> width;

        conversion.clear();

        line_count++;
        std::getline(image, line);
        conversion << line;
        conversion >> height;

        line_count++;
        std::getline(image, line);
        if (line != "7")
        {
            throw std::invalid_argument("Invalid color depth");
        }

        line_count++;
        std::getline(image, line);
        if (!line.empty())
        {
            throw std::invalid_argument("Expected empty line between color depth and pixel values");
        }

        for (uint64_t i = 0; i < height; i++)
        {
            pixel_matrix.emplace_back();
            for (uint64_t j = 0; j < width; j++)
            {
                pixel_matrix.at(i).push_back(0);
            }
        }
        pixel_matrix.emplace_back();
        pixel_matrix.at(height).push_back(0);

        if (format == 2)
        {
            while (!image.eof())
            {
                line_count++;
                std::getline(image, line);
                if (line.empty())
                {
                    line = " ";
                }
                pixel_matrix.at(static_cast<uint64_t>(pixel_number / width)).at(static_cast<uint64_t>(pixel_number % width)) = char_to_num(line.at(0));
                pixel_number++;
            }
        }
        else if (format == 3)
        {
            while (!image.eof())
            {
                line_count++;
                std::getline(image, line);
                if ((line_count - 6) % 3 == 0)
                {
                    if (line.empty())
                    {
                        line = " ";
                    }
                    pixel_matrix.at(static_cast<uint64_t>(pixel_number / width)).at(pixel_number % width) = char_to_num(line.at(0));
                    pixel_number++;
                }
            }
        }
        pixel_matrix.pop_back();
        image.close();
    }
}

///@brief:  these functions convert the numbers stored in the matrix to a char viewable in the terminal
char num_to_viewable_char(uint8_t num)
{
    switch (num)
    {
        case 0:
            return '.';
        case 1:
            return ':';
        case 2:
            return '-';
        case 3:
            return '=';
        case 4:
            return '*';
        case 5:
            return '#';
        case 6:
            return '%';
        case 7:
            return '@';
        default:
            return ' ';
    }
}

char num_to_viewable_char_inverted(uint8_t num)
{
    switch (num)
    {
        case 0:
            return '@';
        case 1:
            return '%';
        case 2:
            return '#';
        case 3:
            return '*';
        case 4:
            return '=';
        case 5:
            return '-';
        case 6:
            return ':';
        case 7:
            return '.';
        default:
            return ' ';
    }
}

///@brief:  this function converts the matrix back to pixels with various brightness based on the input vector
void show_grid_output(uint64_t &width, uint64_t &height, std::vector<std::vector<uint8_t>> &grid, const std::string spacing = " ", bool inverted = false)
{
    std::string output;

    for (uint64_t i = 0; i < height; i++)
    {
        output = "";
        for (uint64_t j = 0; j < width; j++)
        {
            if (!inverted)
                output += num_to_viewable_char(grid.at(i).at(j));
            else
                output += num_to_viewable_char_inverted(grid.at(i).at(j));

            output += spacing;
        }
        std::cout << output << std::endl;
    }
}

#endif //A_STAR_ALGORITHM_IMAGE_TO_MATRIX_H

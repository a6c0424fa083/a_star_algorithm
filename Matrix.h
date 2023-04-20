#ifndef MATRIX_H
#define MATRIX_H

///@brief:  basic includes
#include <cstdint>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>

///@brief:  these values are used for calculating the distance between 2 coordinates
#define diagonalValue 14142
#define orthogonalValue 10000

const uint8_t startNode = 4;
const uint8_t endNode = 2;
const uint8_t obstacleNode = 0;
const uint8_t freeNode = 7;

enum Status
{
    StartNode,
    EndNode,
    Obstacle,
    Free,
    Unknown
};

///@brief:  this function converts the numbers (0 to 9) from a char to a uint8_t form.
///         should only be used by numbers from 0 to 9
uint8_t char_to_num(char num)
{
    return num - 48;
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

///@brief:  this struct contains the basics functionality of a coordinate (x and y coordinate values)
struct Coordinate
{
    Coordinate() = default;

    Coordinate(uint64_t X, uint64_t Y)
        : X(X), Y(Y) {}



    uint64_t X{};
    uint64_t Y{};
};

struct ASCoordinate // A-Star-Coordinate
{
    ASCoordinate() = default;
    ASCoordinate(Coordinate coordinate)
        : coordinate(coordinate) {}


    Coordinate coordinate;
    Coordinate parent_Node;
    uint8_t status = Status::Unknown;
};

///@brief: returns the distance between 2 Coordinated based in the value for orthogonal and diagonal movement
uint64_t distance_between_Coordinates(Coordinate c1, Coordinate c2)
{
    uint64_t horizontal_spacing = (c2.Y - c1.Y) >= 0 ? (c2.Y - c1.Y) : (c1.Y - c2.Y);
    uint64_t vertical_spacing = (c2.X - c1.X) >= 0 ? (c2.X - c1.X) : (c1.X - c2.X);
    return (horizontal_spacing < vertical_spacing) ?
           diagonalValue * horizontal_spacing + (vertical_spacing - horizontal_spacing) * orthogonalValue :
           diagonalValue * vertical_spacing + (horizontal_spacing - vertical_spacing) * orthogonalValue;
}
uint64_t distance_between_Coordinates(ASCoordinate c1, ASCoordinate c2)
{
    uint64_t horizontal_spacing = (c2.coordinate.Y - c1.coordinate.Y) >= 0 ? (c2.coordinate.Y - c1.coordinate.Y) : (c1.coordinate.Y - c2.coordinate.Y);
    uint64_t vertical_spacing = (c2.coordinate.X - c1.coordinate.X) >= 0 ? (c2.coordinate.X - c1.coordinate.X) : (c1.coordinate.X - c2.coordinate.X);
    return (horizontal_spacing < vertical_spacing) ?
           diagonalValue * horizontal_spacing + (vertical_spacing - horizontal_spacing) * orthogonalValue :
           diagonalValue * vertical_spacing + (horizontal_spacing - vertical_spacing) * orthogonalValue;
}

///@brief:  this struct contains basic
class Grid
{
public:
    Grid() = default;

    Grid(std::string file_path)
    {
        std::ifstream image;

        image.open(file_path, std::ios::in | std::ios::binary);

        if (image.is_open())
        {
            std::string line;
            uint64_t line_count = 1;

            std::getline(image, line);
            if (line == "P2")
            {
                image_format = 2;
            }
            else if (line == "P3")
            {
                image_format = 3;
            }
            else
            {
                throw std::invalid_argument("Invalid Format");
            }

            std::stringstream conversion;

            line_count++;
            std::getline(image, line);
            conversion << line;
            conversion >> grid_width;

            conversion.clear();

            line_count++;
            std::getline(image, line);
            conversion << line;
            conversion >> grid_height;

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

            if (image_format == 2)
            {
                while (!image.eof())
                {
                    std::getline(image, line);
                    if (!line.empty())
                    {
                        line_count++;

                        grid.push_back(ASCoordinate(Coordinate((line_count - 6) % grid_width, (uint64_t)(line_count / grid_height))));
                        switch (char_to_num(line[0]))
                        {
                            case obstacleNode:
                                grid.back().status = Status::Obstacle;
                                obstacles.push_back(grid.back());
                                break;
                            case endNode:
                                grid.back().status = Status::EndNode;
                                end_node = grid.back();
                                break;
                            case startNode:
                                grid.back().status = Status::StartNode;
                                start_node = grid.back();
                                break;
                            case freeNode:
                                grid.back().status = Status::Free;
                                break;
                        }
                    }
                }
            }
            else if (image_format == 3)
            {
                while (!image.eof())
                {
                    std::getline(image, line);
                    if ((line_count - 6) % 3 == 0)
                    {
                        if (!line.empty())
                        {
                            line_count++;

                            grid.push_back(ASCoordinate(Coordinate((line_count - 6) % grid_width, (uint64_t)(line_count / grid_height))));
                            switch (char_to_num(line[0]))
                            {
                                case obstacleNode:
                                    grid.back().status = Status::Obstacle;
                                    obstacles.push_back(grid.back());
                                    break;
                                case endNode:
                                    grid.back().status = Status::EndNode;
                                    end_node = grid.back();
                                    break;
                                case startNode:
                                    grid.back().status = Status::StartNode;
                                    start_node = grid.back();
                                    break;
                                case freeNode:
                                    grid.back().status = Status::Free;
                                    break;
                            }
                        }
                    }
                }
            }
            image.close();
        }
        else
        {
            throw std::invalid_argument("Cannot open file");
        }
    }

    ///@brief:  this function converts the matrix back to pixels with various brightness based on the input vector
    void show_grid_output(const std::string spacing = " ", bool inverted = false)
    {
        std::string output;

        for (uint64_t i = 0; i < grid_height; i++)
        {
            output = "";
            for (uint64_t j = 0; j < grid_width; j++)
            {
                if (!inverted)
                    output += num_to_viewable_char(grid.at((i * grid_width) + j).status);
                else
                    output += num_to_viewable_char_inverted(grid.at((i * grid_width) + j).status);

                output += spacing;
            }
            std::cout << output << std::endl;
        }
    }

    void find_shortest_path()
    {

    }

    ///@brief:  get all valid nodes surrounding a given node
    void add_daughter_node_to_open_nodes(ASCoordinate coordinate)
    {
        if (coordinate.coordinate.Y != 0)
            open_nodes.push_back(ASCoordinate(Coordinate(coordinate.coordinate.X, coordinate.coordinate.Y - 1)));

        if (coordinate.coordinate.X != 0)
            open_nodes.push_back(ASCoordinate(Coordinate(coordinate.coordinate.X - 1, coordinate.coordinate.Y)));

        if (coordinate.coordinate.X != grid_width - 1)
            open_nodes.push_back(ASCoordinate(Coordinate(coordinate.coordinate.X + 1, coordinate.coordinate.Y)));

        if (coordinate.coordinate.Y != grid_height - 1)
            open_nodes.push_back(ASCoordinate(Coordinate(coordinate.coordinate.X, coordinate.coordinate.Y + 1)));


        if ((coordinate.coordinate.Y != 0) && (coordinate.coordinate.X != 0))
            open_nodes.push_back(ASCoordinate(Coordinate(coordinate.coordinate.X - 1, coordinate.coordinate.Y - 1)));

        if ((coordinate.coordinate.Y != 0) && (coordinate.coordinate.X != grid_width - 1))
            open_nodes.push_back(ASCoordinate(Coordinate(coordinate.coordinate.X + 1, coordinate.coordinate.Y - 1)));

        if ((coordinate.coordinate.Y != grid_height - 1) && (coordinate.coordinate.X != 0))
            open_nodes.push_back(ASCoordinate(Coordinate(coordinate.coordinate.X - 1, coordinate.coordinate.Y + 1)));

        if ((coordinate.coordinate.Y != grid_height - 1) && (coordinate.coordinate.X != grid_width - 1))
            open_nodes.push_back(ASCoordinate(Coordinate(coordinate.coordinate.X + 1, coordinate.coordinate.Y + 1)));

    }

public:
    std::vector<ASCoordinate> grid;
    std::vector<ASCoordinate> obstacles;
    ASCoordinate start_node;
    ASCoordinate end_node;
    std::vector<ASCoordinate> open_nodes;
    std::vector<ASCoordinate> closed_nodes;
    uint64_t grid_height;
    uint64_t grid_width;
    uint8_t image_format;

};

#endif //MATRIX_H

#ifndef MATRIX_H
#define MATRIX_H

///@brief:  basic includes
#include <cstdint>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <cmath>
#include <cstdlib>

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
    Path,
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

    Coordinate(uint64_t row, uint64_t column)
        : row(row), column(column) {}



    uint64_t column{};
    uint64_t row{};
};

struct ASCoordinate // A-Star-Coordinate
{
    ASCoordinate() = default;
    ASCoordinate(Coordinate coordinate)
        : coordinate(coordinate) {}


    Coordinate coordinate;
    Coordinate parent_Node;
    uint8_t status = Status::Unknown;
    uint64_t distance_to_start_node;
    uint64_t distance_to_end_node;
    bool on_closed_list = false;
    bool on_open_list = false;
};

uint64_t difference(int64_t value)
{
    return value < 0 ? -value : value;
}

///@brief: returns the distance between 2 Coordinated based in the value for orthogonal and diagonal movement
uint64_t distance_between_Coordinates(Coordinate c1, Coordinate c2)
{
    uint64_t horizontal_spacing = difference(c2.column - c1.column);
    uint64_t vertical_spacing = difference(c2.row - c1.row);
    return (horizontal_spacing < vertical_spacing) ?
           diagonalValue * horizontal_spacing + (vertical_spacing - horizontal_spacing) * orthogonalValue :
           diagonalValue * vertical_spacing + (horizontal_spacing - vertical_spacing) * orthogonalValue;
}
uint64_t distance_between_Coordinates(ASCoordinate c1, ASCoordinate c2)
{
    uint64_t horizontal_spacing = difference(c2.coordinate.column - c1.coordinate.column);
    uint64_t vertical_spacing = difference(c2.coordinate.row - c1.coordinate.row);
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

                        grid.push_back(ASCoordinate(Coordinate( (uint64_t)((line_count - 6) / grid_width), (line_count - 6) % grid_width)));
                        switch (char_to_num(line[0]))
                        {
                            case obstacleNode:
                                grid.back().status = Status::Obstacle;
                                obstacles.push_back(&grid.back());
                                break;
                            case endNode:
                                grid.back().status = Status::EndNode;
                                end_node = &grid.back();
                                break;
                            case startNode:
                                grid.back().status = Status::StartNode;
                                start_node = &grid.back();
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
                                    //obstacles.push_back(&grid.back());
                                    break;
                                case endNode:
                                    grid.back().status = Status::EndNode;
                                    //end_node = &grid.back();
                                    break;
                                case startNode:
                                    grid.back().status = Status::StartNode;
                                    //start_node = &grid.back();
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

            for (ASCoordinate &node : grid)
            {
                switch (node.status) {
                    case Status::Obstacle:
                        obstacles.push_back(&node);
                        break;
                    case Status::StartNode:
                        start_node = &node;
                        start_node->on_closed_list = true;
                        closed_nodes.push_back(start_node);
                        break;
                    case Status::EndNode:
                        end_node = &node;
                        break;
                    default:
                        break;
                }
            }
            /*
            start_node->distance_to_start_node = 0;
            end_node->distance_to_end_node = 0;
            start_node->distance_to_end_node = distance_between_Coordinates(*start_node, *end_node);
            end_node->distance_to_start_node = distance_between_Coordinates(*start_node, *end_node);
            start_node->on_closed_list = true;
            closed_nodes.push_back(start_node);*/
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

    ///@brief:  get all valid nodes surrounding a given node
    void add_daughter_nodes_to_list(ASCoordinate coordinate, std::vector<Coordinate> &list)
    {
        //std::vector<Coordinate> coordinates;

        if (coordinate.coordinate.column > 0)
            list.push_back(Coordinate(coordinate.coordinate.row, coordinate.coordinate.column - 1));

        if (coordinate.coordinate.row > 0)
            list.push_back(Coordinate(coordinate.coordinate.row - 1, coordinate.coordinate.column));

        if (coordinate.coordinate.row < grid_width - 1)
            list.push_back(Coordinate(coordinate.coordinate.row + 1, coordinate.coordinate.column));

        if (coordinate.coordinate.column < grid_height - 1)
            list.push_back(Coordinate(coordinate.coordinate.row, coordinate.coordinate.column + 1));


        if ((coordinate.coordinate.column > 0) && (coordinate.coordinate.row > 0))
            list.push_back(Coordinate(coordinate.coordinate.row - 1, coordinate.coordinate.column - 1));

        if ((coordinate.coordinate.column > 0) && (coordinate.coordinate.row < grid_width - 1))
            list.push_back(Coordinate(coordinate.coordinate.row + 1, coordinate.coordinate.column - 1));

        if ((coordinate.coordinate.column < grid_height - 1) && (coordinate.coordinate.row > 0))
            list.push_back(Coordinate(coordinate.coordinate.row - 1, coordinate.coordinate.column + 1));

        if ((coordinate.coordinate.column < grid_height - 1) && (coordinate.coordinate.row < grid_width - 1))
            list.push_back(Coordinate(coordinate.coordinate.row + 1, coordinate.coordinate.column + 1));
    }

    ///@brief:  get all valid nodes surrounding a given node
    void add_daughter_nodes_to_list(ASCoordinate coordinate, std::vector<ASCoordinate *> &list)
    {
        //std::vector<Coordinate> coordinates;

        if (coordinate.coordinate.column > 0)
            list.push_back(&grid.at(grid_width * coordinate.coordinate.row + coordinate.coordinate.column - 1));

        if (coordinate.coordinate.row > 0)
            list.push_back(&grid.at(grid_width * coordinate.coordinate.row - 1 + coordinate.coordinate.column));

        if (coordinate.coordinate.row < grid_width - 1)
            list.push_back(&grid.at(grid_width * coordinate.coordinate.row + 1 + coordinate.coordinate.column));

        if (coordinate.coordinate.column < grid_height - 1)
            list.push_back(&grid.at(grid_width * coordinate.coordinate.row + coordinate.coordinate.column + 1));


        if ((coordinate.coordinate.column > 0) && (coordinate.coordinate.row > 0))
            list.push_back(&grid.at(grid_width * coordinate.coordinate.row - 1 + coordinate.coordinate.column - 1));

        if ((coordinate.coordinate.column > 0) && (coordinate.coordinate.row < grid_width - 1))
            list.push_back(&grid.at(grid_width * coordinate.coordinate.row + 1 + coordinate.coordinate.column - 1));

        if ((coordinate.coordinate.column < grid_height - 1) && (coordinate.coordinate.row > 0))
            list.push_back(&grid.at(grid_width * coordinate.coordinate.row - 1 + coordinate.coordinate.column + 1));

        if ((coordinate.coordinate.column < grid_height - 1) && (coordinate.coordinate.row < grid_width - 1))
            list.push_back(&grid.at(grid_width * coordinate.coordinate.row + 1 + coordinate.coordinate.column + 1));
    }

    void calculate_distances_to_start_node_from_open_nodes_list()
    {
        //std::cout << "Start calculating distances" << std::endl;
        std::vector<uint64_t> distances;
        std::vector<Coordinate> daughter_nodes;
        uint64_t index_of_smallest_element;
        for (ASCoordinate *node : open_nodes)
        {
            //std::cout << "Entered node" << std::endl;
            daughter_nodes.clear();
            distances.clear();

            add_daughter_nodes_to_list(*node, daughter_nodes);

            for (Coordinate &daughter_node : daughter_nodes)
            {
                if (grid.at(grid_width * daughter_node.row + daughter_node.column).on_closed_list) // daughter_node.on_closed_list // pointer for references
                    distances.push_back(grid.at(grid_width * daughter_node.row + daughter_node.column).distance_to_start_node + distance_between_Coordinates(daughter_node, node->coordinate));
            }
            index_of_smallest_element = std::distance(std::begin(distances), std::min_element(std::begin(distances), std::end(distances)));
            node->parent_Node = Coordinate(grid.at(grid_width * daughter_nodes.at(index_of_smallest_element).row + daughter_nodes.at(index_of_smallest_element).column).coordinate);
            node->distance_to_start_node = distances.at(index_of_smallest_element);
            node->distance_to_end_node = distance_between_Coordinates(*node, *end_node);
        }
        //std::cout << "Calculated distances" << std::endl;
    }

    void find_shortest_path()
    {
        found_path = false;
        path_nodes.clear();
        std::vector<uint64_t> distances;
        uint64_t index_of_smallest_element;

        path_nodes.push_back(start_node);

        add_daughter_nodes_to_list(*start_node, open_nodes);

        for(uint64_t counter = 0;!found_path; counter++)
        {
            std::cout << "Round: " << counter << std::endl;
            calculate_distances_to_start_node_from_open_nodes_list();
            distances.clear();

            for (ASCoordinate *node : open_nodes)
            {
                distances.push_back(node->distance_to_start_node + node->distance_to_end_node);
            }
            index_of_smallest_element = std::distance(std::begin(distances), std::min_element(std::begin(distances), std::end(distances)));
            /// Add extended feature in future! Situation: many elements with the same minimal value
            open_nodes.at(index_of_smallest_element)->on_closed_list = true;
            path_nodes.push_back(open_nodes.at(index_of_smallest_element));
            if (path_nodes.back()->distance_to_end_node < 2 * orthogonalValue)
                found_path = true;
        }
    }

public:
    std::vector<ASCoordinate> grid;
    std::vector<ASCoordinate *> obstacles;
    ASCoordinate *start_node;
    ASCoordinate *end_node;
    std::vector<ASCoordinate *> open_nodes;
    std::vector<ASCoordinate *> closed_nodes;
    uint64_t grid_height;
    uint64_t grid_width;
    uint8_t image_format;
    bool found_path;
    std::vector<ASCoordinate *> path_nodes;

};

#endif //MATRIX_H

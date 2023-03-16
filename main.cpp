#include <iostream>
#include <vector>
#include <fstream>
#include <cstdint>
#include <sstream>

#include "Matrix.h"
#include "Image_to_Matrix.h"


int main(int argc, char** argv)
{
    uint64_t width;
    uint64_t height;
    Grid image_grid;
    std::vector<std::vector<uint8_t>> grid_vector;
    std::string path = argv[0];
    std::string image_name = "test.ppm";
    Sign_Handler signHandler(4, 3, 0, 2, 7);
    while (path.at(path.length() - 1) != '/')
    {
        path.pop_back();
    }
    path += image_name;

    get_matrix_from_image(path, width, height, grid_vector);

    show_grid_output(width, height, grid_vector, "  ", true);

    std::cout << std::endl;

    image_grid.init(grid_vector, signHandler);
    image_grid.find_shortest_path();

    std::cout << image_grid.obstacles.size() << " Obstacles detected!" << std::endl;

    for (uint64_t i = 0; i < image_grid.obstacles.size(); i++)
    {
        std::cout << "Obstacle: [" << image_grid.obstacles.at(i).X << ", " << image_grid.obstacles.at(i).Y << "]" << std::endl;
    }
    std::cout << std::endl;
    std::cout << "Start Node: [" << image_grid.start_node.X << ", " << image_grid.start_node.Y << "]" << std::endl;
    std::cout << "End Node: [" << image_grid.end_node.X << ", " << image_grid.end_node.Y << "]" << std::endl;

    char inputX;
    char inputY;
    char input;

    std::vector<Coordinate> daughter_coordinates;

    std::cout << std::endl;

    while (input != 'e')
    {
        std::cout << "Enter X: ";
        std::cin >> inputX;
        std::cout << "Enter Y: ";
        std::cin >> inputY;

        daughter_coordinates = image_grid.get_valid_daughter_nodes_coordinates(A_Coordinate((uint8_t)inputX - 48, (uint8_t)inputY - 48, 0, 0, 0, 0));

        std::cout << "All the Daughter Coordinates: " << std::endl;
        for (uint64_t i = 0; i < daughter_coordinates.size(); i++)
        {
            std::cout << "Coordinate: [" << daughter_coordinates.at(i).X << ", " << daughter_coordinates.at(i).Y << "]" << std::endl;
        }

        std::cout << "Exit? Press 'e': ";
        std::cin >> input;
        std::cout << std::endl;
    }

    return 0;
}

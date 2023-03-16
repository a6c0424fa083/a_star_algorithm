#ifndef A_STAR_ALGORITHM_MATRIX_H
#define A_STAR_ALGORITHM_MATRIX_H

///@brief:  basic includes
#include <cstdint>
#include <vector>
#include <iostream>

///@brief:  these values are used for calculating the distance between 2 coordinates
#define diagonalValue 14142
#define orthogonalValue 10000

///@brief:  this struct contains the basics functionality of a coordinate (x and y coordinate values)
struct Coordinate
{
    Coordinate() = default;

    Coordinate(uint64_t X, uint64_t Y)
            : X(X), Y(Y) {}

    [[nodiscard("Class reference should be handled properly")]] Coordinate get_Coordinate() const
    {
        return *this;
    }
    void set_Coordinate(Coordinate *coordinate)
    {
        X = coordinate->X;
        Y = coordinate->Y;
    }
    [[nodiscard]] bool is_initialized() const
    {
        if (X && Y)
            return true;
        else
            return false;
    }

    uint64_t X{};
    uint64_t Y{};
};

///@brief:  this struct represents a coordinate used in 'A* Algorithm'
///         it contains additional variables such as the parent coordinate as well as the distance to certain coordinates
struct A_Coordinate
{
    A_Coordinate() = delete;

    A_Coordinate(Coordinate coordinate, Coordinate start_coordinate, Coordinate end_coordinate)
            : coordinate(coordinate), start_coordinate(start_coordinate), end_coordinate(end_coordinate) {}

    A_Coordinate(uint64_t X, uint64_t Y, uint64_t start_X, uint64_t start_Y, uint64_t end_X, uint64_t end_Y)
    {
        coordinate.X = X;
        coordinate.Y = Y;
        start_coordinate.X = start_X;
        start_coordinate.Y = start_Y;
        end_coordinate.X = start_X;
        end_coordinate.Y = start_Y;
    }

    Coordinate coordinate;
    Coordinate start_coordinate;
    Coordinate end_coordinate;
    Coordinate parent_Coordinate;
    uint8_t status;
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

///@brief:  the conversion from a number to e.g. a start- or end-node is stored here
struct Sign_Handler
{
    Sign_Handler() = delete;

    Sign_Handler(uint8_t start_node, uint8_t end_node, uint8_t obstacle_node, uint8_t path_node, uint8_t clear_node)
            : start_node(start_node), end_node(end_node), obstacle_node(obstacle_node), path_node(path_node), clear_node(clear_node) {}

    uint8_t start_node;
    uint8_t end_node;
    uint8_t obstacle_node;
    uint8_t path_node;
    uint8_t clear_node;
};

///@brief:  this struct contains basic
class Grid
{
public:
    Grid()
            : sign_Handler(Sign_Handler(1, 3, 7, 2, 0)) {}

    Grid(std::vector<std::vector<uint8_t>> grid, Sign_Handler sign_Handler)
            : grid(grid), sign_Handler(sign_Handler), grid_height(grid.size()), grid_width(grid.at(0).size()) {}

    void init(std::vector<std::vector<uint8_t>> i_grid, Sign_Handler i_sign_Handler = Sign_Handler(1, 3, 7, 2, 0))
    {
        grid = i_grid;
        sign_Handler = i_sign_Handler;
        grid_height = grid.size();
        grid_width = grid.at(0).size();
    }

    void find_shortest_path()
    {
        set_internal_information_from_grid();
        convert_from_uint8t_to_ACoordinate();
    }

private:
    ///@brief:  this function sets the internal values that are necessary for further computation
    ///         e.g. start- or end-node as well as all the obstacle-nodes
    void set_internal_information_from_grid()
    {
        for (uint64_t i = 0; i < grid.size(); i++)
        {
            for (uint64_t j = 0; j < grid.at(0).size(); j++)
            {
                if (grid.at(i).at(j) == sign_Handler.start_node)
                    start_node = Coordinate(i, j);

                else if (grid.at(i).at(j) == sign_Handler.end_node)
                    end_node = Coordinate(i, j);

                else if (grid.at(i).at(j) == sign_Handler.obstacle_node)
                    obstacles.push_back(Coordinate(i, j));
            }
        }
    }
    ///@brief:  create a 2-dimensional grid of 'A_Coordinate' in order to archive more complex operations
    void convert_from_uint8t_to_ACoordinate()
    {
        std::vector<A_Coordinate> local_coordinate_line;
        for (uint64_t i = 0; i < grid.size(); i++)
        {
            for (uint64_t j = 0; j < grid.size(); j++)
            {
                local_coordinate_line.push_back(A_Coordinate(i, j, start_node.X, start_node.Y, end_node.X, end_node.Y));
            }
            coordinate_grid.push_back(local_coordinate_line);
            local_coordinate_line.clear();
        }

        for (uint64_t i = 0; i < grid.size(); i++)
        {
            for (uint64_t j = 0; j < grid.size(); j++)
            {
                if (grid.at(i).at(j) == sign_Handler.start_node)
                    coordinate_grid.at(i).at(j).status = sign_Handler.start_node;

                else if (grid.at(i).at(j) == sign_Handler.end_node)
                    coordinate_grid.at(i).at(j).status = sign_Handler.end_node;

                else if (grid.at(i).at(j) == sign_Handler.obstacle_node)
                    coordinate_grid.at(i).at(j).status = sign_Handler.obstacle_node;

                else
                    coordinate_grid.at(i).at(j).status = sign_Handler.clear_node;
            }
        }
    }

public:
    ///@brief:  get all valid nodes surrounding a given node
    std::vector<Coordinate> get_valid_daughter_nodes_coordinates(A_Coordinate coordinate)
    {
        bool left_side_touching = false;
        bool right_side_touching = false;
        bool upper_side_touching = false;
        bool lower_side_touching = false;
        std::vector<Coordinate> coordinates;

        if (coordinate.coordinate.X == 0)
            left_side_touching = true;

        if (coordinate.coordinate.X == grid_width - 1)
            right_side_touching = true;

        if (coordinate.coordinate.Y == 0)
            upper_side_touching = true;

        if (coordinate.coordinate.Y == grid_height - 1)
            lower_side_touching = true;

        if (!upper_side_touching)
            coordinates.push_back(Coordinate(coordinate.coordinate.X, coordinate.coordinate.Y - 1));

        if (!left_side_touching)
            coordinates.push_back(Coordinate(coordinate.coordinate.X - 1, coordinate.coordinate.Y));

        if (!right_side_touching)
            coordinates.push_back(Coordinate(coordinate.coordinate.X + 1, coordinate.coordinate.Y));

        if (!lower_side_touching)
            coordinates.push_back(Coordinate(coordinate.coordinate.X, coordinate.coordinate.Y + 1));


        if (!upper_side_touching && !left_side_touching)
            coordinates.push_back(Coordinate(coordinate.coordinate.X - 1, coordinate.coordinate.Y - 1));

        if (!upper_side_touching && !right_side_touching)
            coordinates.push_back(Coordinate(coordinate.coordinate.X + 1, coordinate.coordinate.Y - 1));

        if (!lower_side_touching && !left_side_touching)
            coordinates.push_back(Coordinate(coordinate.coordinate.X - 1, coordinate.coordinate.Y + 1));

        if (!lower_side_touching && !right_side_touching)
            coordinates.push_back(Coordinate(coordinate.coordinate.X + 1, coordinate.coordinate.Y + 1));

        return coordinates;
    }

public:
    std::vector<std::vector<uint8_t>> grid;
    std::vector<std::vector<A_Coordinate>> coordinate_grid;
    Sign_Handler sign_Handler;
    std::vector<Coordinate> obstacles;
    std::vector<A_Coordinate> closed_nodes;
    std::vector<A_Coordinate> open_nodes;
    Coordinate start_node;
    Coordinate end_node;
    uint64_t grid_height;
    uint64_t grid_width;

};

#endif //A_STAR_ALGORITHM_MATRIX_H

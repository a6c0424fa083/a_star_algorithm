//
// Created by a6c0424fa083 on 15.06.23.
//

#ifndef GRID_H
#define GRID_H

#include <fstream>
#include <vector>

#include "Color.h"
#include "Node.h"

#define diagonalValue   14142
#define orthogonalValue 10000

class A_Star_Grid
{
public:
    A_Star_Grid() = delete;

    A_Star_Grid(std::vector<Color8> grid, uint64_t width, uint64_t height) : width(width), height(height)
    {
        //WRITE_LOG("Called 'A_Star_Grid' initializer (W: %i, H: %i)", width, height);
        for (uint64_t row = 0; row < height; row++)
        {
            for (uint64_t column = 0; column < width; column++)
            {
                const Color8 color = grid.at(row * width + column);

                Node temp_node(Coordinate(row, column), Status::UNKNOWN);

                if (color.red == 255 && color.green == 0 && color.blue == 0)
                {
                    temp_node.status = Status::START;
                    //WRITE_LOG("Checking Pixel Status (Color) at position (Row: %i, Column: %i): Status: START", row, column);
                }
                else if (color.red == 0 && color.green == 255 && color.blue == 0)
                {
                    temp_node.status = Status::END;
                    //WRITE_LOG("Checking Pixel Status (Color) at position (Row: %i, Column: %i): Status: END", row, column);
                }
                else if (color.red == 0 && color.green == 0 && color.blue == 0)
                {
                    temp_node.status = Status::OBSTACLE;
                    //WRITE_LOG("Checking Pixel Status (Color) at position (Row: %i, Column: %i): Status: OBSTACLE", row, column);
                }
                else
                {
                    temp_node.status = Status::FREE;
                    //WRITE_LOG("Checking Pixel Status (Color) at position (Row: %i, Column: %i): Status: FREE", row, column);
                }
                this->grid.push_back(temp_node);
            }
        }

        for (Node &node : this->grid)
        {
            if (node.status == Status::START)
                start_node = &node;
            if (node.status == Status::END)
                end_node = &node;
        }
    }

    void view_grid(std::string spaces = " ")
    {

        for (uint64_t row = 0; row < height; row++)
        {
            std::cout << "|" << spaces;

            for (uint64_t column = 0; column < width; column++)
            {
                const Node &node = grid.at(row * width + column);

                if (node.status == Status::FREE)
                    std::cout << "." << spaces;
                else if (node.status == Status::START)
                    std::cout << "+" << spaces;
                else if (node.status == Status::END)
                    std::cout << "#" << spaces;
                else if (node.status == Status::PATH)
                    std::cout << ":" << spaces;
                else if (node.status == Status::CLOSED)
                    std::cout << "X" << spaces;
                else if (node.status == Status::OPEN)
                    std::cout << "?" << spaces;
                else
                    std::cout << "@" << spaces;
            }
            std::cout << "|";
            std::cout << '\n';
        }
    }

    void calculate_shortest_path()
    {
        closed_nodes.clear();
        open_nodes.clear();
        path_found                    = false;
        start_node->distance_to_start = 0;
        end_node->distance_to_end     = 0;
        start_node->distance_to_end   = distance_between_Coordinates(start_node->coordinate, end_node->coordinate);
        end_node->distance_to_start   = distance_between_Coordinates(start_node->coordinate, end_node->coordinate);
        start_node->parent_node       = start_node;
        closed_nodes.push_back(start_node);

        std::vector<Node *> neighbor_nodes;
        uint64_t            lowest_distance;
        uint64_t            lowest_distance_end;
        Node               *lowest_distance_node;

        //bool first_round = true;
        uint64_t counter = 1;

        while (!path_found)
        {
            WRITE_LOG("ROUND: %i", counter);
            /*if (first_round)
            {
                get_free_neighbor_nodes(*start_node, open_nodes);
            }
            first_round = false;*/
            for (Node *node : closed_nodes)
            {
                get_free_neighbor_nodes(*node, open_nodes);
            }
            std::cout << std::endl;
            std::cout << "ROUND: " << counter << std::endl;
            view_grid("");
            std::cout << std::endl;
            system("clear");

            counter++;
            //WRITE_LOG("CHECKPOINT 1");
            for (Node *node : open_nodes)
            {
                lowest_distance     = -1;
                lowest_distance_end = -1;
                neighbor_nodes.clear();
                get_all_neighbor_nodes(*node, neighbor_nodes);

                //WRITE_LOG("CHECKPOINT 2");
                for (Node *node_ : neighbor_nodes)
                {
                    if (node_->distance_to_start < lowest_distance && (node_->status == Status::CLOSED || node_->status == Status::START))
                    {
                        lowest_distance   = node_->distance_to_start;
                        node->parent_node = node_;
                    }
                }
                node->distance_to_end = distance_between_Coordinates(node->coordinate, end_node->coordinate);
                node->distance_to_start =
                    node->parent_node->distance_to_start + distance_between_Coordinates(node->coordinate, node->parent_node->coordinate);
                WRITE_LOG("Node (Row: %i, Column: %i, Dist. to Start: %i) has Parent Node (Row: %i, Column: %i, Dist. to Start: %i)",
                          node->coordinate.row,
                          node->coordinate.column,
                          node->distance_to_start,
                          node->parent_node->coordinate.row,
                          node->parent_node->coordinate.column,
                          node->parent_node->distance_to_start);
            }

            lowest_distance     = -1;
            lowest_distance_end = -1;

            //WRITE_LOG("CHECKPOINT 3");
            for (Node *node : open_nodes)
            {
                if (node->distance_to_start + distance_between_Coordinates(node->coordinate, end_node->coordinate) < lowest_distance)
                {
                    lowest_distance_end  = distance_between_Coordinates(node->coordinate, end_node->coordinate);
                    lowest_distance      = node->distance_to_start + distance_between_Coordinates(node->coordinate, end_node->coordinate);
                    lowest_distance_node = node;
                }
                if (node->distance_to_start + distance_between_Coordinates(node->coordinate, end_node->coordinate) == lowest_distance)
                {
                    if (distance_between_Coordinates(node->coordinate, end_node->coordinate) < lowest_distance_end)
                    {
                        lowest_distance_end  = distance_between_Coordinates(node->coordinate, end_node->coordinate);
                        lowest_distance      = node->distance_to_start + distance_between_Coordinates(node->coordinate, end_node->coordinate);
                        lowest_distance_node = node;
                    }
                }
            }

            //WRITE_LOG("CHECKPOINT 4");
            for (uint64_t i = 0; i < open_nodes.size(); i++)
            {
                if (distance_between_Coordinates(open_nodes.at(i)->coordinate, end_node->coordinate) < 20000)
                {
                    path_found            = true;
                    end_node->parent_node = open_nodes.at(i);
                }

                if (open_nodes.at(i)->coordinate.row == lowest_distance_node->coordinate.row &&
                    open_nodes.at(i)->coordinate.column == lowest_distance_node->coordinate.column)
                {
                    open_nodes.erase(open_nodes.begin() + static_cast<long>(i));
                }
            }
            closed_nodes.push_back(lowest_distance_node);
            lowest_distance_node->status = Status::CLOSED;
            WRITE_LOG("Added Node (Row: %i, Column: %i) to Closed List",
                      lowest_distance_node->coordinate.row,
                      lowest_distance_node->coordinate.column);

            //path_found = true;
        }

        // clearing image
        for (Node &node : grid)
        {
            if (node.status == Status::CLOSED || node.status == Status::OPEN)
                node.status = Status::FREE;
        }

        Node *trace_back_node = end_node->parent_node;

        while (trace_back_node->status != Status::START)
        {
            trace_back_node->status = PATH;
            trace_back_node         = trace_back_node->parent_node;

            // visual output
            std::cout << std::endl;
            std::cout << "TRACEBACK" << std::endl;
            view_grid("");
            std::cout << std::endl;
            system("clear");
        }
    }

    void write_grid(std::string file_path)
    {
        std::ofstream file;
        file.open(file_path, std::ios::out);
        if (file.is_open())
        {
            file << "P6" << std::endl << width << std::endl << height << std::endl << "255" << std::endl;

            for (Node &node : grid)
            {
                if (node.status == FREE)
                    file << static_cast<char>(255) << static_cast<char>(255) << static_cast<char>(255);
                if (node.status == OBSTACLE)
                    file << static_cast<char>(0) << static_cast<char>(0) << static_cast<char>(0);
                if (node.status == PATH)
                    file << static_cast<char>(0) << static_cast<char>(0) << static_cast<char>(255);
                if (node.status == START)
                    file << static_cast<char>(255) << static_cast<char>(0) << static_cast<char>(0);
                if (node.status == END)
                    file << static_cast<char>(0) << static_cast<char>(255) << static_cast<char>(0);
            }
            file.close();
        }
    }

private:
    void get_free_neighbor_nodes(Node &node, std::vector<Node *> &ret_nodes)
    {
        std::vector<Node *> nodes;
        const uint64_t      ROW    = node.coordinate.row;
        const uint64_t      COLUMN = node.coordinate.column;

        const bool row_is_not_min    = ROW > 0;
        const bool row_is_not_max    = ROW < height - 1;
        const bool column_is_not_min = COLUMN > 0;
        const bool column_is_not_max = COLUMN < (width - 1);

        if (row_is_not_min)
            nodes.push_back(&grid.at((ROW - 1) * width + COLUMN));
        if (row_is_not_max)
            nodes.push_back(&grid.at((ROW + 1) * width + COLUMN));
        if (column_is_not_min)
            nodes.push_back(&grid.at(ROW * width + (COLUMN - 1)));
        if (column_is_not_max)
            nodes.push_back(&grid.at(ROW * width + (COLUMN + 1)));

        if (row_is_not_min && column_is_not_min)
            nodes.push_back(&grid.at((ROW - 1) * width + (COLUMN - 1)));
        if (row_is_not_min && column_is_not_max)
            nodes.push_back(&grid.at((ROW - 1) * width + (COLUMN + 1)));
        if (row_is_not_max && column_is_not_min)
            nodes.push_back(&grid.at((ROW + 1) * width + (COLUMN - 1)));
        if (row_is_not_max && column_is_not_max)
            nodes.push_back(&grid.at((ROW + 1) * width + (COLUMN + 1)));

        //WRITE_LOG("Size: %i", nodes.size());

        for (Node *node_ : nodes)
        {
            //WRITE_LOG("Erased node: (Row: %i, Column: %i)", ROW, COLUMN);
            if (node_->status == Status::FREE)
            {
                WRITE_LOG("Added Neighbor Node (Row: %i, Column: %i) to Open List", node_->coordinate.row, node_->coordinate.column);
                node_->status = Status::OPEN;
                ret_nodes.push_back(node_);
            }
            if (node_->status == Status::END)
            {
                // algorithm finished
                path_found = true;
            }
        }
    }
    void get_all_neighbor_nodes(Node &node, std::vector<Node *> &ret_nodes)
    {
        std::vector<Node *> nodes;
        const uint64_t      ROW    = node.coordinate.row;
        const uint64_t      COLUMN = node.coordinate.column;

        const bool row_is_not_min    = ROW > 0;
        const bool row_is_not_max    = ROW < height - 1;
        const bool column_is_not_min = COLUMN > 0;
        const bool column_is_not_max = COLUMN < (width - 1);

        if (row_is_not_min)
            nodes.push_back(&grid.at((ROW - 1) * width + COLUMN));
        if (row_is_not_max)
            nodes.push_back(&grid.at((ROW + 1) * width + COLUMN));
        if (column_is_not_min)
            nodes.push_back(&grid.at(ROW * width + (COLUMN - 1)));
        if (column_is_not_max)
            nodes.push_back(&grid.at(ROW * width + (COLUMN + 1)));

        if (row_is_not_min && column_is_not_min)
            nodes.push_back(&grid.at((ROW - 1) * width + (COLUMN - 1)));
        if (row_is_not_min && column_is_not_max)
            nodes.push_back(&grid.at((ROW - 1) * width + (COLUMN + 1)));
        if (row_is_not_max && column_is_not_min)
            nodes.push_back(&grid.at((ROW + 1) * width + (COLUMN - 1)));
        if (row_is_not_max && column_is_not_max)
            nodes.push_back(&grid.at((ROW + 1) * width + (COLUMN + 1)));

        //WRITE_LOG("Size: %i", nodes.size());

        for (Node *node_ : nodes)
        {
            //WRITE_LOG("Erased node: (Row: %i, Column: %i)", ROW, COLUMN);
            if (node_->status != Status::OBSTACLE && node_->status != Status::UNKNOWN)
            {
                //WRITE_LOG("Pushed node: (Row: %i, Column: %i)", node_->coordinate.row, node_->coordinate.column);
                ret_nodes.push_back(node_);
            }
        }
    }

    static uint64_t difference(uint64_t value)
    {
        return static_cast<int64_t>(value) < 0 ? (-static_cast<int64_t>(value)) : static_cast<int64_t>(value);
    }

    ///@brief: returns the distance between 2 Coordinated based in the value for orthogonal and diagonal movement
    static uint64_t distance_between_Coordinates(Coordinate c1, Coordinate c2)
    {
        uint64_t horizontal_spacing = difference(c2.column - c1.column);
        uint64_t vertical_spacing   = difference(c2.row - c1.row);
        return (horizontal_spacing < vertical_spacing)
                   ? diagonalValue * horizontal_spacing + (vertical_spacing - horizontal_spacing) * orthogonalValue
                   : diagonalValue * vertical_spacing + (horizontal_spacing - vertical_spacing) * orthogonalValue;
    }

private:
    std::vector<Node>   grid;
    std::vector<Node>   grid_solved;
    std::vector<Node *> open_nodes;
    std::vector<Node *> closed_nodes;
    Node               *start_node;
    Node               *end_node;
    bool                path_found = false;
    uint64_t            width;
    uint64_t            height;
};


#endif //GRID_H

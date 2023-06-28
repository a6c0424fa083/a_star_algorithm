//
// Created by a6c0424fa083 on 15.06.23.
//

#ifndef NODE_H
#define NODE_H

#include "Coordinate.h"
#include "defines.h"

enum Status
{
    FREE,
    OBSTACLE,
    START,
    END,
    PATH,
    OPEN,
    CLOSED,
    UNKNOWN
};

struct Node
{
    Node() = delete;

    Node(Coordinate coordinate, Status status = UNKNOWN) : coordinate(coordinate), status(status) {}

    Coordinate coordinate;
    Status     status;
    uint64_t   distance_to_start, distance_to_end = -1;
    Node      *parent_node;
};


#endif //NODE_H

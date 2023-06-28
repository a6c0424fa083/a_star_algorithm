//
// Created by a6c0424fa083 on 15.06.23.
//

#ifndef COORDINATE_H
#define COORDINATE_H

#include "defines.h"
#include <cstdint>

struct Coordinate
{
    Coordinate() = delete;

    Coordinate(uint64_t row, uint64_t column) : row(row), column(column) {}

    uint64_t row;
    uint64_t column;
};

#endif //COORDINATE_H

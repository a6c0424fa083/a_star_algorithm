//
// Created by a6c0424fa083 on 15.06.23.
//

#ifndef COLOR_H
#define COLOR_H

#include "defines.h"
#include <cstdint>

struct Color8
{
    Color8() = delete;
    Color8(uint8_t red, uint8_t green, uint8_t blue) : red(red), green(green), blue(blue) {}

    uint8_t red;
    uint8_t green;
    uint8_t blue;
};


#endif //COLOR_H

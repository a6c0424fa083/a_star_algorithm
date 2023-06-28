//
// Created by a6c0424fa083 on 15.06.23.
//

#include <iostream>
#include <string>

// Comment to disable logging (the program is much faster than)
// must be defined before including 'defines.h' !

#define LOG_PROCESS


// IMPORTANT //
//
// Past this on the very beginning of the main function:
//
// LOG::WRITE_LOG_INIT();
// atexit(LOG::EXIT_FUNCTION);

// 'defines.h' must be the first own include
#include "defines.h"

#include "Grid.h"
#include "file.h"


int main(int argc, char **argv)
{

    WRITE_LOG_INIT();
    atexit(EXIT_FUNCTION);

    uint64_t width;
    uint64_t height;
    uint16_t max_color_value;

    std::vector<Color8> pixels = readPPMImage("./test.ppm", width, height, max_color_value);
    A_Star_Grid         grid(pixels, width, height);

    std::cout << std::endl;
    std::cout << std::endl;

    grid.calculate_shortest_path();

    grid.write_grid("./test_result.ppm");

    return 0;
}

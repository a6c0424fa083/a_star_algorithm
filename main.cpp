#include <iostream>
#include "Matrix.h"

int main() {
    Grid grid("./test.ppm");
    grid.show_grid_output("  ", true);
    return 0;
}

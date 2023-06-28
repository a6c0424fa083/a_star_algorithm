#
# Created by a6c0424fa083 on 15.06.23.
#

cmake_minimum_required(VERSION 3.25)
project(
    a_star_algorithm_4
    VERSION 0.4.0
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(
    ${PROJECT_NAME}
    main.cpp
    defines.h
    Color.h
    Coordinate.h
    Node.h
    file.h
    Grid.h
)

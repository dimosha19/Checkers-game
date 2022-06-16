cmake_minimum_required(VERSION 3.22)
project(checkers)

set(CMAKE_CXX_STANDARD 17)

add_executable(server server.cpp source/game.cpp)
add_executable(player1 player1.cpp)
add_executable(player2 player2.cpp)

set(SFML_STATIC_LIBRARIES TRUE)
set(SFML_DIR C:/Users/shiln/CLionProjects/SFML/lib/cmake/SFML)
find_package(SFML COMPONENTS system window graphics audio network REQUIRED)
include_directories(C:/Users/shiln/CLionProjects/SFML/includes)
target_link_libraries(server sfml-graphics sfml-system sfml-window sfml-audio sfml-network)
target_link_libraries(player1 sfml-graphics sfml-system sfml-window sfml-audio sfml-network)
target_link_libraries(player2 sfml-graphics sfml-system sfml-window sfml-audio sfml-network)
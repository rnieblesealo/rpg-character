cmake_minimum_required(VERSION 3.10)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# define project
project(rpg VERSION 1.0)

# add all source files in src
file(GLOB_RECURSE SRC_FILES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)
add_executable(game ${SRC_FILES})

# add own includes
target_include_directories(game PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

# add sfml, important to find_package first!
find_package(SFML COMPONENTS graphics window system audio)
target_link_libraries(game sfml-graphics sfml-window sfml-system sfml-audio)

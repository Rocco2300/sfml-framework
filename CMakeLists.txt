cmake_minimum_required(VERSION 3.23)
project(sfml_framework)

set(CMAKE_CXX_STANDARD 20)

include_directories(include)
include_directories(vendor/sfml/include)

add_subdirectory(src)
add_subdirectory(vendor/sfml)

add_executable(playground Playground.cpp)
add_executable(customTexGen CustomTextureGenerator.cpp)

target_link_libraries(playground core)
target_link_libraries(playground sfml-graphics sfml-window sfml-system sfml-audio)
cmake_minimum_required(VERSION 3.23)
project(sfml_framework)

set(CMAKE_CXX_STANDARD 20)

add_subdirectory(src)
add_subdirectory(vendor/sfml)

add_executable(playground Playground.cpp)
target_include_directories(playground PUBLIC include)
target_include_directories(playground PUBLIC vendor/sfml/include)
target_include_directories(playground PUBLIC vendor/nlohmann-json/include)

target_link_libraries(playground core)
target_link_libraries(playground sfml-graphics sfml-window sfml-system sfml-audio)

add_executable(customTexGen CustomTextureGenerator.cpp)
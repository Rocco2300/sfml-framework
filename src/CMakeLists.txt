file(GLOB SOURCES *.cpp)
file(GLOB HEADERS ${CMAKE_SOURCE_DIR}/include/*.h)

add_library(core ${SOURCES} ${HEADERS})

target_include_directories(core PUBLIC ${CMAKE_SOURCE_DIR}/include)
target_include_directories(core PUBLIC ${CMAKE_SOURCE_DIR}/vendor/sfml/include)
target_include_directories(core PUBLIC ${CMAKE_SOURCE_DIR}/vendor/nlohmann-json/include)

target_link_libraries(core sfml-graphics)
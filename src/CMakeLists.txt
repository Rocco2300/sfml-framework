file(GLOB SOURCES *.cpp)
file(GLOB HEADERS include/*.h)

add_library(core ${SOURCES} ${HEADERS})
target_link_libraries(core sfml-graphics)
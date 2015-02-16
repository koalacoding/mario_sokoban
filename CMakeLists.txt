cmake_minimum_required(VERSION 2.8)
project(mario C)

find_package(SDL)
find_package(SDL_image)

include_directories(${SDL_IMAGE_INCLUDE_DIRS} ${SDL_INCLUDE_DIR})

# to activate debug mode in qtcreator you have to add
# -DCMAKE_BUILD_TYPE=Debug to the projects/CMake arguments until bug is fixed...
add_definitions(-D_DEBUG)

add_executable(mario
               main.c
               sprite.c
               sprite.h
               window.c
               window.h
               board.c
               board.h
               map.c
               map.h
               way.h
               debug.c
               debug.h)

target_link_libraries(mario ${SDL_IMAGE_LIBRARIES} ${SDL_LIBRARY})

file(COPY maps DESTINATION ${CMAKE_BINARY_DIR})
file(COPY sprites DESTINATION ${CMAKE_BINARY_DIR})

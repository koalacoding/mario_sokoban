cmake_minimum_required(VERSION 2.8)
project(mario C)

find_package(SDL)
find_package(SDL_image)

include_directories(${SDL_IMAGE_INCLUDE_DIRS} ${SDL_INCLUDE_DIR})

# to activate debug mode in qtcreator you have to add
set(CMAKE_BUILD_TYPE Debug)  # until qtcreator fix it's target selection
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
               status.c
               status.h)

target_link_libraries(mario ${SDL_IMAGE_LIBRARIES} ${SDL_LIBRARY})

file(COPY maps DESTINATION ${CMAKE_BINARY_DIR})
file(COPY sprites DESTINATION ${CMAKE_BINARY_DIR})

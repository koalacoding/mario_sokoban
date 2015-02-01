cmake_minimum_required(VERSION 2.8)
project(mario)

find_package(SDL)

include_directories(${SDL_INCLUDE_DIR})
add_executable(mario main.c)
target_link_libraries(mario ${SDL_LIBRARY})

file(COPY maps/map0.map
	 DESTINATION ${CMAKE_BINARY_DIR}/maps)

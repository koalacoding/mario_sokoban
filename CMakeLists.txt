cmake_minimum_required(VERSION 2.8)
project(mario)

find_package(SDL)
find_package(SDL_image)

include_directories(${SDL_IMAGE_INCLUDE_DIRS}
					${SDL_INCLUDE_DIR})
add_executable(mario main.c)
target_link_libraries(mario ${SDL_IMAGE_LIBRARIES}
					  ${SDL_LIBRARY})

file(COPY maps
	 DESTINATION ${CMAKE_BINARY_DIR})

file(COPY sprites
	 DESTINATION ${CMAKE_BINARY_DIR})

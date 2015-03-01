cmake_minimum_required(VERSION 2.8)
project(mario)

find_package(SDL)
find_package(SDL_image)
find_package(SDL_ttf)

# to activate debug mode in qtcreator you have to add
# -DCMAKE_BUILD_TYPE=Debug to the projects/CMake arguments until bug is fixed...
add_definitions(-D_DEBUG)
set(CMAKE_BUILD_TYPE Debug)

include_directories(${SDL_TTF_INCLUDE_DIRS}
					${SDL_IMAGE_INCLUDE_DIRS}
					${SDL_INCLUDE_DIR})

add_executable(mario
			   main.c
			   game_window/game_window.c
			   game_window/game_window.h
			   main_window/main_window.c
			   main_window/main_window.h
			   map_editor/map_editor.c
			   map_editor/map_editor.h
			   select_map_window/select_map_window.c
			   select_map_window/select_map_window.h
			   tools/blit_surface/blit_surface.h
			   tools/blit_surface/blit_surface.c
			   tools/write_text_on_window/write_text_on_window.c
			   tools/write_text_on_window/write_text_on_window.h
			   structures.h)

target_link_libraries(mario ${SDL_IMAGE_LIBRARIES}
							${SDL_TTF_LIBRARIES}
							${SDL_LIBRARY})

target_compile_options(mario PUBLIC "-O0")

file(COPY ubuntu-b.ttf DESTINATION ${CMAKE_BINARY_DIR})
file(COPY maps DESTINATION ${CMAKE_BINARY_DIR})
file(COPY images DESTINATION ${CMAKE_BINARY_DIR})

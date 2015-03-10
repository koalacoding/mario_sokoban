cmake_minimum_required(VERSION 2.8)
project(mario C)

find_package(SDL)
find_package(SDL_image)
find_package(SDL_ttf)

include_directories(${SDL_INCLUDE_DIR}
                    ${SDL_IMAGE_INCLUDE_DIRS}
                    ${SDL_TTF_INCLUDE_DIRS})

# to activate debug mode in qtcreator you have to add
set(CMAKE_BUILD_TYPE Debug)  # until qtcreator fix it's target selection
add_definitions(-D_DEBUG)

add_executable(mario
               main.c
               game.c
               game.h
               menu.c
               menu.h
               menu_view.c
               menu_view.h
               editor.c
               editor.h
               sprite.c
               sprite.h
               window.c
               window.h
               map.c
               map.h
               map_view.c
               map_view.h
               status.c
               status.h)

target_link_libraries(mario ${SDL_LIBRARY}
                            ${SDL_IMAGE_LIBRARIES}
                            ${SDL_TTF_LIBRARIES})

file(COPY fonts DESTINATION ${CMAKE_BINARY_DIR})
file(COPY maps DESTINATION ${CMAKE_BINARY_DIR})
file(COPY sprites DESTINATION ${CMAKE_BINARY_DIR})

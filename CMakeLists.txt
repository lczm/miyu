project(miyu)

cmake_minimum_required(VERSION 3.5)

set(CMAKE_CXX_FLAGS "-Wall -O2")
set(CMAKE_CXX_STANDARD 14)

add_executable(${PROJECT_NAME} 
        main.cpp
        render_utils.h)

if (WIN32)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/modules")

find_package(SDL2 REQUIRED sdl2)
find_package(SDL2IMAGE REQUIRED SDL2_image>2.0.0)
endif(WIN32)

if (UNIX)
include(FindPkgConfig)

PKG_SEARCH_MODULE(SDL2 REQUIRED sdl2)
PKG_SEARCH_MODULE(SDL2IMAGE REQUIRED SDL2_image>=2.0.0)
endif(UNIX)
include_directories(${SDL2_INCLUDE_DIRS} ${SDL2IMAGE_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} 
        ${SDL2_LIBRARIES} 
        ${SDL2IMAGE_LIBRARIES})

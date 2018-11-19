cmake_minimum_required(VERSION 3.0)
project(ultimatetapankaikki)

set(CMAKE_MODULE_PATH ${CMAKE_BINARY_DIR}/cmake-modules)

if(NOT EXISTS "${CMAKE_BINARY_DIR}/cmake-modules/FindSDL2.cmake")
    file(DOWNLOAD "https://github.com/tcbrindle/sdl2-cmake-scripts/raw/master/FindSDL2.cmake" "${CMAKE_BINARY_DIR}/cmake-modules/FindSDL2.cmake")
endif()

find_package(SDL2 REQUIRED)

file(GLOB_RECURSE SOURCES SRC/*.H SRC/*.CPP SRC/*.C)

add_executable(tk3 ${SOURCES})
target_include_directories(tk3 SYSTEM PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/SRC ${SDL2_INCLUDE_DIR})
target_link_libraries(tk3 ${SDL2_LIBRARY})

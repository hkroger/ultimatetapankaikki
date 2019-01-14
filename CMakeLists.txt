cmake_minimum_required(VERSION 3.0)
project(ultimatetapankaikki)

set(CMAKE_MODULE_PATH ${CMAKE_BINARY_DIR}/cmake-modules)

if(NOT EXISTS "${CMAKE_BINARY_DIR}/cmake-modules/FindSDL2.cmake")
	file(DOWNLOAD "https://github.com/tcbrindle/sdl2-cmake-scripts/raw/master/FindSDL2.cmake" "${CMAKE_BINARY_DIR}/cmake-modules/FindSDL2.cmake")
endif()
if(NOT EXISTS "${CMAKE_BINARY_DIR}/cmake-modules/FindSDL2_mixer.cmake")
	file(DOWNLOAD "https://github.com/tcbrindle/sdl2-cmake-scripts/raw/master/FindSDL2_mixer.cmake" "${CMAKE_BINARY_DIR}/cmake-modules/FindSDL2_mixer.cmake")
endif()
if(NOT EXISTS "${CMAKE_BINARY_DIR}/cmake-modules/FindSDL2_image.cmake")
	file(DOWNLOAD "https://github.com/tcbrindle/sdl2-cmake-scripts/raw/master/FindSDL2_image.cmake" "${CMAKE_BINARY_DIR}/cmake-modules/FindSDL2_image.cmake")
endif()
if(NOT EXISTS "${CMAKE_BINARY_DIR}/cmake-modules/FindSDL2_net.cmake")
	file(DOWNLOAD "https://github.com/veikkos/sdl2-cmake-scripts/raw/master/FindSDL2_net.cmake" "${CMAKE_BINARY_DIR}/cmake-modules/FindSDL2_net.cmake")
endif()

find_package(SDL2 REQUIRED)
find_package(SDL2_mixer REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2_net REQUIRED)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

if (WIN32)
	add_compile_options(/WX /wd4244 /wd4996)
else()
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -funsigned-char -Wall -Wno-pedantic -Wsign-compare")
endif()

file(GLOB SOURCES
	SRC/*.H SRC/*.CPP
	SRC/EFP/*.H SRC/EFP/*.CPP
	SRC/ERROR/*.H SRC/ERROR/*.CPP
	SRC/NET/*.H SRC/NET/*.CPP
	SRC/INPUT/*.H SRC/INPUT/*.CPP)

if (WIN32)
	file(GLOB MSVC_SOURCES SRC/MSVC/*.H SRC/MSVC/*.CPP)
	list(APPEND SOURCES ${MSVC_SOURCES})
endif()

add_executable(ultimatetapankaikki ${SOURCES})
target_include_directories(ultimatetapankaikki SYSTEM PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/SRC ${SDL2_INCLUDE_DIR} ${SDL2_MIXER_INCLUDE_DIRS} ${SDL2_IMAGE_INCLUDE_DIRS} ${SDL2_NET_INCLUDE_DIRS})
target_link_libraries(ultimatetapankaikki ${SDL2_LIBRARY} ${SDL2_IMAGE_LIBRARIES} ${SDL2_MIXER_LIBRARIES} ${SDL2_NET_LIBRARIES})

if (WIN32)
	set_target_properties(ultimatetapankaikki PROPERTIES OUTPUT_NAME "GAME" SUFFIX ".EXE")
endif()

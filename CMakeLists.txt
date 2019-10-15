cmake_minimum_required(VERSION 3.0)
project(ultimatetapankaikki VERSION 1.0.2)

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

if (NOT CMAKE_BUILD_TYPE)
	set(CMAKE_BUILD_TYPE Debug)
endif()

include(GNUInstallDirs)
if (NOT WIN32 AND NOT (${CMAKE_BUILD_TYPE} STREQUAL "Debug"))
	set(TK_DATADIR "${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_DATADIR}/${PROJECT_NAME}")
else()
	set(TK_DATADIR ".")
endif()
set(TK_SUFFIXED_DATADIR ${CMAKE_INSTALL_DATADIR}/${PROJECT_NAME})

if (CMAKE_MACOSX_BUNDLE)
	set(CMAKE_INSTALL_DEFAULT_COMPONENT_NAME Runtime)
	set(CMAKE_INSTALL_BINDIR ".")  # no bin/ directory under destination directory
	set(CMAKE_INSTALL_DATADIR ${CMAKE_INSTALL_BINDIR}/${PROJECT_NAME}.app/Contents/Resources)
	set(CMAKE_INSTALL_DOCDIR ${CMAKE_INSTALL_BINDIR}/${PROJECT_NAME}.app/Contents/Documentation)
	set(TK_SUFFIXED_DATADIR ${CMAKE_INSTALL_DATADIR})  # within bundle, no suffix needed
	set(TK_MACOSX_BUNDLE 1)
	set(MACOSX_BUNDLE_IDENTIFIER "suomipelit.ultimatetapankaikki")
	set(ICON_NAME "icon.icns")
	set(ICON_PATH "icon/${ICON_NAME}")
	set(MACOSX_BUNDLE_ICON_FILE ${ICON_NAME})
endif()

configure_file("${PROJECT_SOURCE_DIR}/CONFIG.H.IN" "${PROJECT_BINARY_DIR}/CONFIG.H")
include_directories(${PROJECT_BINARY_DIR})

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

if (WIN32)
	add_compile_options(/MT /WX /wd4244 /wd4996)
else()
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -funsigned-char -Wall -Wno-pedantic -Wsign-compare")
endif()

file(GLOB SOURCES
	SRC/*.H SRC/*.CPP
	SRC/EFP/*.H SRC/EFP/*.CPP
	SRC/ERROR/*.H SRC/ERROR/*.CPP
	SRC/NET/*.H SRC/NET/*.CPP
	SRC/INPUT/*.H SRC/INPUT/*.CPP
        CONFIG.H)

if (WIN32)
	file(GLOB MSVC_SOURCES SRC/MSVC/*.H SRC/MSVC/*.CPP)
	list(APPEND SOURCES ${MSVC_SOURCES})
endif()

add_executable(${PROJECT_NAME} ${SOURCES})
target_include_directories(${PROJECT_NAME} SYSTEM PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/SRC ${SDL2_INCLUDE_DIR} ${SDL2_MIXER_INCLUDE_DIRS} ${SDL2_IMAGE_INCLUDE_DIRS} ${SDL2_NET_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} ${SDL2_LIBRARY} ${SDL2_IMAGE_LIBRARIES} ${SDL2_MIXER_LIBRARIES} ${SDL2_NET_LIBRARIES})

if (WIN32)
	set_target_properties(${PROJECT_NAME} PROPERTIES OUTPUT_NAME "GAME" SUFFIX ".EXE")
endif()


install(TARGETS ${PROJECT_NAME} DESTINATION ${CMAKE_INSTALL_BINDIR})
install(FILES OPTIONS.CFG DESTINATION ${TK_SUFFIXED_DATADIR})
install(DIRECTORY EFPS FNTS LEVS MUSIC WAVS icon DESTINATION ${TK_SUFFIXED_DATADIR})
install(FILES README.md README.ORIG.TXT EDITOR.TXT EPISODES.TXT LICENSE WHAT'S.NEW DESTINATION ${CMAKE_INSTALL_DOCDIR})
install(FILES pkg/ultimatetapankaikki.desktop DESTINATION ${CMAKE_INSTALL_DATADIR}/applications)
if (CMAKE_MACOSX_BUNDLE)
	install(FILES ${ICON_PATH} DESTINATION ${TK_SUFFIXED_DATADIR})
	INSTALL(CODE "
	set(BU_CHMOD_BUNDLE_ITEMS ON)
	include(BundleUtilities)
	fixup_bundle(\"\${CMAKE_INSTALL_PREFIX}/${PROJECT_NAME}.app\" \"\" \"\")
	" COMPONENT Runtime)
	# see e.g. https://cmake.org/pipermail/cmake/2011-March/043532.html
endif()

include(CPack)

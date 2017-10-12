cmake_minimum_required(VERSION 3.1)
project(tinyx86)

# Check for in tree builds
if("${CMAKE_SOURCE_DIR}" STREQUAL "${CMAKE_BINARY_DIR}")
    message(FATAL_ERROR "In-source builds are not allowed.")
endif("${CMAKE_SOURCE_DIR}" STREQUAL "${CMAKE_BINARY_DIR}")

# Set CFLAGS
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra")
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_CFLAGS_DEBUG} -g -O0")
set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_CFLAGS_RELEASE} -O3")

include_directories(include)

add_executable(tinyx86 board.c log.c main.c tinyx86.c)
add_subdirectory(hw)

INCLUDE(FindPkgConfig)

PKG_SEARCH_MODULE(SDL2 REQUIRED sdl2)

include_directories(${SDL2_INCLUDE_DIRS})
target_link_libraries(tinyx86 ${SDL2_LIBRARIES})

install(TARGETS tinyx86 DESTINATION /usr/local/bin)

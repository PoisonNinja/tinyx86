cmake_minimum_required(VERSION 3.1)
project(x86emu)

# Check for in tree builds
if("${CMAKE_SOURCE_DIR}" STREQUAL "${CMAKE_BINARY_DIR}")
    message(FATAL_ERROR "In-source builds are not allowed.")
endif("${CMAKE_SOURCE_DIR}" STREQUAL "${CMAKE_BINARY_DIR}")

# Set CFLAGS
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -std=gnu11")
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_CFLAGS_DEBUG} -g -O0")
set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_CFLAGS_RELEASE} -O3")

include_directories(include)

add_executable(x86emu log.c main.c)
add_subdirectory(cpu)

install(TARGETS x86emu DESTINATION /usr/local/bin)

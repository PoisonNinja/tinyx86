target_sources(tinyx86
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/io.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/rtc.cc
)

add_subdirectory(memory)
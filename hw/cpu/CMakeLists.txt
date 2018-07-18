target_sources(tinyx86
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/arithmetic.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/cpu.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/decode.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/modrm.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/opcode.cc
)

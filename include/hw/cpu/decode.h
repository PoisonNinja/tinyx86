#pragma once

#include <spdlog/spdlog.h>
#include <cstdint>

class CPU;

enum class SGRegister : int;
enum class GPRegister : int;

struct PrefixState {
    SGRegister segment;
    uint8_t operand32;
    uint8_t repne;
    uint8_t repe;
};

class InstructionDecoder
{
public:
    InstructionDecoder(CPU& cpu);
    ~InstructionDecoder();

    void tick();
    void reset();

private:
    std::shared_ptr<spdlog::logger> log;

    CPU& cpu;

    PrefixState prefixes;

    // ModRM byte
    uint8_t modrm;

    typedef void (InstructionDecoder::*InstructionPointer)();

    InstructionPointer opcodes[256];

    // Instructions
    void add_rm8_r8();
};

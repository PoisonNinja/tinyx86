#pragma once

#include <cstdint>

class CPU;

struct PrefixState {
    struct Segment* segment;
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

    // Instructions
    void add_rm8_r8();

private:
    CPU& cpu;

    PrefixState prefixes;

    // ModRM byte
    uint8_t modrm;

    typedef void (InstructionDecoder::*InstructionPointer)();

    InstructionPointer opcodes[256];
};
#pragma once

#include <spdlog/spdlog.h>
#include <cstdint>

class CPU;

enum class SGRegister : int;
enum class GPRegister : int;

struct PrefixState {
    SGRegister segment;
    bool opsize;
    bool addrsize;
    uint8_t repne;
    uint8_t repe;
    void reset();
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

    InstructionPointer opcodes16[256];
    InstructionPointer opcodes32[256];

    // Instructions
    void mov_sp_imm16();
    void mov_esp_imm32();
};

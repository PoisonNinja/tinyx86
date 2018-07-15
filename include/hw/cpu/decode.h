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
    void push_dx();  // 0x52
    void push_edx();
    void push_bx();  // 0x53
    void push_ebx();

    void mov_ax_imm16();  // 0xB8
    void mov_eax_imm32();
    void mov_sp_imm16();  // 0xBC
    void mov_esp_imm32();

    void call_rel16();  // 0xE8
    void call_rel32();
};

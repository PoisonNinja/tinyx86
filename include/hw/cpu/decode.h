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

struct ModRM {
    uint8_t rm : 3;
    uint8_t reg : 3;
    uint8_t mod : 2;
} __attribute__((packed));

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
    const struct ModRM* raw_to_modrm();
    addr_t modrm_to_address(uint8_t mod, uint8_t rm);

    typedef void (InstructionDecoder::*InstructionPointer)();

    InstructionPointer opcodes16[256];
    InstructionPointer opcodes32[256];

    // ModRM decoding
    uint8_t read_modrm_r8();
    uint8_t read_modrm_rm8();
    uint16_t read_modrm_r16();
    uint16_t read_modrm_rm16();
    uint32_t read_modrm_r32();
    uint32_t read_modrm_rm32();

    /*
     * Note to future me: Do not convert these into overloaded functions because
     * they need to match the read_modrm_* family above in pairs
     */
    void write_modrm_r8(uint8_t value);
    void write_modrm_rm8(uint8_t value);
    void write_modrm_r16(uint16_t value);
    void write_modrm_rm16(uint16_t value);
    void write_modrm_r32(uint32_t value);
    void write_modrm_rm32(uint32_t value);

    // Instructions
    void push_dx();  // 0x52
    void push_edx();
    void push_bx();  // 0x53
    void push_ebx();

    void mov_rm16_r16();  // 0x89
    void mov_rm32_r32();

    void mov_ax_imm16();  // 0xB8
    void mov_eax_imm32();
    void mov_sp_imm16();  // 0xBC
    void mov_esp_imm32();

    void call_rel16();  // 0xE8
    void call_rel32();
};

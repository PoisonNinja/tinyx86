#pragma once

#include <spdlog/spdlog.h>
#include <tinyx86.h>
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
    uint8_t raw_modrm;
    struct ModRM* modrm;
    void load_modrm();
    addr_t modrm_to_address(uint8_t mod, uint8_t rm);

    // Operand size
    bool is_osize_32();
    bool is_asize_32();

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

    // Arithmetic
    template <typename T>
    T add(T a, T b);
    template <typename T>
    T do_and(T a, T b);  // and is a keyword in C++, alternative to &&
    template <typename T>
    T inc(T v);
    template <typename T>
    T do_or(T a, T b);  // or is a keyword in C++, alternative to ||

    // Instructions
    void and_rm8_r8();  // 0x20

    void inc_bx();  // 0x43
    void inc_ebx();
    void inc_di();  // 0x47
    void inc_edi();

    void push_dx();  // 0x52
    void push_edx();
    void push_bx();  // 0x53
    void push_ebx();
    void push_di();  // 0x57
    void push_edi();

    void pop_dx();  // 0x5A
    void pop_edx();
    void pop_bx();  // 0x5B
    void pop_ebx();
    void pop_di();  // 0x5F
    void pop_edi();

    void jmpnz_ne();  // 0x75

    void xchg_r8_rm8();  // 0x86

    void mov_rm8_r8();    // 0x88
    void mov_rm16_r16();  // 0x89
    void mov_rm32_r32();
    void mov_r8_rm8();    // 0x8A
    void mov_r16_rm16();  // 0x8B
    void mov_r32_rm32();

    void mov_ax_imm16();  // 0xB8
    void mov_eax_imm32();
    void mov_bx_imm16();  // 0xBB
    void mov_ebx_imm32();
    void mov_sp_imm16();  // 0xBC
    void mov_esp_imm32();

    void retn16();  // 0xC3
    void retn32();

    void call_rel16();  // 0xE8
    void call_rel32();

    void hlt();  // 0xF4
};

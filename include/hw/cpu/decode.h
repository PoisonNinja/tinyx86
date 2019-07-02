#pragma once

#include <spdlog/spdlog.h>
#include <tinyx86.h>
#include <cstdint>

class CPU;

enum class SGRegister : int;

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
    addr_t decoded_addr;
    void load_modrm();
    addr_t modrm_to_address(uint8_t mod, uint8_t rm);

    // Operand size
    bool is_osize_32();
    bool is_asize_32();

    using InstructionPointer = void (InstructionDecoder::*)();

    InstructionPointer opcodes16[256];
    InstructionPointer opcodes32[256];

    InstructionPointer fopcodes16[256];
    InstructionPointer fopcodes32[256];

    // ModRM decoding
    SGRegister decode_modrm_sgreg();

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
    /*
     * write_modrm_rm* variants must be called after a call to read_modrm_rm*
     */
    void write_modrm_r8(uint8_t value);
    void write_modrm_rm8(uint8_t value);
    void write_modrm_r16(uint16_t value);
    void write_modrm_rm16(uint16_t value);
    void write_modrm_r32(uint32_t value);
    void write_modrm_rm32(uint32_t value);

    // Arithmetic
    void aaa();
    void aad(uint8_t imm8);
    void aam(uint8_t imm8);
    void aas();
    template <typename T>
    T adc(T a, T b);
    template <typename T>
    T add(T a, T b);
    template <typename T>
    T do_and(T a, T b);  // and is a keyword in C++, alternative to &&
    template <typename T>
    T bsf(T a, T b);
    template <typename T>
    T bsr(T a, T b);
    // template <typename T>
    // T bt(T a, T b);
    // template <typename T>
    // T btc(T a, T b);
    // template <typename T>
    // T btr(T a, T b);
    // template <typename T>
    // T bts(T a, T b);
    template <typename T>
    void cmp(T a, T b);
    void daa();
    void das();
    template <typename T>
    T dec(T a);
    template <typename T>
    void div(T a);
    template <typename T>
    void idiv(T a);
    template <typename T, typename R>
    R imul(T a, T b);
    template <typename T>
    T inc(T a);
    template <typename T>
    void mul(T a);
    template <typename T>
    T neg(T a);
    template <typename T>
    T do_not(T a, T b);  // not is a keyword in C++, alternative to !
    template <typename T>
    T do_or(T a, T b);  // or is a keyword in C++, alternative to ||
    template <typename T>
    T popcnt(T a, T b);
    template <typename T>
    T rcl(T a, T b);
    template <typename T>
    T rcr(T a, T b);
    template <typename T>
    T rol(T a, T b);
    template <typename T>
    T ror(T a, T b);
    template <typename T>
    T sar(T a, T b);
    template <typename T>
    T sbb(T a, T b);
    template <typename T>
    T shl(T a, T b);
    template <typename T>
    T shld(T a, T b, T c);
    template <typename T>
    T shr(T a, T b);
    template <typename T>
    T shrd(T a, T b, T c);
    template <typename T>
    T sub(T a, T b);
    template <typename T>
    void test(T a, T b);
    template <typename T>
    T do_xor(T a, T b);  // xor is a keyword in C++, alterative to ^

    // Instructions
    void add_rm8_r8();    // 0x00
    void add_rm16_r16();  // 0x01
    void add_rm32_r32();
    void add_r8_rm8();    // 0x02
    void add_r16_rm16();  // 0x03
    void add_r32_rm32();
    void add_al_imm8();   // 0x04
    void add_ax_imm16();  // 0x05
    void add_eax_imm32();
    void push_es16();  // 0x06
    void push_es32();
    void pop_es16();  // 0x07
    void pop_es32();
    void or_rm8_r8();    // 0x08
    void or_rm16_r16();  // 0x09
    void or_rm32_r32();
    void or_r8_rm8();    // 0x0A
    void or_r16_rm16();  // 0x0B
    void or_r32_rm32();
    void or_al_imm8();   // 0x0C
    void or_ax_imm16();  // 0x0D
    void or_eax_imm32();
    void push_cs16();  // 0x0E
    void push_cs32();

    // 0x0F are 2 byte operations

    void adc_rm8_r8();    // 0x10
    void adc_rm16_r16();  // 0x11
    void adc_rm32_r32();
    void adc_r8_rm8();    // 0x12
    void adc_r16_rm16();  // 0x13
    void adc_r32_rm32();
    void adc_al_imm8();   // 0x14
    void adc_ax_imm16();  // 0x15
    void adc_eax_imm32();
    void push_ss16();  // 0x16
    void push_ss32();
    void pop_ss16();  // 0x17
    void pop_ss32();
    void sbb_rm8_r8();    // 0x18
    void sbb_rm16_r16();  // 0x19
    void sbb_rm32_r32();
    void sbb_r8_rm8();    // 0x1A
    void sbb_r16_rm16();  // 0x1B
    void sbb_r32_rm32();
    void sbb_al_imm8();   // 0x1C
    void sbb_ax_imm16();  // 0x1D
    void sbb_eax_imm32();
    void push_ds16();  // 0x1E
    void push_ds32();
    void pop_ds16();  // 0x1F
    void pop_ds32();
    void and_rm8_r8();    // 0x20
    void and_rm16_r16();  // 0x21
    void and_rm32_r32();
    void and_r8_rm8();    // 0x22
    void and_r16_rm16();  // 0x23
    void and_r32_rm32();
    void and_al_imm8();   // 0x24
    void and_ax_imm16();  // 0x25
    void and_eax_imm32();

    // 0x26 is ES override

    void daa_al();        // 0x27
    void sub_rm8_r8();    // 0x28
    void sub_rm16_r16();  // 0x29
    void sub_rm32_r32();
    void sub_r8_rm8();    // 0x2A
    void sub_r16_rm16();  // 0x2B
    void sub_r32_rm32();
    void sub_al_imm8();   // 0x2C
    void sub_ax_imm16();  // 0x2D
    void sub_eax_imm32();

    // 0x2E is CS override

    void das_al();        // 0x2F
    void xor_rm8_r8();    // 0x30
    void xor_rm16_r16();  // 0x31
    void xor_rm32_r32();
    void xor_r8_rm8();    // 0x32
    void xor_r16_rm16();  // 0x33
    void xor_r32_rm32();
    void xor_al_imm8();   // 0x34
    void xor_ax_imm16();  // 0x35
    void xor_eax_imm32();

    // 0x36 is SS override

    void aaa_al_ah();     // 0x37
    void cmp_rm8_r8();    // 0x38
    void cmp_rm16_r16();  // 0x39
    void cmp_rm32_r32();
    void cmp_r8_rm8();    // 0x3A
    void cmp_r16_rm16();  // 0x3B
    void cmp_r32_rm32();
    void cmp_al_imm8();   // 0x3C
    void cmp_ax_imm16();  // 0x3D
    void cmp_eax_imm32();

    // 0x3E is DS override

    void aas_al_ah();  // 0x3F
    void inc_ax();     // 0x40
    void inc_eax();
    void inc_cx();  // 0x41
    void inc_ecx();
    void inc_dx();  // 0x42
    void inc_edx();
    void inc_bx();  // 0x43
    void inc_ebx();
    void inc_sp();  // 0x44
    void inc_esp();
    void inc_bp();  // 0x45
    void inc_ebp();
    void inc_si();  // 0x46
    void inc_esi();
    void inc_di();  // 0x47
    void inc_edi();
    void dec_ax();  // 0x48
    void dec_eax();
    void dec_cx();  // 0x49
    void dec_ecx();
    void dec_dx();  // 0x4A
    void dec_edx();
    void dec_bx();  // 0x4B
    void dec_ebx();
    void dec_sp();  // 0x4C
    void dec_esp();
    void dec_bp();  // 0x4D
    void dec_ebp();
    void dec_si();  // 0x4E
    void dec_esi();
    void dec_di();  // 0x4F
    void dec_edi();
    void push_ax();  // 0x50
    void push_eax();
    void push_cx();  // 0x51
    void push_ecx();
    void push_dx();  // 0x52
    void push_edx();
    void push_bx();  // 0x53
    void push_ebx();
    void push_sp();  // 0x54
    void push_esp();
    void push_bp();  // 0x55
    void push_ebp();
    void push_si();  // 0x56
    void push_esi();
    void push_di();  // 0x57
    void push_edi();

    void pop_ax();  // 0x58
    void pop_eax();
    void pop_cx();  // 0x59
    void pop_ecx();
    void pop_dx();  // 0x5A
    void pop_edx();
    void pop_bx();  // 0x5B
    void pop_ebx();
    void pop_sp();  // 0x5C
    void pop_esp();
    void pop_bp();  // 0x5D
    void pop_ebp();
    void pop_si();  // 0x5E
    void pop_esi();
    void pop_di();  // 0x5F
    void pop_edi();

    void pusha16();  // 0x60
    void pusha32();
    void popa16();  // 0x61
    void popa32();

    // TODO: 0x62, BOUND
    // TODO: 0x63, ARPL
    // 0x64 is FS override
    // 0x65 is GS override

    void push_imm16();  // 0x68
    void push_imm32();
    void imul_r16_rm16_imm16();  // 0x69
    void imul_r32_rm32_imm32();
    void push_imm8();           // 0x6A
    void imul_r16_rm16_imm8();  // 0x6B
    void imul_r32_rm32_imm8();

    // TODO: 0x6C - 0x6F/INS/OUTS

    void jo();           // 0x70
    void jno();          // 0x71
    void jb_jnae_jc();   // 0x72
    void jnb_jae_jnc();  // 0x73
    void jz_je();        // 0x74
    void jnz_jne();      // 0x75
    void jbe_jna();      // 0x76
    void jnbe_ja();      // 0x77
    void js();           // 0x78
    void jns();          // 0x79
    void jp_jpe();       // 0x7A
    void jnp_jpo();      // 0x7B
    void jl_jnge();      // 0x7C
    void jnl_jge();      // 0x7D
    void jle_jng();      // 0x7E
    void jnle_jg();      // 0x7F

    void do_rm8_imm8();    // 0x80
    void do_rm16_imm16();  // 0x81
    void do_rm32_imm32();
    // 0x82 is aliased to 0x80
    void do_rm16_imm8();  // 0x83
    void do_rm32_imm8();
    void test_rm8_r8();    // 0x84
    void test_rm16_r16();  // 0x85
    void test_rm32_r32();
    void xchg_rm8_r8();    // 0x86
    void xchg_rm16_r16();  // 0x87
    void xchg_rm32_r32();
    void mov_rm8_r8();    // 0x88
    void mov_rm16_r16();  // 0x89
    void mov_rm32_r32();
    void mov_r8_rm8();    // 0x8A
    void mov_r16_rm16();  // 0x8B
    void mov_r32_rm32();
    void mov_rm16_sreg();  // 0x8C
    void mov_rm32_sreg();
    void lea_rm16_m();  // 0x8D
    void lea_rm32_m();
    void mov_sgreg_rm16();  // 0x8E
    void pop_rm16();        // 0x8F
    void pop_rm32();

    void mov_al_imm8();   // 0xB0
    void mov_cl_imm8();   // 0xB1
    void mov_dl_imm8();   // 0xB2
    void mov_bl_imm8();   // 0xB3
    void mov_ah_imm8();   // 0xB4
    void mov_ch_imm8();   // 0xB5
    void mov_dh_imm8();   // 0xB6
    void mov_bh_imm8();   // 0xB7
    void mov_ax_imm16();  // 0xB8
    void mov_eax_imm32();
    void mov_cx_imm16();  // 0xB9
    void mov_ecx_imm32();
    void mov_dx_imm16();  // 0xBA
    void mov_edx_imm32();
    void mov_bx_imm16();  // 0xBB
    void mov_ebx_imm32();
    void mov_sp_imm16();  // 0xBC
    void mov_esp_imm32();
    void mov_bp_imm16();  // 0xBD
    void mov_ebp_imm32();
    void mov_si_imm16();  // 0xBE
    void mov_esi_imm32();
    void mov_di_imm16();  // 0xBF
    void mov_edi_imm32();

    void retn16();  // 0xC3
    void retn32();

    void out_imm8_al();  // 0xE6
    void out_imm8_ax();  // 0xE7
    void out_imm8_eax();
    void call_rel16();  // 0xE8
    void call_rel32();
    void jmp_rel16();  // 0xE9
    void jmp_rel32();
    void jmpf_ptr16();  // 0xEA
    void jmpf_ptr32();

    void hlt();  // 0xF4
    void cli();  // 0xFA
    void cld();  // 0xFC

    // Two byte opcodes
    void jnz_jne16();  // 0x85
    void jnz_jne32();
};

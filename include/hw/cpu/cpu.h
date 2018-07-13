#pragma once

#include <hw/board.h>
#include <cstddef>
#include <cstdint>

union Register {
    uint32_t regs_32;
    uint16_t regs_16;
    uint8_t regs_8;
};

struct Segment {
    uint32_t base, limit;
    uint16_t selector;
};

constexpr size_t num_gpregs = 8;
constexpr size_t num_sgregs = 6;

constexpr size_t operand_size_8 = 7;
constexpr size_t operand_size_16 = 15;
constexpr size_t operand_size_32 = 31;

constexpr uint32_t eflags_cf = (1 << 0);
constexpr uint32_t eflags_pf = (1 << 2);
constexpr uint32_t eflags_af = (1 << 4);
constexpr uint32_t eflags_zf = (1 << 6);
constexpr uint32_t eflags_sf = (1 << 7);
constexpr uint32_t eflags_of = (1 << 11);
constexpr uint32_t eflags_all =
    (eflags_cf | eflags_pf | eflags_af | eflags_zf | eflags_sf | eflags_of);

class EFLAGS
{
public:
    EFLAGS();
    ~EFLAGS();

    uint32_t get_eflags();  // Gets EFLAGS and recomputes as necessary
    bool get_cf();
    bool get_pf();
    bool get_af();
    bool get_zf();
    bool get_sf();
    bool get_of();

private:
    uint32_t eflags;           // Internal representation of EFLAGS
    uint32_t eflags_dirty;     // EFLAGS dirty status
    uint32_t last_op1;         // Last operand #1
    uint32_t last_op2;         // Last operand #2
    uint32_t last_result;      // Result of last operation
    uint32_t last_add_result;  // Result of last add operation
    uint8_t last_size;         // Size of last operation
};

enum class GPRegister : int {
    AX = 0,
    BX,
    CX,
    DX,
    SP,
    BP,
    SI,
    DI,
};

enum class SGRegister : int {
    CS = 0,
    DS,
    SS,
    ES,
    FS,
    GS,
};

enum class CPUState {
    STOPPED,
    HALTED,
    RUNNING,
};

struct PrefixState {
    struct Segment* segment;
    uint8_t operand32;
    uint8_t repne;
    uint8_t repe;
};

class CPU
{
public:
    CPU(Board& board);
    ~CPU();

    void tick();
    void reset();

    // Register operations
    uint8_t read_gpreg8(GPRegister reg);
    uint16_t read_gpreg16(GPRegister reg);
    uint32_t read_gpreg32(GPRegister reg);

    void write_gpreg8(GPRegister reg, uint8_t value);
    void write_gpreg16(GPRegister reg, uint16_t value);
    void write_gpreg32(GPRegister reg, uint32_t value);

    uint16_t read_sgreg(SGRegister reg);
    void write_sgreg(SGRegister reg, uint16_t value);

private:
    Board& board;  // Parent board

    CPUState state;

    // General purpose registers
    Register gpregs[num_gpregs];

    // Segment registers + cached values
    Segment sgregs[num_sgregs];

    PrefixState prefixes;

    // Instruction pointer
    Register ip;

    // EFLAGS
    EFLAGS eflags;

    // ModRM byte
    uint8_t modrm;
};
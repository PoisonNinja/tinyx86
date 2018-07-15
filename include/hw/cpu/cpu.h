#pragma once

#include <spdlog/spdlog.h>
#include <tinyx86.h>
#include <cstddef>
#include <cstdint>
#include "decode.h"

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

    void reset();

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

class Board;
class MemoryController;

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
    void write_sgreg(SGRegister reg, uint16_t selector);

    uint16_t read_ip();
    uint32_t read_eip();

    void write_ip(uint16_t value);
    void write_eip(uint32_t value);

    // Memory interface
    uint8_t read_mem8(addr_t addr);
    uint16_t read_mem16(addr_t addr);
    uint32_t read_mem32(addr_t addr);

    void write_mem8(addr_t addr, uint8_t value);
    void write_mem16(addr_t addr, uint16_t value);
    void write_mem32(addr_t addr, uint32_t value);

    // Convenience functions
    uint8_t read_instruction8();
    uint16_t read_instruction16();
    uint32_t read_instruction32();

    void push16(uint16_t value);
    void push32(uint32_t value);

    uint16_t pop16();
    uint32_t pop32();

    addr_t segment_to_linear(SGRegister seg, addr_t offset);

    void halt();
    void stop();

    CPUState get_state();

private:
    Board& board;  // Parent board
    std::shared_ptr<spdlog::logger> log;

    InstructionDecoder decoder;

    CPUState state;

    // General purpose registers
    Register gpregs[num_gpregs];

    // Segment registers + cached values
    Segment sgregs[num_sgregs];

    // Instruction pointer
    Register ip;

    // EFLAGS
    EFLAGS eflags;
};

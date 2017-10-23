#pragma once

#include <tinyx86.h>

struct board;

union cpu_register {
    uint32_t regs_32;
    uint16_t regs_16;
    uint8_t regs_8;
};

struct cpu_segment {
    uint32_t base, limit;
    uint16_t selector;
};

#define CPU_EFLAGS_CF 0x1
#define CPU_EFLAGS_PF 0x4
#define CPU_EFLAGS_AF 0x10
#define CPU_EFLAGS_ZF 0x40
#define CPU_EFLAGS_SF 0x80
#define CPU_EFLAGS_OF 0x800

struct cpu_prefix_state {
    struct cpu_segment* segment;
#define CPU_PREFIX_STATE_OPERAND32(cpu) ((cpu)->prefix_state.operand32 ? 1 : 0)
    uint8_t operand32;
    uint8_t repne;
    uint8_t repe;
};

struct cpu {
    struct board* board;
    // CPU registers
    union cpu_register ax, bx, cx, dx;
    union cpu_register sp, bp, si, di, ip;
    struct cpu_segment cs, ds, ss, es, fs, gs;
    // CPU EFLAGS + required support structures for lazy computation
    uint32_t eflags;
    uint32_t eflags_dirty;
    struct cpu_prefix_state prefix_state;
#define CPU_STOPPED 0x0
#define CPU_RUNNING 0x1
#define CPU_HALTED 0x2
    uint8_t state;
};

extern void cpu_cycle(struct cpu* cpu);

extern void cpu_reset(struct cpu* cpu);

extern struct cpu* cpu_create(void);
extern void cpu_destroy(struct cpu* cpu);

extern void cpu_dump(struct cpu* cpu);

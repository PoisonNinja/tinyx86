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

#define CPU_EFLAGS_CF 0
#define CPU_EFLAGS_PF 2
#define CPU_EFLAGS_AF 4
#define CPU_EFLAGS_ZF 6
#define CPU_EFLAGS_SF 7
#define CPU_EFLAGS_OF 11

// Convert EFLAGS struct to integer for use in binary ops
#define CPU_EFLAGS_AS_INTEGER(eflags) (*(uint32_t*)&(eflags))

struct eflags {
    uint8_t carry : 1;
    uint8_t reserved_1 : 1;
    uint8_t parity : 1;
    uint8_t reserved_2 : 1;
    uint8_t adjust : 1;
    uint8_t reserved_3 : 1;
    uint8_t zero : 1;
    uint8_t sign : 1;
    uint8_t trap : 1;
    uint8_t interrupt : 1;
    uint8_t direction : 1;
    uint8_t overflow : 1;
    uint8_t iopl : 2;
    uint8_t nested_task : 1;
    uint8_t reserved_4 : 1;
    uint8_t resume : 1;
    uint8_t vm : 1;
    uint8_t alignment_check : 1;
    uint8_t virtual_interrupt : 1;
    uint8_t virtual_interrupt_pending : 1;
    uint8_t cpuid : 1;
    uint16_t reserved_5 : 10;
} __attribute__((packed));

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
    struct eflags eflags;
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

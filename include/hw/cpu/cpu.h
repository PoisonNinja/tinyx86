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

struct eflags {
    uint8_t carry;
    uint8_t reserved_1;
    uint8_t parity;
    uint8_t reserved_2;
    uint8_t adjust;
    uint8_t reserved_3;
    uint8_t zero;
    uint8_t sign;
    uint8_t trap;
    uint8_t interrupt;
    uint8_t direction;
    uint8_t overflow;
    uint8_t iopl : 2;
    uint8_t nested_task;
    uint8_t reserved_4;
    uint8_t resume;
    uint8_t vm;
    uint8_t alignment_check;
    uint8_t virtual_interrupt;
    uint8_t virtual_interrupt_pending;
    uint8_t cpuid;
    uint8_t reserved_5[10];
};

struct cpu_prefix_state {
    struct cpu_segment* segment;
    uint8_t operand32;
    uint8_t repne;
    uint8_t repe;
};

struct cpu {
    struct board* board;
    union cpu_register ax, bx, cx, dx;
    union cpu_register sp, bp, si, di, ip;
    struct cpu_segment cs, ds, ss, es, fs, gs;
    struct eflags eflags;
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

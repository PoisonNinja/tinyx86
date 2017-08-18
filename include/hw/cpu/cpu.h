#pragma once

#include <x86emu.h>

union cpu_register {
    uint32_t regs_32;
    uint16_t regs_16;
    uint8_t regs_8;
};

struct cpu_selector {
    uint32_t base, limit;
    uint16_t selector;
};

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
    uint8_t reserved_5[10];
} __attribute((packed));

struct cpu {
    union cpu_register ax, bx, cx, dx;
    union cpu_register sp, bp, si, di, ip;
    struct cpu_register cs, ds, ss, es, fs, gs;
    struct eflags eflags;
};

extern void cpu_reset(struct cpu* cpu);

extern struct cpu* cpu_create(void);
extern void cpu_destroy(struct cpu* cpu);

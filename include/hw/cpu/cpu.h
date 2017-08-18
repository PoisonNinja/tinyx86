#pragma once

#include <x86emu.h>

struct cpu_16bit {
    uint16_t ax, bx, cx, dx;
    uint16_t si, di;
    uint16_t bp, sp;
    uint16_t ip;
};

struct cpu_32bit {
    uint32_t eax, ebx, ecx, edx;
    uint32_t esi, edi;
    uint32_t ebp, esp;
    uint32_t eip;
};

struct cpu_segments {
    uint16_t cs, ds, ss;
    uint16_t es, fs, gs;
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
    struct cpu_16bit registers16;
    struct cpu_32bit registers32;
    struct cpu_segments segments;
    struct eflags eflags;
};

extern void cpu_reset(struct cpu* cpu);

extern struct cpu* cpu_create(void);
extern void cpu_destroy(struct cpu* cpu);

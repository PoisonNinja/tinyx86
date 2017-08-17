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

struct cpu {
    struct cpu_16bit registers16;
    struct cpu_32bit registers32;
    struct cpu_segments segments;
};

struct cpu* cpu_create(void);
void cpu_destroy(struct cpu* cpu);

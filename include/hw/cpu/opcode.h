#pragma once

struct cpu;

struct modrm {
    uint8_t rm : 3;
    uint8_t reg : 3;
    uint8_t mod : 2;
} __attribute__((packed));

struct sib {
    uint8_t base : 3;
    uint8_t index : 3;
    uint8_t scale : 2;
} __attribute__((packed));

extern void opcode_execute(struct cpu* cpu);

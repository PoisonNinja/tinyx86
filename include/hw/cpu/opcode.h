#pragma once

struct cpu;
union cpu_register;

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
extern void opcode_two_execute(struct cpu* cpu);

extern void raw_to_modrm(uint8_t raw, struct modrm* modrm);
extern union cpu_register* modrm_to_register(struct cpu* cpu, uint8_t modrm);
extern uint32_t modrm_to_address(struct cpu* cpu, uint8_t mod, uint8_t rm);

typedef void (*opcode_fn_t)(struct cpu* cpu);

#define OPCODE_DEFINE(number) static void opcode##number(struct cpu* cpu)

extern uint8_t fetch_modrm_r8(struct cpu* cpu);
extern uint8_t fetch_modrm_rm8(struct cpu* cpu);
extern void store_modrm_r8(struct cpu* cpu, uint8_t val);
extern void store_modrm_rm8(struct cpu* cpu, uint8_t val);

extern void far_jump(struct cpu* cpu, uint32_t eip, uint16_t cs);
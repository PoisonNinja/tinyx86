#include <hw/cpu/cpu.h>
#include <hw/cpu/memory.h>
#include <hw/cpu/opcode.h>

typedef void (*opcode_fn_t)(struct cpu* cpu);

#define OPCODE_DEFINE(number) static void opcode##number(struct cpu* cpu)
#define OPCODE_ENTRY(number) &opcode##number

static inline void raw_to_modrm(uint8_t raw, struct modrm* modrm)
{
    uint8_t* tmp = (uint8_t*)modrm;
    *tmp = raw;
}

void push_word(struct cpu* cpu, uint16_t value)
{
    cpu->sp.regs_16 -= 2;
    cpu_store_word(cpu, &cpu->ss, cpu->sp.regs_16, value);
}

/*
 * 0xB8: MOV r16/32, imm16/32
 */
OPCODE_DEFINE(B8)
{
    log_trace("mov ax, imm16");
    uint16_t val = cpu_fetch_instruction_word(cpu);
    cpu->ax.regs_16 = val;
}

/*
 * 0xE8: CALL rel16/32
 */
OPCODE_DEFINE(E8)
{
    log_trace("call rel16/32");
    int16_t offset = cpu_fetch_instruction_word(cpu);
    uint16_t eip = cpu->ip.regs_16 + offset;
    push_word(cpu, cpu->ip.regs_16);
    cpu->ip.regs_16 = eip;
}

opcode_fn_t opcode_table[256] = {
    NULL, NULL, NULL, NULL, NULL,     NULL, NULL, NULL, NULL, NULL, NULL, NULL,
    NULL, NULL, NULL, NULL, NULL,     NULL, NULL, NULL, NULL, NULL, NULL, NULL,
    NULL, NULL, NULL, NULL, NULL,     NULL, NULL, NULL, NULL, NULL, NULL, NULL,
    NULL, NULL, NULL, NULL, NULL,     NULL, NULL, NULL, NULL, NULL, NULL, NULL,
    NULL, NULL, NULL, NULL, NULL,     NULL, NULL, NULL, NULL, NULL, NULL, NULL,
    NULL, NULL, NULL, NULL, NULL,     NULL, NULL, NULL, NULL, NULL, NULL, NULL,
    NULL, NULL, NULL, NULL, NULL,     NULL, NULL, NULL, NULL, NULL, NULL, NULL,
    NULL, NULL, NULL, NULL, NULL,     NULL, NULL, NULL, NULL, NULL, NULL, NULL,
    NULL, NULL, NULL, NULL, NULL,     NULL, NULL, NULL, NULL, NULL, NULL, NULL,
    NULL, NULL, NULL, NULL, NULL,     NULL, NULL, NULL, NULL, NULL, NULL, NULL,
    NULL, NULL, NULL, NULL, NULL,     NULL, NULL, NULL, NULL, NULL, NULL, NULL,
    NULL, NULL, NULL, NULL, NULL,     NULL, NULL, NULL, NULL, NULL, NULL, NULL,
    NULL, NULL, NULL, NULL, NULL,     NULL, NULL, NULL, NULL, NULL, NULL, NULL,
    NULL, NULL, NULL, NULL, NULL,     NULL, NULL, NULL, NULL, NULL, NULL, NULL,
    NULL, NULL, NULL, NULL, NULL,     NULL, NULL, NULL, NULL, NULL, NULL, NULL,
    NULL, NULL, NULL, NULL, opcodeB8, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
    NULL, NULL, NULL, NULL, NULL,     NULL, NULL, NULL, NULL, NULL, NULL, NULL,
    NULL, NULL, NULL, NULL, NULL,     NULL, NULL, NULL, NULL, NULL, NULL, NULL,
    NULL, NULL, NULL, NULL, NULL,     NULL, NULL, NULL, NULL, NULL, NULL, NULL,
    NULL, NULL, NULL, NULL, opcodeE8, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
    NULL, NULL, NULL, NULL, NULL,     NULL, NULL, NULL, NULL, NULL, NULL, NULL,
    NULL, NULL, NULL, NULL,
};

void opcode_execute(struct cpu* cpu)
{
    uint8_t opcode = cpu_fetch_instruction_byte(cpu);
    if (opcode == 0x0F) {
        log_fatal("We do not support two byte opcodes yet!");
    }
    log_trace("Opcode: %X", opcode);
    if (!opcode_table[opcode]) {
        log_fatal("Unknown opcode, should probably throw an Invalid Opcode "
                  "exception, got %X",
                  opcode);
    }
    opcode_table[opcode](cpu);
}

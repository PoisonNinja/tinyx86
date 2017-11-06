#include <hw/cpu/arithmetic.h>
#include <hw/cpu/cpu.h>
#include <hw/cpu/memory.h>
#include <hw/cpu/opcode.h>

/*
 * 0x85: jnz rel16/32
 */
OPCODE_DEFINE(85)
{
    log_trace("jnz rel16/32");
    if (CPU_PREFIX_STATE_OPERAND32(cpu)) {
        int32_t offset = cpu_fetch_instruction_u32(cpu);
        if (!cpu_get_zf(cpu)) {
            cpu->ip.regs_32 += offset;
        }
    } else {
        int16_t offset = cpu_fetch_instruction_u16(cpu);
        if (!cpu_get_zf(cpu)) {
            cpu->ip.regs_16 += offset;
        }
    }
}

static opcode_fn_t opcode_table[256] = {
    NULL, NULL,     NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
    NULL, NULL,     NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
    NULL, NULL,     NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
    NULL, NULL,     NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
    NULL, NULL,     NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
    NULL, NULL,     NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
    NULL, NULL,     NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
    NULL, NULL,     NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
    NULL, NULL,     NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
    NULL, NULL,     NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
    NULL, NULL,     NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
    NULL, opcode85, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
    NULL, NULL,     NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
    NULL, NULL,     NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
    NULL, NULL,     NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
    NULL, NULL,     NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
    NULL, NULL,     NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
    NULL, NULL,     NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
    NULL, NULL,     NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
    NULL, NULL,     NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
    NULL, NULL,     NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
    NULL, NULL,     NULL, NULL,
};

void opcode_two_execute(struct cpu* cpu)
{
    uint8_t opcode = cpu_fetch_instruction_u8(cpu);
    log_trace("Two-byte opcode: %X", opcode);
    if (!opcode_table[opcode]) {
        log_fatal("Unknown opcode, should probably throw an Invalid Opcode "
                  "exception, got opcode 0x%X at IP 0x%X",
                  opcode, cpu->cs.base + cpu->ip.regs_16 - 1);
        cpu->state = CPU_HALTED;
        return;
    }
    opcode_table[opcode](cpu);
}

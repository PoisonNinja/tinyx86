#include <hw/cpu/cpu.h>
#include <hw/cpu/memory.h>
#include <hw/cpu/opcode.h>

void opcode_execute(struct cpu* cpu)
{
    uint16_t ip = cpu->ip.regs_16;
    uint8_t opcode = memory_read_byte(cpu->board, ip);
}

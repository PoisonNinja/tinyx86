#include <hw/cpu/cpu.h>
#include <hw/cpu/memory.h>
#include <hw/cpu/opcode.h>

void opcode_execute(struct cpu* cpu)
{
    uint8_t opcode = cpu_fetch_instruction_byte(cpu);
}

#include <board.h>
#include <hw/chipset/memory.h>
#include <hw/cpu/cpu.h>
#include <hw/cpu/memory.h>
#include <x86emu.h>

uint8_t cpu_fetch_instruction_byte(struct cpu* cpu)
{
    addr_t final = cpu->cs.base + cpu->ip.regs_16;
    uint8_t ret = memory_read_byte(cpu->board, final);
    cpu->ip.regs_16++;
    return ret;
}

uint8_t cpu_fetch_data_byte(struct cpu* cpu, addr_t offset)
{
    addr_t final = cpu->ds.base + offset;
    return memory_read_byte(cpu->board, final);
}

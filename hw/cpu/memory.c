#include <board.h>
#include <hw/chipset/memory.h>
#include <hw/cpu/cpu.h>
#include <hw/cpu/memory.h>

uint8_t cpu_fetch_data_byte(struct cpu* cpu, addr_t offset)
{
    addr_t final = cpu->ds.base + offset;
    return memory_read_byte(cpu->board, final);
}

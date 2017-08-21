#include <board.h>
#include <hw/chipset/memory.h>
#include <hw/cpu/cpu.h>
#include <hw/cpu/memory.h>
#include <tinyx86.h>

uint8_t cpu_fetch_instruction_byte(struct cpu* cpu)
{
    addr_t final = cpu->cs.base + cpu->ip.regs_16;
    uint8_t ret = memory_read_byte(cpu->board, final);
    cpu->ip.regs_16++;
    return ret;
}

uint16_t cpu_fetch_instruction_word(struct cpu* cpu)
{
    addr_t final = cpu->cs.base + cpu->ip.regs_16;
    uint16_t ret = memory_read_word(cpu->board, final);
    cpu->ip.regs_16 += 2;
    return ret;
}

uint8_t cpu_fetch_byte(struct cpu* cpu, struct cpu_segment* segment,
                       addr_t offset)
{
    addr_t final = segment->base + offset;
    return memory_read_byte(cpu->board, final);
}

void cpu_store_byte(struct cpu* cpu, struct cpu_segment* segment, addr_t offset,
                    uint8_t value)
{
    addr_t final = segment->base + offset;
    return memory_write_byte(cpu->board, final, value);
}

uint16_t cpu_fetch_word(struct cpu* cpu, struct cpu_segment* segment,
                        addr_t offset)
{
    addr_t final = segment->base + offset;
    return memory_read_word(cpu->board, final);
}

void cpu_store_word(struct cpu* cpu, struct cpu_segment* segment, addr_t offset,
                    uint16_t value)
{
    addr_t final = segment->base + offset;
    return memory_write_word(cpu->board, final, value);
}

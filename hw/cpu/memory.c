#include <hw/board.h>
#include <hw/chipset/memory.h>
#include <hw/cpu/cpu.h>
#include <hw/cpu/memory.h>
#include <tinyx86.h>

uint8_t cpu_fetch_instruction_u8(struct cpu* cpu)
{
    addr_t final = cpu->cs.base + cpu->ip.regs_16;
    uint8_t ret = memory_read_u8(cpu->board, final);
    cpu->ip.regs_16++;
    return ret;
}

uint16_t cpu_fetch_instruction_u16(struct cpu* cpu)
{
    addr_t final = cpu->cs.base + cpu->ip.regs_16;
    uint16_t ret = memory_read_u16(cpu->board, final);
    cpu->ip.regs_16 += 2;
    return ret;
}

uint32_t cpu_fetch_instruction_u32(struct cpu* cpu)
{
    addr_t final = cpu->cs.base + cpu->ip.regs_32;
    uint16_t ret = memory_read_u32(cpu->board, final);
    cpu->ip.regs_32 += 4;
    return ret;
}

uint8_t cpu_fetch_u8(struct cpu* cpu, struct cpu_segment* segment,
                     addr_t offset)
{
    addr_t final = segment->base + offset;
    return memory_read_u8(cpu->board, final);
}

void cpu_store_u8(struct cpu* cpu, struct cpu_segment* segment, addr_t offset,
                  uint8_t value)
{
    addr_t final = segment->base + offset;
    return memory_write_u8(cpu->board, final, value);
}

uint16_t cpu_fetch_u16(struct cpu* cpu, struct cpu_segment* segment,
                       addr_t offset)
{
    addr_t final = segment->base + offset;
    return memory_read_u16(cpu->board, final);
}

void cpu_store_u16(struct cpu* cpu, struct cpu_segment* segment, addr_t offset,
                   uint16_t value)
{
    addr_t final = segment->base + offset;
    return memory_write_u16(cpu->board, final, value);
}

uint32_t cpu_fetch_u32(struct cpu* cpu, struct cpu_segment* segment,
                       addr_t offset)
{
    addr_t final = segment->base + offset;
    return memory_read_u32(cpu->board, final);
}

void cpu_store_u32(struct cpu* cpu, struct cpu_segment* segment, addr_t offset,
                   uint32_t value)
{
    addr_t final = segment->base + offset;
    return memory_write_u32(cpu->board, final, value);
}

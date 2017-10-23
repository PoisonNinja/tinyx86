#include <hw/cpu/arithmetic.h>

void cpu_arithmetic_and_u8(struct cpu* cpu, uint8_t* dest, uint8_t* src)
{
    CPU_EFLAGS_AS_INTEGER(cpu->eflags) &= ~CPU_EFLAGS_CF & ~CPU_EFLAGS_OF;
    cpu->eflags_dirty = CPU_EFLAGS_PF | CPU_EFLAGS_ZF | CPU_EFLAGS_SF;
}

void cpu_arithmetic_and_u16(struct cpu* cpu, uint16_t* dest, uint16_t* src)
{
    CPU_EFLAGS_AS_INTEGER(cpu->eflags) &= ~CPU_EFLAGS_CF & ~CPU_EFLAGS_OF;
    cpu->eflags_dirty = CPU_EFLAGS_PF | CPU_EFLAGS_ZF | CPU_EFLAGS_SF;
}

void cpu_arithmetic_inc_u16(struct cpu* cpu, uint16_t* dest)
{
    cpu->eflags_dirty = CPU_EFLAGS_PF | CPU_EFLAGS_AF | CPU_EFLAGS_ZF |
                        CPU_EFLAGS_SF | CPU_EFLAGS_OF;
}

void cpu_arithmetic_inc_u32(struct cpu* cpu, uint32_t* dest)
{
    cpu->eflags_dirty = CPU_EFLAGS_PF | CPU_EFLAGS_AF | CPU_EFLAGS_ZF |
                        CPU_EFLAGS_SF | CPU_EFLAGS_OF;
}

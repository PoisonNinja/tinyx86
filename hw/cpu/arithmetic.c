#include <hw/cpu/arithmetic.h>

void cpu_arithmetic_and_u8(struct cpu* cpu, uint8_t* dest, uint8_t* src)
{
    cpu->last_size = CPU_OPERAND_SIZE_8;
    cpu->eflags &= ~CPU_EFLAGS_CF & ~CPU_EFLAGS_OF;
    cpu->eflags_dirty = CPU_EFLAGS_PF | CPU_EFLAGS_ZF | CPU_EFLAGS_SF;
}

void cpu_arithmetic_and_u16(struct cpu* cpu, uint16_t* dest, uint16_t* src)
{
    cpu->last_size = CPU_OPERAND_SIZE_16;
    cpu->eflags &= ~CPU_EFLAGS_CF & ~CPU_EFLAGS_OF;
    cpu->eflags_dirty = CPU_EFLAGS_PF | CPU_EFLAGS_ZF | CPU_EFLAGS_SF;
}

void cpu_arithmetic_inc_u16(struct cpu* cpu, uint16_t* dest)
{
    cpu->last_op1 = *dest;
    cpu->last_op2 = 1;
    *dest += 1;
    cpu->last_size = CPU_OPERAND_SIZE_16;
    cpu->eflags_dirty = CPU_EFLAGS_PF | CPU_EFLAGS_AF | CPU_EFLAGS_ZF |
                        CPU_EFLAGS_SF | CPU_EFLAGS_OF;
}

void cpu_arithmetic_inc_u32(struct cpu* cpu, uint32_t* dest)
{
    cpu->last_op1 = *dest;
    cpu->last_op2 = 1;
    *dest += 1;
    cpu->last_size = CPU_OPERAND_SIZE_32;
    cpu->eflags_dirty = CPU_EFLAGS_PF | CPU_EFLAGS_AF | CPU_EFLAGS_ZF |
                        CPU_EFLAGS_SF | CPU_EFLAGS_OF;
}

bool cpu_get_cf(struct cpu* cpu)
{
    if (cpu->eflags_dirty & CPU_EFLAGS_CF) {
        cpu->eflags_dirty &= ~CPU_EFLAGS_CF;
    } else {
        return cpu->eflags & CPU_EFLAGS_CF;
    }
}

bool cpu_get_zf(struct cpu* cpu)
{
    if (cpu->eflags_dirty & CPU_EFLAGS_ZF) {
        cpu->eflags_dirty &= ~CPU_EFLAGS_ZF;
    } else {
        return cpu->eflags & CPU_EFLAGS_ZF;
    }
}

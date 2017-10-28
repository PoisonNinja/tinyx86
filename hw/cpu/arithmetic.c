#include <hw/cpu/arithmetic.h>

// clang-format off
// clang-format tries to move the values all the way to the left
#define CPU_ARITHMETIC_OP_ADD_U8    0x1
#define CPU_ARITHMETIC_OP_ADD_U16   0x2
#define CPU_ARITHMETIC_OP_ADD_U32   0x3
#define CPU_ARITHMETIC_OP_ADC_U8    0x4
#define CPU_ARITHMETIC_OP_ADC_U16   0x5
#define CPU_ARITHMETIC_OP_ADC_U32   0x6
#define CPU_ARITHMETIC_OP_SUB_U8    0x7
#define CPU_ARITHMETIC_OP_SUB_U16   0x8
#define CPU_ARITHMETIC_OP_SUB_U32   0x9
#define CPU_ARITHMETIC_OP_SBB_U8    0xA
#define CPU_ARITHMETIC_OP_SBB_U16   0xB
#define CPU_ARITHMETIC_OP_SBB_U32   0xC
#define CPU_ARITHMETIC_OP_AND_U8    0xD
#define CPU_ARITHMETIC_OP_AND_U16   0xE
#define CPU_ARITHMETIC_OP_AND_U32   0xF
#define CPU_ARITHMETIC_OP_OR_U8     0x10
#define CPU_ARITHMETIC_OP_OR_U16    0x11
#define CPU_ARITHMETIC_OP_OR_U32    0x12
#define CPU_ARITHMETIC_OP_XOR_U8    0x13
#define CPU_ARITHMETIC_OP_XOR_U16   0x14
#define CPU_ARITHMETIC_OP_XOR_U32   0x15
#define CPU_ARITHMETIC_OP_CMP_U8    0x16
#define CPU_ARITHMETIC_OP_CMP_U16   0x17
#define CPU_ARITHMETIC_OP_CMP_U32   0x18
#define CPU_ARITHMETIC_OP_TEST_U8   0x19
#define CPU_ARITHMETIC_OP_TEST_U16  0x1A
#define CPU_ARITHMETIC_OP_TEST_U32  0x1B
// clang-format on

void cpu_arithmetic_and_u8(struct cpu* cpu, uint8_t* dest, uint8_t* src)
{
    cpu->eflags &= ~CPU_EFLAGS_CF & ~CPU_EFLAGS_OF;
    cpu->eflags_dirty = CPU_EFLAGS_PF | CPU_EFLAGS_ZF | CPU_EFLAGS_SF;
}

void cpu_arithmetic_and_u16(struct cpu* cpu, uint16_t* dest, uint16_t* src)
{
    cpu->eflags &= ~CPU_EFLAGS_CF & ~CPU_EFLAGS_OF;
    cpu->eflags_dirty = CPU_EFLAGS_PF | CPU_EFLAGS_ZF | CPU_EFLAGS_SF;
}

void cpu_arithmetic_inc_u16(struct cpu* cpu, uint16_t* dest)
{
    cpu->last_op1 = *dest;
    cpu->last_op2 = 1;
    *dest += 1;
    cpu->eflags_dirty = CPU_EFLAGS_PF | CPU_EFLAGS_AF | CPU_EFLAGS_ZF |
                        CPU_EFLAGS_SF | CPU_EFLAGS_OF;
}

void cpu_arithmetic_inc_u32(struct cpu* cpu, uint32_t* dest)
{
    cpu->last_op1 = *dest;
    cpu->last_op2 = 1;
    *dest += 1;
    cpu->eflags_dirty = CPU_EFLAGS_PF | CPU_EFLAGS_AF | CPU_EFLAGS_ZF |
                        CPU_EFLAGS_SF | CPU_EFLAGS_OF;
}

bool cpu_get_cf(struct cpu* cpu)
{
    if (cpu->eflags_dirty & CPU_EFLAGS_CF) {
        cpu->eflags_dirty &= ~CPU_EFLAGS_CF;
    }
    return cpu->eflags & CPU_EFLAGS_CF;
}

bool cpu_get_pf(struct cpu* cpu)
{
    if (cpu->eflags_dirty & CPU_EFLAGS_PF) {
        uint8_t v = cpu->last_result & 0xFF;  // Low 8 bits
        v ^= v >> 1;
        v ^= v >> 2;
        v = (v & 0x11111111U) * 0x11111111U;
        cpu->eflags |= (!((v >> 28) & 1)) ? CPU_EFLAGS_PF : 0;
        cpu->eflags_dirty &= ~CPU_EFLAGS_PF;
    }
    return cpu->eflags & CPU_EFLAGS_PF;
}

bool cpu_get_zf(struct cpu* cpu)
{
    if (cpu->eflags_dirty & CPU_EFLAGS_ZF) {
        cpu->eflags |= (cpu->last_result) ? CPU_EFLAGS_ZF : 0;
        cpu->eflags_dirty &= ~CPU_EFLAGS_ZF;
    }
    return cpu->eflags & CPU_EFLAGS_ZF;
}

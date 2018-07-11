#include <hw/cpu/arithmetic.h>

void cpu_arithmetic_add_u8(struct cpu* cpu, uint8_t* dest, uint8_t* src)
{
    cpu->last_op1 = *dest;
    cpu->last_op2 = *src;
    cpu->last_result = *dest = *dest + *src;
    cpu->last_size = CPU_OP_SIZE_8;
    cpu->eflags_dirty = CPU_EFLAGS_ALL;
}

void cpu_arithmetic_add_u16(struct cpu* cpu, uint16_t* dest, uint16_t* src)
{
    cpu->last_op1 = *dest;
    cpu->last_op2 = *src;
    cpu->last_result = *dest = *dest + *src;
    cpu->last_size = CPU_OP_SIZE_16;
    cpu->eflags_dirty = CPU_EFLAGS_ALL;
}

void cpu_arithmetic_add_u32(struct cpu* cpu, uint32_t* dest, uint32_t* src)
{
    cpu->last_op1 = *dest;
    cpu->last_op2 = *src;
    cpu->last_result = *dest = *dest + *src;
    cpu->last_size = CPU_OP_SIZE_32;
    cpu->eflags_dirty = CPU_EFLAGS_ALL;
}

void cpu_arithmetic_sub_u8(struct cpu* cpu, uint8_t* dest, uint8_t* src)
{
    cpu->last_result = *dest;
    cpu->last_op2 = *src;
    cpu->last_op1 = *dest = *dest - *src;
    cpu->last_size = CPU_OP_SIZE_8;
    cpu->eflags_dirty = CPU_EFLAGS_ALL;
}

void cpu_arithmetic_sub_u16(struct cpu* cpu, uint16_t* dest, uint16_t* src)
{
    cpu->last_result = *dest;
    cpu->last_op2 = *src;
    cpu->last_op1 = *dest = *dest - *src;
    cpu->last_size = CPU_OP_SIZE_16;
    cpu->eflags_dirty = CPU_EFLAGS_ALL;
}

void cpu_arithmetic_sub_u32(struct cpu* cpu, uint32_t* dest, uint32_t* src)
{
    cpu->last_result = *dest;
    cpu->last_op2 = *src;
    cpu->last_op1 = *dest = *dest - *src;
    cpu->last_size = CPU_OP_SIZE_32;
    cpu->eflags_dirty = CPU_EFLAGS_ALL;
}

void cpu_arithmetic_and_u8(struct cpu* cpu, uint8_t* dest, uint8_t* src)
{
    cpu->last_op1 = *dest;
    cpu->last_op2 = *src;
    cpu->last_result = *dest = *dest & *src;
    cpu->last_size = CPU_OP_SIZE_8;
    cpu->eflags &= ~CPU_EFLAGS_CF & ~CPU_EFLAGS_OF;
    cpu->eflags_dirty = CPU_EFLAGS_PF | CPU_EFLAGS_ZF | CPU_EFLAGS_SF;
}

void cpu_arithmetic_and_u16(struct cpu* cpu, uint16_t* dest, uint16_t* src)
{
    cpu->last_op1 = *dest;
    cpu->last_op2 = *src;
    cpu->last_result = *dest = *dest & *src;
    cpu->last_size = CPU_OP_SIZE_16;
    cpu->eflags &= ~CPU_EFLAGS_CF & ~CPU_EFLAGS_OF;
    cpu->eflags_dirty = CPU_EFLAGS_PF | CPU_EFLAGS_ZF | CPU_EFLAGS_SF;
}

void cpu_arithmetic_and_u32(struct cpu* cpu, uint16_t* dest, uint16_t* src)
{
    cpu->last_op1 = *dest;
    cpu->last_op2 = *src;
    cpu->last_result = *dest = *dest & *src;
    cpu->last_size = CPU_OP_SIZE_32;
    cpu->eflags &= ~CPU_EFLAGS_CF & ~CPU_EFLAGS_OF;
    cpu->eflags_dirty = CPU_EFLAGS_PF | CPU_EFLAGS_ZF | CPU_EFLAGS_SF;
}

void cpu_arithmetic_inc_u16(struct cpu* cpu, uint16_t* dest)
{
    cpu->last_op1 = *dest;
    cpu->last_op2 = 1;
    cpu->last_result = ++*dest;
    cpu->last_size = CPU_OP_SIZE_16;
    cpu->eflags_dirty = CPU_EFLAGS_PF | CPU_EFLAGS_AF | CPU_EFLAGS_ZF |
                        CPU_EFLAGS_SF | CPU_EFLAGS_OF;
}

void cpu_arithmetic_inc_u32(struct cpu* cpu, uint32_t* dest)
{
    cpu->last_op1 = *dest;
    cpu->last_op2 = 1;
    cpu->last_result = ++*dest;
    cpu->last_size = CPU_OP_SIZE_32;
    cpu->eflags_dirty = CPU_EFLAGS_PF | CPU_EFLAGS_AF | CPU_EFLAGS_ZF |
                        CPU_EFLAGS_SF | CPU_EFLAGS_OF;
}

/*
 * The following flag calculations are largely borrowed from v86 by
 * Fabian Hemmer. The code is licensed under the Simplified BSD license.
 * It has been reproduced here:
 *
 * Copyright (c) 2012-2014, Fabian Hemmer
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 * The views and conclusions contained in the software and documentation are
 * those of the authors and should not be interpreted as representing official
 * policies, either expressed or implied, of the FreeBSD Project.
 */
bool cpu_get_cf(struct cpu* cpu)
{
    if (cpu->eflags_dirty & CPU_EFLAGS_CF) {
        cpu->eflags |=
            ((cpu->last_op1 ^ ((cpu->last_op1 ^ cpu->last_op2) &
                               (cpu->last_op2 ^ cpu->last_result))) >>
                 cpu->last_size &
             1) ?
                CPU_EFLAGS_CF :
                0;
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

bool cpu_get_af(struct cpu* cpu)
{
    if (cpu->eflags_dirty & CPU_EFLAGS_AF) {
        cpu->eflags |=
            ((cpu->last_op1 ^ cpu->last_op2 ^ cpu->last_result) & 16) ?
                CPU_EFLAGS_AF :
                0;
        cpu->eflags_dirty &= ~CPU_EFLAGS_AF;
    }
    return cpu->eflags & CPU_EFLAGS_AF;
}

bool cpu_get_zf(struct cpu* cpu)
{
    if (cpu->eflags_dirty & CPU_EFLAGS_ZF) {
        cpu->eflags |= (cpu->last_result) ? 0 : CPU_EFLAGS_ZF;
        cpu->eflags_dirty &= ~CPU_EFLAGS_ZF;
    }
    return cpu->eflags & CPU_EFLAGS_ZF;
}

bool cpu_get_sf(struct cpu* cpu)
{
    if (cpu->eflags_dirty & CPU_EFLAGS_SF) {
        cpu->eflags |=
            (cpu->last_result >> cpu->last_size & 1) ? CPU_EFLAGS_SF : 0;
        cpu->eflags_dirty &= ~CPU_EFLAGS_ZF;
    }
    return cpu->eflags & CPU_EFLAGS_SF;
}

bool cpu_get_of(struct cpu* cpu)
{
    if (cpu->eflags_dirty & CPU_EFLAGS_OF) {
        cpu->eflags |= (((cpu->last_op1 ^ cpu->last_result) &
                         (cpu->last_op2 ^ cpu->last_result)) >>
                            cpu->last_size &
                        1) ?
                           CPU_EFLAGS_OF :
                           0;
        cpu->eflags_dirty &= ~CPU_EFLAGS_OF;
    }
    return cpu->eflags & CPU_EFLAGS_OF;
}

void cpu_recompute_flags(struct cpu* cpu)
{
    cpu_get_cf(cpu);
    cpu_get_pf(cpu);
    cpu_get_af(cpu);
    cpu_get_zf(cpu);
    cpu_get_sf(cpu);
    cpu_get_of(cpu);
}

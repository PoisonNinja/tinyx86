#pragma once

#include <hw/cpu/cpu.h>
#include <tinyx86.h>

extern void cpu_arithmetic_and_u8(struct cpu* cpu, uint8_t* dest, uint8_t* src);
extern void cpu_arithmetic_and_u16(struct cpu* cpu, uint16_t* dest,
                                   uint16_t* src);

extern void cpu_arithmetic_inc_u16(struct cpu* cpu, uint16_t* dest);
extern void cpu_arithmetic_inc_u32(struct cpu* cpu, uint32_t* dest);

extern bool cpu_get_cf(struct cpu* cpu);
extern bool cpu_get_af(struct cpu* cpu);
extern bool cpu_get_zf(struct cpu* cpu);
extern bool cpu_get_of(struct cpu* cpu);

extern void cpu_recompute_flags(struct cpu* cpu);

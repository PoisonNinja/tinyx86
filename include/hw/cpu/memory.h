#pragma once

#include <tinyx86.h>

extern uint8_t cpu_fetch_instruction_u8(struct cpu* cpu);
extern uint16_t cpu_fetch_instruction_u16(struct cpu* cpu);
extern uint32_t cpu_fetch_instruction_u32(struct cpu* cpu);

extern uint8_t cpu_fetch_u8(struct cpu* cpu, struct cpu_segment* segment,
                            addr_t offset);
extern void cpu_store_u8(struct cpu* cpu, struct cpu_segment* segment,
                         addr_t offset, uint8_t value);

extern uint16_t cpu_fetch_u16(struct cpu* cpu, struct cpu_segment* segment,
                              addr_t offset);
extern void cpu_store_u16(struct cpu* cpu, struct cpu_segment* segment,
                          addr_t offset, uint16_t value);

extern uint32_t cpu_fetch_u32(struct cpu* cpu, struct cpu_segment* segment,
                              addr_t offset);
extern void cpu_store_u32(struct cpu* cpu, struct cpu_segment* segment,
                          addr_t offset, uint32_t value);

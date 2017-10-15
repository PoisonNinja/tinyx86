#pragma once

#include <tinyx86.h>

extern uint8_t cpu_fetch_instruction_byte(struct cpu* cpu);
extern uint16_t cpu_fetch_instruction_word(struct cpu* cpu);
extern uint32_t cpu_fetch_instruction_long(struct cpu* cpu);

extern uint8_t cpu_fetch_byte(struct cpu* cpu, struct cpu_segment* segment,
                              addr_t offset);
extern void cpu_store_byte(struct cpu* cpu, struct cpu_segment* segment,
                           addr_t offset, uint8_t value);

extern uint16_t cpu_fetch_word(struct cpu* cpu, struct cpu_segment* segment,
                               addr_t offset);
extern void cpu_store_word(struct cpu* cpu, struct cpu_segment* segment,
                           addr_t offset, uint16_t value);

extern uint32_t cpu_fetch_long(struct cpu* cpu, struct cpu_segment* segment,
                               addr_t offset);
extern void cpu_store_long(struct cpu* cpu, struct cpu_segment* segment,
                           addr_t offset, uint32_t value);

#pragma once

#include <x86emu.h>

extern uint8_t cpu_fetch_instruction_byte(struct cpu* cpu);
extern uint8_t cpu_fetch_data_byte(struct cpu* cpu, addr_t offset);

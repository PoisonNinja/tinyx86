#pragma once

#include <tinyx86.h>

extern uint8_t ram_read_byte(struct memory_region* region, addr_t addr);
extern void ram_write_byte(struct memory_region* region, addr_t addr,
                           uint8_t value);

extern uint16_t ram_read_word(struct memory_region* region, addr_t addr);
extern void ram_write_word(struct memory_region* region, addr_t addr,
                           uint16_t value);

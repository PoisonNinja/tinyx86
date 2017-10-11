#pragma once

#include <tinyx86.h>

extern void memory_init_alias(struct board* board, addr_t base, size_t size,
                              void* host);

extern uint8_t alias_read_byte(struct memory_region* region, addr_t addr);
extern void alias_write_byte(struct memory_region* region, addr_t addr,
                             uint8_t value);

extern uint16_t alias_read_word(struct memory_region* region, addr_t addr);
extern void alias_write_word(struct memory_region* region, addr_t addr,
                             uint16_t value);

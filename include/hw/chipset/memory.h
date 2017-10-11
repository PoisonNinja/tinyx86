#pragma once

#include <tinyx86.h>

struct board;

extern uint8_t memory_read_byte(struct board* board, addr_t addr);
extern void memory_write_byte(struct board* board, addr_t addr, uint8_t value);

extern uint16_t memory_read_word(struct board* board, addr_t addr);
extern void memory_write_word(struct board* board, addr_t addr, uint16_t value);

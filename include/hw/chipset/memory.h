#pragma once

#include <list.h>
#include <x86emu.h>

struct board;

struct mmio {
    addr_t base;
    addr_t extent;
    uint64_t (*read)(addr_t address, size_t size);
    void (*write)(addr_t address, uint64_t data, size_t size);
    struct list_element list;
};

extern void mmio_register(struct mmio* mmio);

extern uint8_t memory_read_byte(struct board* board, addr_t addr);
extern void memory_write_byte(struct board* board, addr_t addr, uint8_t value);

extern uint16_t memory_read_word(struct board* board, addr_t addr);
extern void memory_write_word(struct board* board, addr_t addr, uint16_t value);

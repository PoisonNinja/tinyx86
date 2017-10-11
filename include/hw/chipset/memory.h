#pragma once

#include <list.h>
#include <tinyx86.h>

struct memory_region {
    addr_t base;
    addr_t size;
    void* data;  // Specific data types
    struct list_element list;
};

struct memory {
    struct list_element mmio_regions;
    struct list_element rom_regions;
    struct list_element alias_regions;
    struct list_element ram_regions;
};

struct board;

extern struct memory* memory_create(void);

extern void memory_init_ram(struct board* board, addr_t base, size_t size);

extern uint8_t memory_read_byte(struct board* board, addr_t addr);
extern void memory_write_byte(struct board* board, addr_t addr, uint8_t value);

extern uint16_t memory_read_word(struct board* board, addr_t addr);
extern void memory_write_word(struct board* board, addr_t addr, uint16_t value);

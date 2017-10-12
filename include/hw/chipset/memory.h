#pragma once

#include <list.h>
#include <tinyx86.h>

struct memory_region {
    addr_t base;
    void* host_base;
    addr_t size;
    struct list_element list;
};

struct memory {
    struct list_element mmio_regions;
    struct list_element rom_regions;
    struct list_element ram_regions;
};

struct board;

extern struct memory* memory_create(void);

extern uint8_t memory_read_byte(struct board* board, addr_t addr);
extern void memory_write_byte(struct board* board, addr_t addr, uint8_t value);

extern uint16_t memory_read_word(struct board* board, addr_t addr);
extern void memory_write_word(struct board* board, addr_t addr, uint16_t value);

extern int memory_load_image(struct memory_region* region, void* blob,
                             addr_t offset, size_t size);

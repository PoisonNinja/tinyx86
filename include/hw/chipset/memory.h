#pragma once

#include <list.h>
#include <tinyx86.h>

struct memory_region;

struct memory_region_operations {
    uint8_t (*read_byte)(struct memory_region* region, addr_t addr);
    void (*write_byte)(struct memory_region* region, addr_t addr,
                       uint8_t value);
    uint16_t (*read_word)(struct memory_region* region, addr_t addr);
    void (*write_word)(struct memory_region* region, addr_t addr,
                       uint16_t value);
    uint32_t (*read_long)(struct memory_region* region, addr_t addr);
    void (*write_long)(struct memory_region* region, addr_t addr,
                       uint32_t value);
};

struct memory_region {
    addr_t base;
    void* host_base;
    addr_t size;
    struct memory_region_operations* memory_region_ops;
    struct list_element subregions;
    struct list_element list;
};

struct memory {
    struct list_element regions;
};

struct board;

extern struct memory* memory_create(void);

extern uint8_t memory_read_byte(struct board* board, addr_t addr);
extern void memory_write_byte(struct board* board, addr_t addr, uint8_t value);

extern uint16_t memory_read_word(struct board* board, addr_t addr);
extern void memory_write_word(struct board* board, addr_t addr, uint16_t value);

extern uint32_t memory_read_long(struct board* board, addr_t addr);
extern void memory_write_long(struct board* board, addr_t addr, uint32_t value);

extern void memory_register_region(struct board* board,
                                   struct memory_region* region);

extern int memory_load_image(struct memory_region* region, void* blob,
                             addr_t offset, size_t size);

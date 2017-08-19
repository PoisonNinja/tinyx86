#pragma once

#include <x86emu.h>

struct cpu;

struct board {
    struct cpu* cpu;
    uintptr_t memory_base;
    size_t memory_extent;
};

extern struct board* board_create(size_t memory);
extern void board_poweron(struct board* board);
extern void board_run(struct board* board);
extern void board_load(struct board* board, addr_t load, void* blob,
                       size_t size);

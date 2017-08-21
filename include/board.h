#pragma once

#include <hw/cpu/cpu.h>
#include <x86emu.h>

struct board {
    struct cpu* cpu;
    uintptr_t memory_base;
    size_t memory_extent;
};

extern struct board* board_create(size_t memory);
extern void board_destroy(struct board* board);
extern void board_poweron(struct board* board);
extern void board_poweroff(struct board* board);
extern void board_run(struct board* board);
extern void board_load(struct board* board, addr_t load, void* blob,
                       size_t size);

#pragma once

#include <hw/chipset/memory.h>
#include <hw/cpu/cpu.h>
#include <tinyx86.h>

struct board {
    struct cpu* cpu;
    struct memory* memory;
};

extern struct board* board_create(size_t memory);
extern void board_destroy(struct board* board);
extern void board_poweron(struct board* board);
extern void board_poweroff(struct board* board);
extern void board_run(struct board* board);
extern void board_load(struct board* board, addr_t load, void* blob,
                       size_t size);

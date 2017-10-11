#include <hw/board.h>
#include <stdlib.h>
#include <string.h>

#define MB_TO_BYTE(x) ((x)*1024 * 1024)

extern void rom_load(void);

struct board* board_create(size_t memory)
{
    struct board* board = malloc(sizeof(struct board));
    if (!board) {
        log_fatal("Failed to allocate memory for board data structure.");
        return NULL;
    }
    board->cpu = cpu_create();
    board->cpu->board = board;
    board->memory_base = (uintptr_t)malloc(MB_TO_BYTE(memory));
    if (!board->memory_base) {
        log_fatal("Failed to allocate memory for board.");
        goto fail;
    }
    board->memory_extent = MB_TO_BYTE(memory);
    rom_load();
    return board;
fail:
    free(board);
    return NULL;
}

void board_destroy(struct board* board)
{
    if (!board) {
        log_warn("Attempting to destroy a non-existent board?");
        return;
    }
    cpu_destroy(board->cpu);
    free(board);
}

void board_poweron(struct board* board)
{
    cpu_reset(board->cpu);
}

void board_poweroff(struct board* board)
{
    cpu_reset(board->cpu);
}

void board_run(struct board* board)
{
    board->cpu->state = CPU_RUNNING;
    while (1) {
        cpu_cycle(board->cpu);
    }
}

void board_load(struct board* board, addr_t load, void* blob, size_t size)
{
    uint8_t* memory = (uint8_t*)board->memory_base;
    memcpy(&memory[load], blob, size);
}

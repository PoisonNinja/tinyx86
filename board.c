#include <board.h>
#include <hw/cpu/cpu.h>
#include <stdlib.h>

struct board* board_create(size_t memory)
{
    struct board* board = malloc(sizeof(struct board));
    if (!board) {
        log_fatal("Failed to allocate memory for board data structure.");
        return NULL;
    }
    board->cpu = cpu_create();
    board->memory_base = (uintptr_t)malloc(memory);
    if (!board->memory_base) {
        log_fatal("Failed to allocate memory for board.");
        goto fail;
    }
    board->memory_extent = memory;
    return board;
fail:
    free(board);
    return NULL;
}

void board_poweron(struct board* board)
{
    cpu_reset(board->cpu);
}

void board_run(struct board* board)
{
    while (1) {
    }
}

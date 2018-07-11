#include <hw/board.h>
#include <hw/chipset/bios.h>
#include <hw/chipset/ram.h>
#include <stdlib.h>
#include <string.h>

#define MB_TO_BYTE(x) ((x)*1024 * 1024)

static void dump_vga(struct board* board)
{
    int x, y;
    for (y = 0; y < 25; y++) {
        for (x = 0; x < 80; x++) {
            fprintf(stdout, "%c", memory_read_u8(board, 0x8000 + (y * 80) + x));
        }
        fprintf(stdout, "\n");
    }
}

struct board* board_create(size_t memory)
{
    struct board* board = malloc(sizeof(struct board));
    if (!board) {
        log_fatal("Failed to allocate memory for board data structure.");
        return NULL;
    }
    board->cpu = cpu_create();
    board->cpu->board = board;
    board->memory = memory_create();
    memory_init_ram(board, 0, MB_TO_BYTE(memory));
    bios_load(board);
    return board;
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
        if (board->cpu->state == CPU_RUNNING) {
            cpu_cycle(board->cpu);
        } else {
            dump_vga(board);
            board_destroy(board);
            return;
        }
    }
}

#include <hw/board.h>
#include <hw/chipset/memory.h>
#include <hw/chipset/ram.h>

uint8_t memory_read_byte(struct board* board, addr_t addr)
{
    return ram_read_byte(board, addr);
}

void memory_write_byte(struct board* board, addr_t addr, uint8_t value)
{
    return ram_write_byte(board, addr, value);
}

uint16_t memory_read_word(struct board* board, addr_t addr)
{
    return ram_read_word(board, addr);
}

void memory_write_word(struct board* board, addr_t addr, uint16_t value)
{
    return ram_write_word(board, addr, value);
}

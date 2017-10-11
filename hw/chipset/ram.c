#include <hw/board.h>
#include <hw/chipset/ram.h>

uint8_t ram_read_byte(struct board* board, addr_t addr)
{
    uint8_t* memory = (uint8_t*)board->memory_base;
    if (addr > board->memory_extent - 1) {
        return 0xFF;
    }
    return memory[addr];
}

void ram_write_byte(struct board* board, addr_t addr, uint8_t value)
{
    uint8_t* memory = (uint8_t*)board->memory_base;
    if (addr >= board->memory_extent - 1) {
        return;
    }
    memory[addr] = value;
}

uint16_t ram_read_word(struct board* board, addr_t addr)
{
    uint8_t* memory = (uint8_t*)board->memory_base;
    if (addr >= board->memory_extent - 2) {
        return 0xFFFF;
    }
    return *(uint16_t*)&memory[addr];
}

void ram_write_word(struct board* board, addr_t addr, uint16_t value)
{
    uint8_t* memory = (uint8_t*)board->memory_base;
    if (addr >= board->memory_extent - 2) {
        return;
    }
    *(uint16_t*)&memory[addr] = value;
}

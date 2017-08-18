#include <board.h>
#include <hw/cpu/memory.h>

uint8_t memory_read_byte(struct board* board, addr_t addr)
{
    uint8_t* memory = (uint8_t*)board->memory_base;
    if (addr > board->memory_extent - 1) {
        return 0xFF;
    }
    return memory[addr];
}

void memory_write_byte(struct board* board, addr_t addr, uint8_t value)
{
    uint8_t* memory = (uint8_t*)board->memory_base;
    if (addr >= board->memory_extent - 1) {
        return;
    }
    memory[addr] = value;
}

uint16_t memory_read_word(struct board* board, addr_t addr)
{
    uint8_t* memory = (uint8_t*)board->memory_base;
    if (addr >= board->memory_extent - 2) {
        return 0xFFFF;
    }
    return *(uint16_t*)&memory[addr];
}

void memory_write_word(struct board* board, addr_t addr, uint16_t value)
{
    uint8_t* memory = (uint8_t*)board->memory_base;
    if (addr >= board->memory_extent - 2) {
        return;
    }
    *(uint16_t*)&memory[addr] = value;
}

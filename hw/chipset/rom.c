#include <hw/board.h>
#include <hw/chipset/rom.h>

struct memory_region* memory_init_rom(struct board* board, addr_t base,
                                      size_t size)
{
    struct memory_region* region = tinyx86_malloc(sizeof(struct memory_region));
    region->base = base;
    region->size = size;
    region->host_base = tinyx86_malloc(size);
    list_add(&board->memory->rom_regions, &region->list);
    return region;
}

uint8_t rom_read_byte(struct memory_region* region, addr_t addr)
{
    uint8_t* memory = (uint8_t*)region->host_base;
    return memory[addr];
}

void rom_write_byte(__attribute__((unused)) struct memory_region* region,
                    __attribute__((unused)) addr_t addr,
                    __attribute__((unused)) uint8_t value)
{
    return;
}

uint16_t rom_read_word(struct memory_region* region, addr_t addr)
{
    uint8_t* memory = (uint8_t*)region->host_base;
    return *(uint16_t*)&memory[addr];
}

void rom_write_word(__attribute__((unused)) struct memory_region* region,
                    __attribute__((unused)) addr_t addr,
                    __attribute__((unused)) uint16_t value)
{
    return;
}

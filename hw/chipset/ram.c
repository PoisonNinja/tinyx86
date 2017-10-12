#include <hw/board.h>
#include <hw/chipset/ram.h>

void memory_init_ram(struct board* board, addr_t base, size_t size)
{
    struct memory_region* region = tinyx86_malloc(sizeof(struct memory_region));
    region->base = base;
    region->size = size;
    region->host_base = tinyx86_malloc(size);
    list_add(&board->memory->ram_regions, &region->list);
}

uint8_t ram_read_byte(struct memory_region* region, addr_t addr)
{
    uint8_t* memory = (uint8_t*)region->host_base;
    return memory[addr];
}

void ram_write_byte(struct memory_region* region, addr_t addr, uint8_t value)
{
    uint8_t* memory = (uint8_t*)region->host_base;
    memory[addr] = value;
}

uint16_t ram_read_word(struct memory_region* region, addr_t addr)
{
    uint8_t* memory = (uint8_t*)region->host_base;
    return *(uint16_t*)&memory[addr];
}

void ram_write_word(struct memory_region* region, addr_t addr, uint16_t value)
{
    uint8_t* memory = (uint8_t*)region->host_base;
    *(uint16_t*)&memory[addr] = value;
}

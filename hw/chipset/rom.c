#include <hw/board.h>
#include <hw/chipset/rom.h>

struct rom_data {
    void* host_base;
};

void memory_init_rom(struct board* board, addr_t base, size_t size, void* blob)
{
    struct memory_region* region = tinyx86_malloc(sizeof(struct memory_region));
    region->base = base;
    region->size = size;
    struct rom_data* rom_data = tinyx86_malloc(sizeof(struct rom_data));
    rom_data->host_base = blob;
    region->data = rom_data;
    list_add(&board->memory->rom_regions, &region->list);
}

uint8_t rom_read_byte(struct memory_region* region, addr_t addr)
{
    uint8_t* memory = (uint8_t*)((struct rom_data*)region->data)->host_base;
    return memory[addr];
}

void rom_write_byte(struct memory_region* region, addr_t addr, uint8_t value)
{
    uint8_t* memory = (uint8_t*)((struct rom_data*)region->data)->host_base;
    memory[addr] = value;
}

uint16_t rom_read_word(struct memory_region* region, addr_t addr)
{
    uint8_t* memory = (uint8_t*)((struct rom_data*)region->data)->host_base;
    return *(uint16_t*)&memory[addr];
}

void rom_write_word(struct memory_region* region, addr_t addr, uint16_t value)
{
    uint8_t* memory = (uint8_t*)((struct rom_data*)region->data)->host_base;
    *(uint16_t*)&memory[addr] = value;
}

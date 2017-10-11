#include <hw/board.h>
#include <hw/chipset/alias.h>

struct alias_data {
    void* host_base;
};

void memory_init_alias(struct board* board, addr_t base, size_t size,
                       void* host)
{
    struct memory_region* region = tinyx86_malloc(sizeof(struct memory_region));
    region->base = base;
    region->size = size;
    struct alias_data* alias_data = tinyx86_malloc(sizeof(struct alias_data));
    alias_data->host_base = host;
    region->data = alias_data;
    list_add(&board->memory->alias_regions, &region->list);
}

uint8_t alias_read_byte(struct memory_region* region, addr_t addr)
{
    uint8_t* memory = (uint8_t*)((struct alias_data*)region->data)->host_base;
    return memory[addr];
}

void alias_write_byte(struct memory_region* region, addr_t addr, uint8_t value)
{
    uint8_t* memory = (uint8_t*)((struct alias_data*)region->data)->host_base;
    memory[addr] = value;
}

uint16_t alias_read_word(struct memory_region* region, addr_t addr)
{
    uint8_t* memory = (uint8_t*)((struct alias_data*)region->data)->host_base;
    return *(uint16_t*)&memory[addr];
}

void alias_write_word(struct memory_region* region, addr_t addr, uint16_t value)
{
    uint8_t* memory = (uint8_t*)((struct alias_data*)region->data)->host_base;
    *(uint16_t*)&memory[addr] = value;
}

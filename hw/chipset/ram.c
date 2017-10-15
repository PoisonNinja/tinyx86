#include <hw/board.h>
#include <hw/chipset/ram.h>

uint8_t ram_read_u8(struct memory_region* region, addr_t addr)
{
    uint8_t* memory = (uint8_t*)region->host_base;
    return memory[addr - region->base];
}

void ram_write_u8(struct memory_region* region, addr_t addr, uint8_t value)
{
    uint8_t* memory = (uint8_t*)region->host_base;
    memory[addr - region->base] = value;
}

uint16_t ram_read_u16(struct memory_region* region, addr_t addr)
{
    uint8_t* memory = (uint8_t*)region->host_base;
    return *(uint16_t*)&memory[addr - region->base];
}

void ram_write_u16(struct memory_region* region, addr_t addr, uint16_t value)
{
    uint8_t* memory = (uint8_t*)region->host_base;
    *(uint16_t*)&memory[addr - region->base] = value;
}

uint32_t ram_read_u32(struct memory_region* region, addr_t addr)
{
    uint8_t* memory = (uint8_t*)region->host_base;
    return *(uint32_t*)&memory[addr - region->base];
}

void ram_write_u32(struct memory_region* region, addr_t addr, uint32_t value)
{
    uint8_t* memory = (uint8_t*)region->host_base;
    *(uint32_t*)&memory[addr - region->base] = value;
}

struct memory_region_operations ram_memory_region_operations = {
    .read_u8 = ram_read_u8,
    .write_u8 = ram_write_u8,
    .read_u16 = ram_read_u16,
    .write_u16 = ram_write_u16,
    .read_u32 = ram_read_u32,
    .write_u32 = ram_write_u32,
};

struct memory_region* memory_init_ram(struct board* board, addr_t base,
                                      size_t size)
{
    struct memory_region* region = tinyx86_malloc(sizeof(struct memory_region));
    region->base = base;
    region->size = size;
    region->host_base = tinyx86_malloc(size);
    region->memory_region_ops = &ram_memory_region_operations;
    list_runtime_init(&region->subregions);
    memory_register_region(board, region);
    return region;
}

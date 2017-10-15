#include <hw/board.h>
#include <hw/chipset/rom.h>

uint8_t rom_read_u8(struct memory_region* region, addr_t addr)
{
    uint8_t* memory = (uint8_t*)region->host_base;
    return memory[addr - region->base];
}

void rom_write_u8(__attribute__((unused)) struct memory_region* region,
                    __attribute__((unused)) addr_t addr,
                    __attribute__((unused)) uint8_t value)
{
    return;
}

uint16_t rom_read_u16(struct memory_region* region, addr_t addr)
{
    uint8_t* memory = (uint8_t*)region->host_base;
    return *(uint16_t*)&memory[addr - region->base];
}

void rom_write_u16(__attribute__((unused)) struct memory_region* region,
                    __attribute__((unused)) addr_t addr,
                    __attribute__((unused)) uint16_t value)
{
    return;
}

uint32_t rom_read_u32(struct memory_region* region, addr_t addr)
{
    uint8_t* memory = (uint8_t*)region->host_base;
    return *(uint32_t*)&memory[addr - region->base];
}

void rom_write_u32(__attribute__((unused)) struct memory_region* region,
                    __attribute__((unused)) addr_t addr,
                    __attribute__((unused)) uint32_t value)
{
    return;
}

struct memory_region_operations rom_memory_region_operations = {
    .read_u8 = rom_read_u8,
    .write_u8 = rom_write_u8,
    .read_u16 = rom_read_u16,
    .write_u16 = rom_write_u16,
    .read_u32 = rom_read_u32,
    .write_u32 = rom_write_u32,
};

struct memory_region* memory_init_rom(struct board* board, addr_t base,
                                      size_t size)
{
    struct memory_region* region = tinyx86_malloc(sizeof(struct memory_region));
    list_runtime_init(&region->subregions);
    region->base = base;
    region->size = size;
    region->host_base = tinyx86_malloc(size);
    region->memory_region_ops = &rom_memory_region_operations;
    memory_register_region(board, region);
    return region;
}

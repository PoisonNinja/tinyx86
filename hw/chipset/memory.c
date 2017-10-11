#include <hw/board.h>
#include <hw/chipset/memory.h>
#include <hw/chipset/ram.h>

struct memory* memory_create()
{
    struct memory* memory = tinyx86_malloc(sizeof(struct memory));
    list_runtime_init(&memory->mmio_regions);
    list_runtime_init(&memory->rom_regions);
    list_runtime_init(&memory->alias_regions);
    list_runtime_init(&memory->ram_regions);
    return memory;
}

uint8_t memory_read_byte(struct board* board, addr_t addr)
{
    struct memory_region* ram = NULL;
    list_for_each(&board->memory->ram_regions, list, ram)
    {
        return ram_read_byte(ram, addr);
    }
}

void memory_write_byte(struct board* board, addr_t addr, uint8_t value)
{
    struct memory_region* ram = NULL;
    list_for_each(&board->memory->ram_regions, list, ram)
    {
        return ram_write_byte(ram, addr, value);
    }
}

uint16_t memory_read_word(struct board* board, addr_t addr)
{
    struct memory_region* ram = NULL;
    list_for_each(&board->memory->ram_regions, list, ram)
    {
        return ram_read_word(ram, addr);
    }
}

void memory_write_word(struct board* board, addr_t addr, uint16_t value)
{
    struct memory_region* ram = NULL;
    list_for_each(&board->memory->ram_regions, list, ram)
    {
        return ram_write_word(ram, addr, value);
    }
}

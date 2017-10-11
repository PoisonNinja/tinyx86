#include <hw/board.h>
#include <hw/chipset/alias.h>
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
    struct memory_region* alias = NULL;
    list_for_each(&board->memory->alias_regions, list, alias)
    {
        if (alias->base <= addr && alias->size - alias->base >= 1)
            return alias_read_byte(alias, addr);
    }
    struct memory_region* ram = NULL;
    list_for_each(&board->memory->ram_regions, list, ram)
    {
        if (ram->base <= addr && ram->size - ram->base >= 1)
            return ram_read_byte(ram, addr);
    }
    return 0x0;
}

void memory_write_byte(struct board* board, addr_t addr, uint8_t value)
{
    struct memory_region* alias = NULL;
    list_for_each(&board->memory->alias_regions, list, alias)
    {
        if (alias->base <= addr && alias->size - alias->base >= 1)
            return alias_write_byte(alias, addr, value);
    }
    struct memory_region* ram = NULL;
    list_for_each(&board->memory->ram_regions, list, ram)
    {
        return ram_write_byte(ram, addr, value);
    }
}

uint16_t memory_read_word(struct board* board, addr_t addr)
{
    struct memory_region* alias = NULL;
    list_for_each(&board->memory->alias_regions, list, alias)
    {
        if (alias->base <= addr && alias->size - alias->base >= 2)
            return alias_read_word(alias, addr);
    }
    struct memory_region* ram = NULL;
    list_for_each(&board->memory->ram_regions, list, ram)
    {
        if (ram->base <= addr && ram->size - ram->base >= 2)
            return ram_read_byte(ram, addr);
    }
    return 0;
}

void memory_write_word(struct board* board, addr_t addr, uint16_t value)
{
    struct memory_region* alias = NULL;
    list_for_each(&board->memory->alias_regions, list, alias)
    {
        if (alias->base <= addr && alias->size - alias->base >= 2)
            return alias_write_word(alias, addr, value);
    }
    struct memory_region* ram = NULL;
    list_for_each(&board->memory->ram_regions, list, ram)
    {
        return ram_write_word(ram, addr, value);
    }
}

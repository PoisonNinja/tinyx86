#include <board.h>
#include <hw/chipset/memory.h>

static struct list_element mmio_list = LIST_COMPILE_INIT(mmio_list);

void mmio_register(struct mmio* mmio)
{
    list_add(&mmio_list, &mmio->list);
}

struct mmio* mmio_get(addr_t address)
{
    struct mmio* mmio = NULL;
    list_for_each(&mmio_list, list, mmio)
    {
        if (mmio->base >= address && address < mmio->base + mmio->extent) {
            return mmio;
        }
    }
    return NULL;
}

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

uint8_t memory_read_byte(struct board* board, addr_t addr)
{
    struct mmio* mmio = NULL;
    if ((mmio = mmio_get(addr))) {
        return (uint8_t)mmio->read(addr - mmio->base, 1);
    }
    uint8_t* memory = (uint8_t*)board->memory_base;
    if (addr > board->memory_extent - 1) {
        return 0xFF;
    }
    return memory[addr];
}

void memory_write_byte(struct board* board, addr_t addr, uint8_t value)
{
    struct mmio* mmio = NULL;
    if ((mmio = mmio_get(addr))) {
        return mmio->write(addr - mmio->base, value, 1);
    }
    uint8_t* memory = (uint8_t*)board->memory_base;
    if (addr >= board->memory_extent - 1) {
        return;
    }
    memory[addr] = value;
}

uint16_t memory_read_word(struct board* board, addr_t addr)
{
    struct mmio* mmio = NULL;
    if ((mmio = mmio_get(addr))) {
        return (uint16_t)mmio->read(addr - mmio->base, 2);
    }
    uint8_t* memory = (uint8_t*)board->memory_base;
    if (addr >= board->memory_extent - 2) {
        return 0xFFFF;
    }
    return *(uint16_t*)&memory[addr];
}

void memory_write_word(struct board* board, addr_t addr, uint16_t value)
{
    struct mmio* mmio = NULL;
    if ((mmio = mmio_get(addr))) {
        return mmio->write(addr - mmio->base, value, 2);
    }
    uint8_t* memory = (uint8_t*)board->memory_base;
    if (addr >= board->memory_extent - 2) {
        return;
    }
    *(uint16_t*)&memory[addr] = value;
}

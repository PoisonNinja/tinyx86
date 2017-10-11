#include <board.h>
#include <hw/chipset/memory.h>
#include <hw/chipset/mmio.h>
#include <hw/chipset/ram.h>

uint8_t memory_read_byte(struct board* board, addr_t addr)
{
    struct mmio* mmio = NULL;
    if ((mmio = mmio_get(addr))) {
        return (uint8_t)mmio->read(addr - mmio->base, 1);
    }
    return ram_read_byte(board, addr);
}

void memory_write_byte(struct board* board, addr_t addr, uint8_t value)
{
    struct mmio* mmio = NULL;
    if ((mmio = mmio_get(addr))) {
        return mmio->write(addr - mmio->base, value, 1);
    }
    return ram_write_byte(board, addr, value);
}

uint16_t memory_read_word(struct board* board, addr_t addr)
{
    struct mmio* mmio = NULL;
    if ((mmio = mmio_get(addr))) {
        return (uint16_t)mmio->read(addr - mmio->base, 2);
    }
    return ram_read_word(board, addr);
}

void memory_write_word(struct board* board, addr_t addr, uint16_t value)
{
    struct mmio* mmio = NULL;
    if ((mmio = mmio_get(addr))) {
        return mmio->write(addr - mmio->base, value, 2);
    }
    return ram_write_word(board, addr, value);
}

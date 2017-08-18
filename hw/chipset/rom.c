#include <hw/chipset/memory.h>
#include <x86emu.h>

static uint64_t rom_read(addr_t address, size_t size)
{
    return 0xFFFFFFFF;
}

struct mmio rom_mmio = {
    .base = 0xFFFFFFF0, .extent = 0xFF, .read = &rom_read,
};

void rom_load(void)
{
    mmio_register(&rom_mmio);
}

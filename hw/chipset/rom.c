#include <hw/chipset/memory.h>
#include <tinyx86.h>

static uint64_t rom_read(addr_t __attribute__((unused)) address,
                         size_t __attribute__((unused)) size)
{
    return 0xFFFFFFFF;
}

struct mmio rom_mmio = {
    .base = 0xFFFFFFF0, .extent = 0xF, .read = &rom_read,
};

void rom_load(void)
{
    mmio_register(&rom_mmio);
}

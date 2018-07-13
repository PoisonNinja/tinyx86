#include <hw/chipset/memory/rom.h>
#include <cstring>

ROM::ROM(void* initial, size_t initial_size, addr_t base, size_t size)
    : b(base), s(size)
{
    host_base = new uint8_t[s];
    // Populate the ROM area with the data
    std::memcpy(host_base, initial, initial_size);
    // Zero out the rest of the area
    std::memset(host_base + (size - initial_size), 0, size - initial_size);
}

ROM::~ROM()
{
    delete[] host_base;
}

addr_t ROM::base()
{
    return this->b;
}

size_t ROM::size()
{
    return this->s;
}

uint8_t ROM::read8(addr_t addr)
{
    return this->host_base[addr - b];
}

uint16_t ROM::read16(addr_t addr)
{
    // x86 is little endian
    return this->host_base[addr - b] | (this->host_base[addr - b + 1] << 8);
}

uint32_t ROM::read32(addr_t addr)
{
    return this->host_base[addr - b] | (this->host_base[addr - b + 1] << 8) |
           (this->host_base[addr - b + 2] << 16) |
           (this->host_base[addr - b + 3] << 24);
}

// ROM discards all writes
void ROM::write8(addr_t, uint8_t)
{
}

void ROM::write16(addr_t, uint16_t)
{
}

void ROM::write32(addr_t, uint32_t)
{
}
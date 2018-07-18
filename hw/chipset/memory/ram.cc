#include <hw/chipset/memory/ram.h>
#include <cstring>

RAM::RAM(addr_t base, size_t size) : b(base), s(size)
{
    host_base = new uint8_t[s];
}

RAM::RAM(void* initial, size_t initial_size, addr_t base, size_t size)
    : RAM(base, size)
{
    // Populate the ROM area with the data
    std::memcpy(host_base, initial, initial_size);
    // Zero out the rest of the area
    std::memset(host_base + (size - initial_size), 0, size - initial_size);
}

RAM::~RAM()
{
    delete[] host_base;
}

addr_t RAM::base()
{
    return this->b;
}

size_t RAM::size()
{
    return this->s;
}

uint8_t RAM::read8(addr_t addr)
{
    return this->host_base[addr - b];
}

uint16_t RAM::read16(addr_t addr)
{
    // x86 is little endian
    return this->host_base[addr - b] | (this->host_base[addr - b + 1] << 8);
}

uint32_t RAM::read32(addr_t addr)
{
    return this->host_base[addr - b] | (this->host_base[addr - b + 1] << 8) |
           (this->host_base[addr - b + 2] << 16) |
           (this->host_base[addr - b + 3] << 24);
}

void RAM::write8(addr_t addr, uint8_t value)
{
    this->host_base[addr - b] = value;
}

void RAM::write16(addr_t addr, uint16_t value)
{
    this->host_base[addr - b] = (value & 0xFF);
    this->host_base[addr - b + 1] = (value >> 8 & 0xFF);
}

void RAM::write32(addr_t addr, uint32_t value)
{
    this->host_base[addr - b] = (value & 0xFF);
    this->host_base[addr - b + 1] = (value >> 8 & 0xFF);
    this->host_base[addr - b + 2] = (value >> 16 & 0xFF);
    this->host_base[addr - b + 3] = (value >> 24 & 0xFF);
}
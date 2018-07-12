#pragma once

#include <tinyx86.h>

class Memory
{
public:
    virtual ~Memory(){};

    virtual addr_t base() = 0;
    virtual size_t size() = 0;

    virtual uint8_t read8(addr_t addr) = 0;
    virtual uint16_t read16(addr_t addr) = 0;
    virtual uint32_t read32(addr_t addr) = 0;

    virtual void write8(addr_t addr, uint8_t value) = 0;
    virtual void write16(addr_t addr, uint16_t value) = 0;
    virtual void write32(addr_t addr, uint32_t value) = 0;
};
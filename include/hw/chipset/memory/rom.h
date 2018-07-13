#pragma once

#include <cstddef>
#include "generic.h"

class ROM : public Memory
{
public:
    ROM(void* initial, size_t initial_size, addr_t base, size_t size);
    ~ROM() override;

    addr_t base() override;
    size_t size() override;

    uint8_t read8(addr_t addr) override;
    uint16_t read16(addr_t addr) override;
    uint32_t read32(addr_t addr) override;

    void write8(addr_t addr, uint8_t value) override;
    void write16(addr_t addr, uint16_t value) override;
    void write32(addr_t addr, uint32_t value) override;

private:
    uint8_t* host_base;
    addr_t b;
    size_t s;
};
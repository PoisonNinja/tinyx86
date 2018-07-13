#pragma once

#include <hw/chipset/memory/controller.h>
#include <hw/cpu/cpu.h>
#include <spdlog/spdlog.h>
#include <tinyx86.h>

class Board
{
public:
    Board(size_t memory);
    ~Board();
    void run();
    void stop();

    // Memory interface
    uint8_t read8(addr_t addr);
    uint16_t read16(addr_t addr);
    uint32_t read32(addr_t addr);

    void write8(addr_t addr, uint8_t value);
    void write16(addr_t addr, uint16_t value);
    void write32(addr_t addr, uint32_t value);

private:
    MemoryController memory;
    CPU cpu;
    std::shared_ptr<spdlog::logger> log;
};
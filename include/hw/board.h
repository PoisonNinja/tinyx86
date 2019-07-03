#pragma once

#include <hw/chipset/io.h>
#include <hw/chipset/memory/controller.h>
#include <hw/chipset/rtc.h>
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

    uint8_t inb(uint16_t port);
    uint16_t inw(uint16_t port);
    uint32_t inl(uint16_t port);

    void outb(uint16_t port, uint8_t val);
    void outw(uint16_t port, uint16_t val);
    void outl(uint16_t port, uint32_t val);

private:
    // Core devices
    CPU cpu;
    MemoryController memory;
    IOController io;

    // Secondary devices
    RTC rtc;

    std::shared_ptr<spdlog::logger> log;
};
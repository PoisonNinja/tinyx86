#include <hw/board.h>
#include <hw/chipset/memory/ram.h>
#include <hw/chipset/memory/rom.h>
#include <fstream>

Board::Board(size_t size) : cpu(*this), rtc(io)
{
    this->log = spdlog::get("stdout");

    // Initialize system RAM
    this->memory.register_memory(new RAM(0, size * 1024 * 1024));

    // Kludge
    std::ifstream bios;
    bios.open("../fw/bios.bin");
    bios.seekg(0, std::ios::end);
    size_t fileSize = bios.tellg();
    if (fileSize > UINT32_MAX) {
        this->log->critical("BIOS image too big, aborting\n");
        // TODO: Throw exception
    }
    bios.seekg(0, std::ios::beg);
    char* buffer = new char[fileSize];
    bios.read(buffer, fileSize);
    this->memory.register_memory(
        new ROM(buffer, fileSize, 0x100000 - fileSize, fileSize));
    uint32_t invert = fileSize;
    this->memory.register_memory(new ROM(buffer, fileSize, -invert, fileSize));
    this->log->info("BIOS image loaded to {:X} and {:X}", -invert,
                    0x100000 - fileSize);
    delete[] buffer;
}

Board::~Board()
{
}

void Board::run()
{
    do {
        cpu.tick();
    } while (cpu.get_state() != CPUState::STOPPED);
}

uint8_t Board::read8(addr_t addr)
{
    return this->memory.read8(addr);
}

uint16_t Board::read16(addr_t addr)
{
    return this->memory.read16(addr);
}

uint32_t Board::read32(addr_t addr)
{
    return this->memory.read32(addr);
}

void Board::write8(addr_t addr, uint8_t value)
{
    this->memory.write8(addr, value);
}

void Board::write16(addr_t addr, uint16_t value)
{
    this->memory.write16(addr, value);
}

void Board::write32(addr_t addr, uint32_t value)
{
    this->memory.write32(addr, value);
}

uint8_t Board::inb(uint16_t port)
{
    return this->io.inb(port);
}

uint16_t Board::inw(uint16_t port)
{
    return this->io.inw(port);
}

uint32_t Board::inl(uint16_t port)
{
    return this->io.inl(port);
}

void Board::outb(uint16_t port, uint8_t val)
{
    return this->io.outb(port, val);
}

void Board::outw(uint16_t port, uint16_t val)
{
    return this->io.outw(port, val);
}

void Board::outl(uint16_t port, uint32_t val)
{
    return this->io.outl(port, val);
}

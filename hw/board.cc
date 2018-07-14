#include <hw/board.h>
#include <hw/chipset/memory/ram.h>
#include <hw/chipset/memory/rom.h>

#include <fstream>

Board::Board(size_t size) : cpu(*this)
{
    this->log = spdlog::get("stdout");

    // Initialize system RAM
    this->memory.register_memory(new RAM(0, size * 1024 * 1024));

    // Kludge
    std::ifstream bios;
    bios.open("../fw/hello.bin");
    bios.seekg(0, std::ios::end);
    size_t fileSize = bios.tellg();
    bios.seekg(0, std::ios::beg);
    char* buffer = new char[fileSize];
    bios.read(buffer, fileSize);
    this->memory.register_memory(new ROM(buffer, fileSize, 0, 128));
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

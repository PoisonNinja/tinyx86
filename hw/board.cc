#include <hw/board.h>
#include <hw/chipset/memory/ram.h>
#include <hw/chipset/memory/rom.h>

Board::Board(size_t memory)
{
    this->log = spdlog::get("stdout");

    // Initialize system RAM
    this->memory.register_memory(new RAM(0, memory * 1024 * 1024));
}

Board::~Board()
{
}
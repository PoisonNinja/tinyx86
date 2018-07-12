#include <hw/board.h>
#include <hw/chipset/memory/ram.h>

Board::Board(size_t memory)
{
    this->log = spdlog::get("stdout");
    this->memory.register_memory(new RAM(0, memory));
}

Board::~Board()
{
}
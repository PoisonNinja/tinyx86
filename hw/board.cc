#include <hw/board.h>

Board::Board(size_t memory)
{
    this->log = spdlog::get("stdout");
}

Board::~Board()
{
}
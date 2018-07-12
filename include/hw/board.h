#pragma once

#include <spdlog/spdlog.h>
#include <tinyx86.h>

class Board
{
public:
    Board(size_t memory);
    ~Board();
    void run();
    void stop();

private:
    std::shared_ptr<spdlog::logger> log;
};
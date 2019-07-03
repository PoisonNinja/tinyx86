#pragma once

#include <hw/chipset/io.h>
#include <spdlog/spdlog.h>
#include <tinyx86.h>

class RTC
{
public:
    RTC(IOController& io);
    ~RTC();

private:
    unsigned selected_port;

    unsigned cmos[3];  // A, B, C

    bool interrupt_enabled;
    float interrupt_period;

    std::shared_ptr<spdlog::logger> log;

    uint32_t read_handler();
    void write_handler(uint16_t port, uint32_t val);
};
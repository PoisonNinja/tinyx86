#pragma once

#include <spdlog/spdlog.h>
#include <cstdint>
#include <functional>

using ioread_handler_t = std::function<uint32_t(uint16_t, unsigned)>;
using iowrite_handler_t = std::function<void(uint16_t, uint32_t, unsigned)>;

class IOController
{
public:
    IOController();
    ~IOController();

    uint8_t inb(uint16_t port);
    uint16_t inw(uint16_t port);
    uint32_t inl(uint16_t port);

    void outb(uint16_t port, uint8_t val);
    void outw(uint16_t port, uint16_t val);
    void outl(uint16_t port, uint32_t val);

    void register_io_read(uint16_t port, ioread_handler_t handler);
    void register_io_write(uint16_t port, iowrite_handler_t handler);

private:
    std::shared_ptr<spdlog::logger> log;
    struct io_handler {
        ioread_handler_t read_handler;
        iowrite_handler_t write_handler;
    };
    struct io_handler handlers[0x10000];
};
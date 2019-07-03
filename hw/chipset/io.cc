#include <hw/chipset/io.h>

IOController::IOController()
{
    this->log = spdlog::get("stdout");
}

IOController::~IOController()
{
}

uint8_t IOController::inb(uint16_t port)
{
    if (this->handlers[port].read_handler) {
        this->log->trace("[ioport] inb: 0x{:X}", port);
        return this->handlers[port].read_handler(port, 1);
    }
    this->log->warn("[ioport] inb: No handler for 0x{:X}", port);
    return 0xFF;
}

uint16_t IOController::inw(uint16_t port)
{
    if (this->handlers[port].read_handler) {
        this->log->trace("[ioport] inw: 0x{:X}", port);
        return this->handlers[port].read_handler(port, 2);
    }
    this->log->warn("[ioport] inw: No handler for 0x{:X}", port);
    return 0xFFFF;
}

uint32_t IOController::inl(uint16_t port)
{
    if (this->handlers[port].read_handler) {
        this->log->trace("[ioport] inl: 0x{:X}", port);
        return this->handlers[port].read_handler(port, 4);
    }
    this->log->warn("[ioport] inl: No handler for 0x{:X}", port);
    return 0xFFFFFFFF;
}

void IOController::outb(uint16_t port, uint8_t val)
{
    if (this->handlers[port].write_handler) {
        this->log->trace("[ioport] outb: 0x{:X}, 0x{:X}", port, val);
        return this->handlers[port].write_handler(port, val, 1);
    }
    this->log->warn("[ioport] outb: No handler for 0x{:X}, value 0x{:X}", port,
                    val);
}

void IOController::outw(uint16_t port, uint16_t val)
{
    if (this->handlers[port].write_handler) {
        this->log->trace("[ioport] outw: 0x{:X}, 0x{:X}", port, val);
        return this->handlers[port].write_handler(port, val, 2);
    }
    this->log->warn("[ioport] outw: No handler for 0x{:X}, value 0x{:X}", port,
                    val);
}

void IOController::outl(uint16_t port, uint32_t val)
{
    if (this->handlers[port].write_handler) {
        this->log->trace("[ioport] outl: 0x{:X}, 0x{:X}", port, val);
        return this->handlers[port].write_handler(port, val, 4);
    }
    this->log->warn("[ioport] outl: No handler for 0x{:X}, value 0x{:X}", port,
                    val);
}

void IOController::register_io_read(uint16_t port, ioread_handler_t handler)
{
    if (!handlers[port].read_handler) {
        this->log->trace("[ioport] Registered read handler for port 0x{:X}",
                         port);
        this->handlers[port].read_handler = handler;
        assert(this->handlers[port].read_handler);
    } else {
        this->log->warn("[ioport] Attempted to double register read handler "
                        "for port 0x{:X}",
                        port);
    }
}

void IOController::register_io_write(uint16_t port, iowrite_handler_t handler)
{
    if (!handlers[port].write_handler) {
        this->log->trace("[ioport] Registered write handler for port 0x{:X}",
                         port);
        this->handlers[port].write_handler = handler;
        assert(this->handlers[port].write_handler != nullptr);
    } else {
        this->log->warn("[ioport] Attempted to double register write handler "
                        "for port 0x{:X}",
                        port);
    }
}
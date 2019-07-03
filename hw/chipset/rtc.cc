#include <hw/chipset/rtc.h>

// https://wiki.osdev.org/RTC

namespace
{
constexpr unsigned CMOS_PORT_A = 0xA;
constexpr unsigned CMOS_PORT_B = 0xB;
constexpr unsigned CMOS_PORT_C = 0xC;
constexpr unsigned CMOS_PORT_D = 0xD;
}  // namespace

RTC::RTC(IOController& io)
    : selected_port(CMOS_PORT_D),
      interrupt_enabled(false),
      interrupt_period(1000 / 1024)
{
    this->log = spdlog::get("stdout");
    io.register_io_write(
        0x70, std::bind(&RTC::write_handler, this, std::placeholders::_1,
                        std::placeholders::_2));
    io.register_io_write(
        0x71, std::bind(&RTC::write_handler, this, std::placeholders::_1,
                        std::placeholders::_2));
    io.register_io_read(0x71, std::bind(&RTC::read_handler, this));
}

RTC::~RTC()
{
    // TODO: Unregister
}

uint32_t RTC::read_handler()
{
    switch (selected_port) {
        case CMOS_PORT_A:
            return cmos[0];
        case CMOS_PORT_B:
            return cmos[1];
        case CMOS_PORT_C:
            // TODO: Special handling for CMOS_PORT_C reads
            return cmos[2];
        case CMOS_PORT_D:
            return 0xFF;
        default:
            this->log->warn("[rtc] Read from unidentified port 0x{:X}",
                            selected_port);
    }
}

void RTC::write_handler(uint16_t port, uint32_t val)
{
    if (port == 0x70) {
        selected_port = val & 0x7F;
        // Jokes on you, we never deliver NMIs anyways
        // TODO: Maybe handle this in the future???
        // nmi_enable = val & 0x80;
    } else if (port == 0x71) {
        switch (selected_port) {
            case CMOS_PORT_A:
                this->log->trace("[rtc] Write to port A, value 0x{:X}", val);
                cmos[0] = val & 0x7F;
                this->interrupt_period =
                    1000 * (32768 >> ((cmos[0] & 0xF) - 1));
                break;
            case CMOS_PORT_B:
                this->log->warn("[rtc] Use of unimplemented port B");
                break;
            case CMOS_PORT_C:
                this->log->warn("[rtc] Attempted write to port C");
                break;
            default:
                this->log->warn("[rtc] Write to unidentified port 0x{:X}",
                                port);
        }
    }
}
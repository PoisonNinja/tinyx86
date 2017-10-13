#include <hw/board.h>
#include <hw/chipset/bios.h>
#include <hw/chipset/ram.h>

#define BIOS_PATH "../fw/bios.bin"
#define VGABIOS_PATH "../fw/vgabios.bin"

void bios_load(struct board* board)
{
    tinyx86_file_t bios = tinyx86_file_open(BIOS_PATH, "r");
    if (!bios) {
        log_fatal("Failed to locate BIOS binary");
        tinyx86_exit(1);
    }
    ssize_t bios_size = tinyx86_file_size(bios);
    uint8_t* bios_buffer = tinyx86_malloc(bios_size);
    if (tinyx86_file_read(bios, bios_buffer, bios_size) < bios_size) {
        log_fatal("Failed to read entire BIOS binary");
        tinyx86_exit(1);
    }
    struct memory_region* bios_low = memory_init_ram(board, 0, 0x100000);
    memory_load_image(bios_low, bios_buffer, 0x100000 - bios_size, bios_size);
    struct memory_region* bios_high =
        memory_init_ram(board, 0xFFF00000, 0x100000);
    memory_load_image(bios_high, bios_buffer, 0x100000 - bios_size, bios_size);
    tinyx86_free(bios_buffer);
    tinyx86_file_close(bios);
}

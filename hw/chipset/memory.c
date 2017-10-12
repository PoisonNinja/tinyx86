#include <hw/board.h>
#include <hw/chipset/memory.h>
#include <hw/chipset/ram.h>
#include <hw/chipset/rom.h>
#include <string.h>

struct memory* memory_create()
{
    struct memory* memory = tinyx86_malloc(sizeof(struct memory));
    list_runtime_init(&memory->regions);
    return memory;
}

struct memory_region* memory_resolve_region(struct board* board, addr_t addr)
{
    struct memory_region* top = NULL;
    list_for_each(&board->memory->regions, list, top)
    {
        if (top->base <= addr && top->base + top->size > addr) {
            if (!list_empty(&top->subregions)) {
                struct memory_region* region = NULL;
                list_for_each(&top->subregions, list, region)
                {
                    if (region->base <= addr &&
                        region->base + region->size > addr) {
                        return region;
                    }
                }
            }
            return top;
        }
    }
    return NULL;
}

void memory_insert_region(struct board* board, struct memory_region* region)
{
    if (list_empty(&board->memory->regions)) {
        list_add(&board->memory->regions, &region->list);
    } else {
        struct memory_region* top = NULL;
        list_for_each(&board->memory->regions, list, top)
        {
            if (top->base <= region->base && top->size >= region->size) {
                list_add(&top->subregions, &region->list);
                return;
            }
        }
        list_add(&board->memory->regions, &region->list);
    }
}

uint8_t memory_read_byte(struct board* board, addr_t addr)
{
    struct memory_region* region = NULL;
    if ((region = memory_resolve_region(board, addr))) {
        return region->memory_region_ops->read_byte(region, addr);
    }
    log_fatal("Attempting to read outside memory: %p", addr);
    tinyx86_exit(1);
}

void memory_write_byte(struct board* board, addr_t addr, uint8_t value)
{
    struct memory_region* region = NULL;
    if ((region = memory_resolve_region(board, addr))) {
        return region->memory_region_ops->write_byte(region, addr, value);
    }
    log_fatal("Attempting to write outside memory: %p", addr);
    tinyx86_exit(1);
}

uint16_t memory_read_word(struct board* board, addr_t addr)
{
    struct memory_region* region = NULL;
    if ((region = memory_resolve_region(board, addr))) {
        return region->memory_region_ops->read_word(region, addr);
    }
    log_fatal("Attempting to read outside memory: %p", addr);
    tinyx86_exit(1);
}

void memory_write_word(struct board* board, addr_t addr, uint16_t value)
{
    struct memory_region* region = NULL;
    if ((region = memory_resolve_region(board, addr))) {
        return region->memory_region_ops->write_word(region, addr, value);
    }
    log_fatal("Attempting to write outside memory: %p", addr);
    tinyx86_exit(1);
}

int memory_load_image(struct memory_region* region, void* blob, addr_t offset,
                      size_t size)
{
    if (offset + size > region->size) {
        return 1;
    }
    uint8_t* target = ((uint8_t*)region->host_base) + offset;
    memcpy(target, blob, size);
    return 0;
}

#pragma once

#include <list.h>
#include <tinyx86.h>

struct mmio {
    addr_t base;
    addr_t extent;
    uint64_t (*read)(addr_t address, size_t size);
    void (*write)(addr_t address, uint64_t data, size_t size);
    struct list_element list;
};

extern void mmio_register(struct mmio* mmio);
extern struct mmio* mmio_get(addr_t address);

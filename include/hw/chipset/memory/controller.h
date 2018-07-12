#pragma once

#include <spdlog/spdlog.h>
#include <cstdint>
#include <memory>
#include <vector>
#include "generic.h"

struct MemoryNode {
    MemoryNode(Memory* m);
    ~MemoryNode();
    Memory* memory;
    std::vector<MemoryNode*> subregions;
};

class MemoryController
{
public:
    MemoryController();
    ~MemoryController();

    void register_memory(Memory* memory);

    uint8_t read8(addr_t addr);
    uint16_t read16(addr_t addr);
    uint32_t read32(addr_t addr);

    void write8(addr_t addr, uint8_t value);
    void write16(addr_t addr, uint16_t value);
    void write32(addr_t addr, uint32_t value);

private:
    MemoryNode* locate_node(addr_t addr);
    MemoryNode* locate_node(addr_t addr, MemoryNode* root);
    std::vector<MemoryNode*> regions;

    std::shared_ptr<spdlog::logger> log;
};
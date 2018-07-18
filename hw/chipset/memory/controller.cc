#include <hw/chipset/memory/controller.h>

MemoryNode::MemoryNode(Memory* m) : memory(m){};

MemoryNode::~MemoryNode()
{
    for (auto node : this->subregions) {
        delete node;
    }
    delete memory;
}

MemoryController::MemoryController()
{
    this->log = spdlog::get("stdout");
}

MemoryController::~MemoryController()
{
    for (auto node : regions) {
        delete node;
    }
}

MemoryNode* MemoryController::locate_node(addr_t addr, MemoryNode* root)
{
    for (auto node : root->subregions) {
        if (node->memory->base() <= addr &&
            node->memory->base() + node->memory->size() > addr) {
            if (node->subregions.empty()) {
                return node;
            } else {
                return this->locate_node(addr, node);
            }
        }
    }
    return root;
}

MemoryNode* MemoryController::locate_node(addr_t addr)
{
    for (auto node : this->regions) {
        if (node->memory->base() <= addr &&
            node->memory->base() + node->memory->size() > addr) {
            if (node->subregions.empty()) {
                return node;
            } else {
                return this->locate_node(addr, node);
            }
        }
    }
    return nullptr;
}

void MemoryController::register_memory(Memory* memory)
{
    MemoryNode* node = new MemoryNode(memory);
    MemoryNode* parent = nullptr;
    if ((parent = this->locate_node(memory->base()))) {
        parent->subregions.push_back(node);
    } else {
        regions.push_back(node);
    }
}

uint8_t MemoryController::read8(addr_t addr)
{
    this->log->trace("[memory] read8 at 0x{:X}", addr);
    MemoryNode* node = locate_node(addr);
    if (!node) {
        this->log->error("Attempting to read outside of memory at {:x}", addr);
        return 0;
    }
    return node->memory->read8(addr);
}

uint16_t MemoryController::read16(addr_t addr)
{
    this->log->trace("[memory] read16 at 0x{:X}", addr);
    MemoryNode* node = locate_node(addr);
    if (!node) {
        this->log->error("Attempting to read outside of memory at {:x}", addr);
        return 0;
    }
    return node->memory->read16(addr);
}

uint32_t MemoryController::read32(addr_t addr)
{
    this->log->trace("[memory] read32 at 0x{:X}", addr);
    MemoryNode* node = locate_node(addr);
    if (!node) {
        this->log->error("Attempting to read outside of memory at {:x}", addr);
        return 0;
    }
    return node->memory->read32(addr);
}

void MemoryController::write8(addr_t addr, uint8_t value)
{
    this->log->trace("[memory] write8 at 0x{:X} with value 0x{:X}", addr,
                     value);
    MemoryNode* node = locate_node(addr);
    if (!node) {
        this->log->error("Attempting to write outside of memory at {:x}", addr);
    }
    return node->memory->write8(addr, value);
}

void MemoryController::write16(addr_t addr, uint16_t value)
{
    this->log->trace("[memory] write16 at 0x{:X} with value 0x{:X}", addr,
                     value);
    MemoryNode* node = locate_node(addr);
    if (!node) {
        this->log->error("Attempting to write outside of memory at {:x}", addr);
    }
    return node->memory->write16(addr, value);
}

void MemoryController::write32(addr_t addr, uint32_t value)
{
    this->log->trace("[memory] write32 at 0x{:X} with value 0x{:X}", addr,
                     value);
    MemoryNode* node = locate_node(addr);
    if (!node) {
        this->log->error("Attempting to write outside of memory at {:x}", addr);
    }
    return node->memory->write32(addr, value);
}
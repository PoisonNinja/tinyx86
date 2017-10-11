#include <board.h>
#include <hw/chipset/mmio.h>

static struct list_element mmio_list = LIST_COMPILE_INIT(mmio_list);

void mmio_register(struct mmio* mmio)
{
    list_add(&mmio_list, &mmio->list);
}

struct mmio* mmio_get(addr_t address)
{
    struct mmio* mmio = NULL;
    list_for_each(&mmio_list, list, mmio)
    {
        if (mmio->base <= address && address < mmio->base + mmio->extent) {
            return mmio;
        }
    }
    return NULL;
}

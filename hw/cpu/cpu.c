#include <hw/cpu/cpu.h>
#include <hw/cpu/opcode.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void cpu_cycle(struct cpu* cpu)
{
    opcode_execute(cpu);
}

void cpu_reset(struct cpu* cpu)
{
    // cpu->cs.selector = 0xF000;
    // cpu->cs.base = 0xFFFF0000;
    // cpu->cs.limit = 0xFFFF;
    // cpu->ip.regs_16 = 0xFFF0;
    // cpu->cs.selector = 0xF000;
    cpu->cs.base = 0x0;
    cpu->cs.limit = 0xFFFF;
    cpu->ip.regs_16 = 0x1000;
}

struct cpu* cpu_create(void)
{
    struct cpu* cpu = malloc(sizeof(struct cpu));
    if (!cpu) {
        log_fatal("Failed to allocate memory for CPU structure");
        return NULL;
    }
    return cpu;
}

void cpu_destroy(struct cpu* cpu)
{
    if (!cpu)
        return;
    free(cpu);
}

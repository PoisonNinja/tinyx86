#include <cpu/cpu.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct cpu* cpu_create(void)
{
    struct cpu* cpu = malloc(sizeof(struct cpu));
    if (!cpu) {
        fprintf(stderr, "Failed to allocate memory for CPU structure\n");
        return NULL;
    }
    memset(cpu, 0, sizeof(struct cpu));
    return cpu;
}

void cpu_destroy(struct cpu* cpu)
{
    if (!cpu)
        return;
    free(cpu);
}

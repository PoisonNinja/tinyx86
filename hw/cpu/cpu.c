#include <hw/board.h>
#include <hw/cpu/cpu.h>
#include <hw/cpu/opcode.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void cpu_cycle(struct cpu* cpu)
{
    if (cpu->state == CPU_RUNNING) {
        opcode_execute(cpu);
        cpu_dump(cpu);
    }
}

void cpu_reset(struct cpu* cpu)
{
    struct board* board = cpu->board;
    memset(cpu, 0, sizeof(struct cpu));
    cpu->board = board;
    cpu->cs.selector = 0xF000;
    cpu->cs.base = 0xFFFF0000;
    cpu->cs.limit = 0xFFFF;
    cpu->ip.regs_16 = 0xFFF0;
    cpu->cs.selector = 0xF000;
    cpu->state = CPU_STOPPED;
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

void cpu_dump(struct cpu* cpu)
{
    log_trace("========================= CPU Dump =========================");
    log_trace("AX: 0x%04X BX: 0x%04X CX: 0x%04X DX: 0x%04X", cpu->ax.regs_16,
              cpu->bx.regs_16, cpu->cx.regs_16, cpu->dx.regs_16);
    log_trace("SP: 0x%04X BP: 0x%04X SI: 0x%04X DI: 0x%04X IP: 0x%04X",
              cpu->sp.regs_16, cpu->bp.regs_16, cpu->si.regs_16,
              cpu->di.regs_16, cpu->ip.regs_16);
    log_trace(
        "CS: 0x%04X DS: 0x%04X SS: 0x%04X ES: 0x%04X FS: 0x%04X GS: 0x%04X",
        cpu->cs.selector, cpu->ds.selector, cpu->ss.selector, cpu->es.selector,
        cpu->fs.selector, cpu->gs.selector);
    log_trace("EFLAGS: %X", *(uint32_t*)&cpu->eflags);
    log_trace("============================================================");
}

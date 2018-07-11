#include <hw/cpu/cpu.h>
#include <hw/cpu/memory.h>
#include <hw/cpu/opcode.h>

void raw_to_modrm(uint8_t raw, struct modrm* modrm)
{
    uint8_t* tmp = (uint8_t*)modrm;
    *tmp = raw;
    log_trace("mod: %u reg: %u rm: %u", modrm->mod, modrm->reg, modrm->rm);
}

union cpu_register* modrm_to_register(struct cpu* cpu, uint8_t modrm)
{
    switch (modrm) {
        case 0:
            return &cpu->ax;
            break;
        case 1:
            return &cpu->cx;
            break;
        case 2:
            return &cpu->dx;
            break;
        case 3:
            return &cpu->bx;
            break;
        case 4:
            return &cpu->sp;
            break;
        case 5:
            return &cpu->bp;
            break;
        case 6:
            return &cpu->si;
            break;
        case 7:
            return &cpu->di;
            break;
        default:
            return NULL;
    }
}

static uint32_t modrm_to_address_no_dis(struct cpu* cpu, uint8_t rm)
{
    if (CPU_PREFIX_STATE_OPERAND32(cpu)) {
        switch (rm) {
            case 0:
                return cpu->ax.regs_32;
                break;
            case 1:
                return cpu->cx.regs_32;
                break;
            case 2:
                return cpu->dx.regs_32;
                break;
            case 3:
                return cpu->bx.regs_32;
                break;
            case 4:
                // TODO: SIB decoding
                return 0;
                break;
            case 5:
                return cpu_fetch_instruction_u32(cpu);
                break;
            case 6:
                return cpu->si.regs_32;
                break;
            case 7:
                return cpu->di.regs_32;
                break;
            default:
                return 0;
        }
    } else {
        switch (rm) {
            case 0:
                return (cpu->bx.regs_16 + cpu->si.regs_16);
                break;
            case 1:
                return (cpu->bx.regs_16 + cpu->di.regs_16);
                break;
            case 2:
                return (cpu->bp.regs_16 + cpu->si.regs_16);
                break;
            case 3:
                return (cpu->bp.regs_16 + cpu->di.regs_16);
                break;
            case 4:
                return (cpu->si.regs_16);
                break;
            case 5:
                return (cpu->di.regs_16);
                break;
            case 6:
                return cpu_fetch_instruction_u16(cpu);
                break;
            case 7:
                return (cpu->bx.regs_16);
                break;
            default:
                return 0;
        }
    }
}

uint32_t modrm_to_address(struct cpu* cpu, uint8_t mod, uint8_t rm)
{
    switch (mod) {
        case 0:
            return modrm_to_address_no_dis(cpu, rm);
            break;
        case 1:
            log_fatal("modrm_to_address_one_dis is not implemented yet!");
            break;
        case 2:
            log_fatal("modrm_to_address_four_dis is not implemented yet!");
            break;
        default:
            log_fatal("modrm_to_address received invalid mod!");
    }
    return 0;
}

uint8_t fetch_modrm_r8(struct cpu* cpu)
{
    struct modrm* modrm = &cpu->modrm;
    return modrm_to_register(cpu, modrm->reg)->regs_8;
}

uint8_t fetch_modrm_rm8(struct cpu* cpu)
{
    struct modrm* modrm = &cpu->modrm;
    if (modrm->mod == 3) {
        return modrm_to_register(cpu, modrm->reg)->regs_8;
    } else {
        return cpu_fetch_u8(
            cpu,
            (cpu->prefix_state.segment) ? cpu->prefix_state.segment : &cpu->ds,
            modrm_to_address(cpu, modrm->mod, modrm->rm));
    }
}

void store_modrm_r8(struct cpu* cpu, uint8_t val)
{
    struct modrm* modrm = &cpu->modrm;
    modrm_to_register(cpu, modrm->reg)->regs_8 = val;
}

void store_modrm_rm8(struct cpu* cpu, uint8_t val)
{
    struct modrm* modrm = &cpu->modrm;
    if (modrm->mod == 3) {
        modrm_to_register(cpu, modrm->reg)->regs_8 = val;
    } else {
        cpu_store_u8(
            cpu,
            (cpu->prefix_state.segment) ? cpu->prefix_state.segment : &cpu->ds,
            modrm_to_address(cpu, modrm->mod, modrm->rm), val);
    }
}
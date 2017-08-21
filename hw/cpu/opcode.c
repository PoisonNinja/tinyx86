#include <hw/cpu/cpu.h>
#include <hw/cpu/memory.h>
#include <hw/cpu/opcode.h>

typedef void (*opcode_fn_t)(struct cpu* cpu);

#define OPCODE_DEFINE(number) static void opcode##number(struct cpu* cpu)
#define OPCODE_ENTRY(number) &opcode##number

/*
 * Fast parity lookup borrowed from:
 * http://www.graphics.stanford.edu/~seander/bithacks.html#ParityLookupTable
 */
static const uint8_t ParityTable256[256] = {
#define P2(n) n, n ^ 1, n ^ 1, n
#define P4(n) P2(n), P2(n ^ 1), P2(n ^ 1), P2(n)
#define P6(n) P4(n), P4(n ^ 1), P4(n ^ 1), P4(n)
    P6(0), P6(1), P6(1), P6(0),
};

static uint8_t calculate_parity(uint32_t value)
{
    value ^= value >> 16;
    value ^= value >> 8;
    uint8_t parity = ParityTable256[value & 0xff];
    return parity;
}

static inline void raw_to_modrm(uint8_t raw, struct modrm* modrm)
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

uint32_t modrm_to_address_no_dis(struct cpu* cpu, uint8_t rm)
{
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
            return cpu_fetch_instruction_word(cpu);
            break;
        case 7:
            return (cpu->bx.regs_16);
            break;
        default:
            return 0;
    }
}

uint32_t modrm_to_address_one_dis(struct cpu* cpu, uint8_t rm)
{
    log_warn("modrm_to_address_one_dis is not implemented yet!");
}

uint32_t modrm_to_address_four_dis(struct cpu* cpu, uint8_t rm)
{
    log_warn("modrm_to_address_four_dis is not implemented yet!");
}

uint32_t modrm_to_address(struct cpu* cpu, uint8_t mod, uint8_t rm)
{
    switch (mod) {
        case 0:
            return modrm_to_address_no_dis(cpu, rm);
            break;
        case 1:
            return modrm_to_address_one_dis(cpu, rm);
            break;
        case 2:
            return modrm_to_address_four_dis(cpu, rm);
            break;
        default:
            return 0;
    }
}

/*
 * Stack operations (push, pop)
 */
void push_word(struct cpu* cpu, uint16_t value)
{
    cpu->sp.regs_16 -= 2;
    cpu_store_word(cpu, &cpu->ss, cpu->sp.regs_16, value);
}

/*
 * Primitive operations (add, sub, etc.)
 */
void inc_word(struct cpu* cpu, union cpu_register* reg)
{
    uint32_t result = reg->regs_16 + 1;
    cpu->eflags.parity = calculate_parity(result);
    cpu->eflags.adjust = (((reg->regs_16 ^ 1 ^ result) & 0x10) != 0);
    cpu->eflags.zero = ((result & 0xFFFF) == 0);
    cpu->eflags.sign = (result & 0x8000);
    cpu->eflags.overflow =
        (((result ^ reg->regs_16) & (result ^ 1) & 0x8000) != 0);
    reg->regs_16 = (uint16_t)result;
}

/*
 * 0x43: INC dx
 */
OPCODE_DEFINE(43)
{
    log_trace("inc bx");
    inc_word(cpu, &cpu->bx);
}

/*
 * 0x52: PUSH dx
 */
OPCODE_DEFINE(52)
{
    log_trace("push dx");
    push_word(cpu, cpu->dx.regs_16);
}

/*
 * 0x53: PUSH bx
 */
OPCODE_DEFINE(53)
{
    log_trace("push bx");
    push_word(cpu, cpu->bx.regs_16);
}

/*
 * 0x89: MOV r/m16/32, r16/32
 */

OPCODE_DEFINE(89)
{
    log_trace("mov r/m16/32, r16/32");
    struct modrm modrm;
    raw_to_modrm(cpu_fetch_instruction_byte(cpu), &modrm);
    if (modrm.mod == 3) {
        // Direct addressing mode, reg is source, rm is dest because d == 1
        union cpu_register* source = modrm_to_register(cpu, modrm.reg);
        union cpu_register* dest = modrm_to_register(cpu, modrm.rm);
        dest->regs_16 = source->regs_16;
    } else {
        // TODO: Implement if mod != 3
        log_fatal("Only MOD 3 for opcode 0x89 is unsupported");
    }
}

/*
 * 0x8A: MOV r8, r/m8
 */
OPCODE_DEFINE(8A)
{
    log_trace("mov r8, r/m8");
    struct modrm modrm;
    raw_to_modrm(cpu_fetch_instruction_byte(cpu), &modrm);
    if (modrm.mod == 3) {
        // TODO; Implement if mod == 3
        log_fatal("MOD 3 for opcode 0x8A is unsupported");
    } else {
        // Direct addressing mode, rm is source, reg is dest because d == 1
        union cpu_register* dest = modrm_to_register(cpu, modrm.reg);
        dest->regs_16 = cpu_fetch_byte(
            cpu, &cpu->ds, modrm_to_address(cpu, modrm.mod, modrm.rm));
    }
}

/*
 * 0xB8: MOV r16/32, imm16/32
 */
OPCODE_DEFINE(B8)
{
    log_trace("mov ax, imm16");
    cpu->ax.regs_16 = cpu_fetch_instruction_word(cpu);
}

/*
 * 0xBC: MOV r16/32, imm16/32
 */
OPCODE_DEFINE(BC)
{
    log_trace("mov r16/32, imm16/32");
    cpu->sp.regs_16 = cpu_fetch_instruction_word(cpu);
}

/*
 * 0xE8: CALL rel16/32
 */
OPCODE_DEFINE(E8)
{
    log_trace("call rel16/32");
    int16_t offset = cpu_fetch_instruction_word(cpu);
    uint16_t eip = cpu->ip.regs_16 + offset;
    push_word(cpu, cpu->ip.regs_16);
    cpu->ip.regs_16 = eip;
}

opcode_fn_t opcode_table[256] = {
    NULL,     NULL,     NULL,     NULL,     NULL,     NULL, NULL, NULL,
    NULL,     NULL,     NULL,     NULL,     NULL,     NULL, NULL, NULL,
    NULL,     NULL,     NULL,     NULL,     NULL,     NULL, NULL, NULL,
    NULL,     NULL,     NULL,     NULL,     NULL,     NULL, NULL, NULL,
    NULL,     NULL,     NULL,     NULL,     NULL,     NULL, NULL, NULL,
    NULL,     NULL,     NULL,     NULL,     NULL,     NULL, NULL, NULL,
    NULL,     NULL,     NULL,     NULL,     NULL,     NULL, NULL, NULL,
    NULL,     NULL,     NULL,     NULL,     NULL,     NULL, NULL, NULL,
    NULL,     NULL,     NULL,     opcode43, NULL,     NULL, NULL, NULL,
    NULL,     NULL,     NULL,     NULL,     NULL,     NULL, NULL, NULL,
    NULL,     NULL,     opcode52, opcode53, NULL,     NULL, NULL, NULL,
    NULL,     NULL,     NULL,     NULL,     NULL,     NULL, NULL, NULL,
    NULL,     NULL,     NULL,     NULL,     NULL,     NULL, NULL, NULL,
    NULL,     NULL,     NULL,     NULL,     NULL,     NULL, NULL, NULL,
    NULL,     NULL,     NULL,     NULL,     NULL,     NULL, NULL, NULL,
    NULL,     NULL,     NULL,     NULL,     NULL,     NULL, NULL, NULL,
    NULL,     NULL,     NULL,     NULL,     NULL,     NULL, NULL, NULL,
    NULL,     opcode89, opcode8A, NULL,     NULL,     NULL, NULL, NULL,
    NULL,     NULL,     NULL,     NULL,     NULL,     NULL, NULL, NULL,
    NULL,     NULL,     NULL,     NULL,     NULL,     NULL, NULL, NULL,
    NULL,     NULL,     NULL,     NULL,     NULL,     NULL, NULL, NULL,
    NULL,     NULL,     NULL,     NULL,     NULL,     NULL, NULL, NULL,
    NULL,     NULL,     NULL,     NULL,     NULL,     NULL, NULL, NULL,
    opcodeB8, NULL,     NULL,     NULL,     opcodeBC, NULL, NULL, NULL,
    NULL,     NULL,     NULL,     NULL,     NULL,     NULL, NULL, NULL,
    NULL,     NULL,     NULL,     NULL,     NULL,     NULL, NULL, NULL,
    NULL,     NULL,     NULL,     NULL,     NULL,     NULL, NULL, NULL,
    NULL,     NULL,     NULL,     NULL,     NULL,     NULL, NULL, NULL,
    NULL,     NULL,     NULL,     NULL,     NULL,     NULL, NULL, NULL,
    opcodeE8, NULL,     NULL,     NULL,     NULL,     NULL, NULL, NULL,
    NULL,     NULL,     NULL,     NULL,     NULL,     NULL, NULL, NULL,
    NULL,     NULL,     NULL,     NULL,     NULL,     NULL, NULL, NULL,
};

void opcode_execute(struct cpu* cpu)
{
    uint8_t opcode = cpu_fetch_instruction_byte(cpu);
    if (opcode == 0x0F) {
        log_fatal("We do not support two byte opcodes yet!");
    }
    log_trace("Opcode: %X", opcode);
    if (!opcode_table[opcode]) {
        log_fatal("Unknown opcode, should probably throw an Invalid Opcode "
                  "exception, got %X",
                  opcode);
    }
    opcode_table[opcode](cpu);
}

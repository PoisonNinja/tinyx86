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
    P6(0),
    P6(1),
    P6(1),
    P6(0),
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
            log_warn("modrm_to_address_one_dis is not implemented yet!");
            break;
        case 2:
            log_warn("modrm_to_address_four_dis is not implemented yet!");
            break;
    }
    return 0;
}

/*
 * Stack operations (push, pop)
 */
void push_u16(struct cpu* cpu, uint16_t value)
{
    cpu->sp.regs_16 -= 2;
    cpu_store_u16(cpu, &cpu->ss, cpu->sp.regs_16, value);
}

uint16_t pop_u16(struct cpu* cpu)
{
    uint16_t ret = cpu_fetch_u16(cpu, &cpu->ss, cpu->sp.regs_16);
    cpu->sp.regs_16 += 2;
    return ret;
}

void push_u32(struct cpu* cpu, uint32_t value)
{
    cpu->sp.regs_16 -= 4;
    cpu_store_u32(cpu, &cpu->ss, cpu->sp.regs_32, value);
}

uint16_t pop_u32(struct cpu* cpu)
{
    uint16_t ret = cpu_fetch_u32(cpu, &cpu->ss, cpu->sp.regs_32);
    cpu->sp.regs_16 += 4;
    return ret;
}

/*
 * Primitive operations (add, sub, etc.)
 */
void inc_u16(struct cpu* cpu, union cpu_register* reg)
{
    uint32_t result = reg->regs_16 + 1;
    cpu->eflags.parity = (calculate_parity(result) == 0);
    cpu->eflags.adjust = (((reg->regs_16 ^ 1 ^ result) & 0x10) != 0);
    cpu->eflags.zero = ((result & 0xFFFF) == 0);
    cpu->eflags.sign = (result & 0x8000);
    cpu->eflags.overflow =
        (((result ^ reg->regs_16) & (result ^ 1) & 0x8000) != 0);
    reg->regs_16 = (uint16_t)result;
}

void inc_u32(struct cpu* cpu, union cpu_register* reg)
{
    uint32_t result = reg->regs_32 + 1;
    cpu->eflags.parity = (calculate_parity(result) == 0);
    cpu->eflags.adjust = (((reg->regs_32 ^ 1 ^ result) & 0x10) != 0);
    cpu->eflags.zero = ((result & 0xFFFFFFFF) == 0);
    cpu->eflags.sign = (result & 0x80000000);
    cpu->eflags.overflow =
        (((result ^ reg->regs_32) & (result ^ 1) & 0x80000000) != 0);
    reg->regs_16 = (uint16_t)result;
}

/*
 * 0x20: AND r/m8, r8
 */
OPCODE_DEFINE(20)
{
    log_trace("AND r/m8, r8");
    struct modrm modrm;
    raw_to_modrm(cpu_fetch_instruction_u8(cpu), &modrm);
    if (modrm.mod == 3) {
        // Direct addressing mode, reg is source, rm is dest because d == 0
        union cpu_register* source = modrm_to_register(cpu, modrm.reg);
        union cpu_register* dest = modrm_to_register(cpu, modrm.rm);
        uint8_t result = source->regs_8 & dest->regs_8;
        cpu->eflags.overflow = 0;
        cpu->eflags.carry = 0;
        cpu->eflags.parity = (calculate_parity(result) == 0);
        cpu->eflags.zero = ((result & 0xFFFF) == 0);
        cpu->eflags.sign = ((result & 0x8000) == 1);
        dest->regs_8 = result;

    } else {
        // TODO: Implement if mod != 3
        log_fatal("Only MOD 3 for opcode 0x20 is supported");
    }
}

/*
 * 0x43: INC dx
 */
OPCODE_DEFINE(43)
{
    if (CPU_PREFIX_STATE_OPERAND32(cpu)) {
        log_trace("inc ebx");
        inc_u32(cpu, &cpu->bx);
    } else {
        log_trace("inc bx");
        inc_u16(cpu, &cpu->bx);
    }
}

/*
 * 0x47: INC di
 */
OPCODE_DEFINE(47)
{
    if (CPU_PREFIX_STATE_OPERAND32(cpu)) {
        log_trace("inc edi");
        inc_u32(cpu, &cpu->di);
    } else {
        log_trace("inc di");
        inc_u16(cpu, &cpu->di);
    }
}

/*
 * 0x52: PUSH dx
 */
OPCODE_DEFINE(52)
{
    if (CPU_PREFIX_STATE_OPERAND32(cpu)) {
        log_trace("push edx");
        push_u32(cpu, cpu->dx.regs_32);
    } else {
        log_trace("push dx");
        push_u16(cpu, cpu->dx.regs_16);
    }
}

/*
 * 0x53: PUSH bx
 */
OPCODE_DEFINE(53)
{
    if (CPU_PREFIX_STATE_OPERAND32(cpu)) {
        log_trace("push ebx");
        push_u32(cpu, cpu->bx.regs_32);
    } else {
        log_trace("push bx");
        push_u16(cpu, cpu->bx.regs_16);
    }
}

/*
 * 0x57: PUSH di
 */
OPCODE_DEFINE(57)
{
    if (CPU_PREFIX_STATE_OPERAND32(cpu)) {
        log_trace("push edi");
        push_u32(cpu, cpu->di.regs_32);
    } else {
        log_trace("push di");
        push_u16(cpu, cpu->di.regs_16);
    }
}

/*
 * 0x5A: POP dx
 */
OPCODE_DEFINE(5A)
{
    if (CPU_PREFIX_STATE_OPERAND32(cpu)) {
        log_trace("pop edx");
        cpu->dx.regs_32 = pop_u32(cpu);
    } else {
        log_trace("pop dx");
        cpu->dx.regs_16 = pop_u16(cpu);
    }
}

/*
 * 0x5B: POP bx
 */
OPCODE_DEFINE(5B)
{
    if (CPU_PREFIX_STATE_OPERAND32(cpu)) {
        log_trace("pop ebx");
        cpu->bx.regs_32 = pop_u32(cpu);
    } else {
        log_trace("pop bx");
        cpu->bx.regs_16 = pop_u16(cpu);
    }
}

/*
 * 0x5F: POP di
 */
OPCODE_DEFINE(5F)
{
    if (CPU_PREFIX_STATE_OPERAND32(cpu)) {
        log_trace("pop edi");
        cpu->di.regs_32 = pop_u32(cpu);
    } else {
        log_trace("pop di");
        cpu->di.regs_16 = pop_u16(cpu);
    }
}

/*
 * 0x75: JNZ/JNE rel8
 */
OPCODE_DEFINE(75)
{
    log_trace("jnz rel8");
    int8_t offset = cpu_fetch_instruction_u8(cpu);
    uint32_t ip = ((CPU_PREFIX_STATE_OPERAND32(cpu)) ? cpu->ip.regs_32 :
                                                       cpu->ip.regs_16) +
                  offset;
    if (!cpu->eflags.zero)
        cpu->ip.regs_32 = ip;
}

/*
 * 0x86: XCHG r8, r/m8
 */
OPCODE_DEFINE(86)
{
    log_trace("xchg r8, r/m8");
    struct modrm modrm;
    raw_to_modrm(cpu_fetch_instruction_u8(cpu), &modrm);
    if (modrm.mod == 3) {
        // Direct addressing mode, rm is source, reg is dest because d == 1
        union cpu_register* a = modrm_to_register(cpu, modrm.reg);
        union cpu_register* b = modrm_to_register(cpu, modrm.rm);
        uint8_t temp = a->regs_8;
        a->regs_8 = b->regs_8;
        b->regs_8 = temp;
    } else {
        // TODO: Implement if mod != 3
        log_fatal("Only MOD 3 for opcode 0x86 is supported");
    }
}

/*
 * 0x88: r/m8, r8
 */
OPCODE_DEFINE(88)
{
    log_trace("mov r/m8, r8");
    struct modrm modrm;
    raw_to_modrm(cpu_fetch_instruction_u8(cpu), &modrm);
    if (modrm.mod == 3) {
        // TODO; Implement if mod == 3
        log_fatal("MOD 3 for opcode 0x88 is unsupported");
    } else {
        union cpu_register* source = modrm_to_register(cpu, modrm.reg);
        addr_t dest = modrm_to_address(cpu, modrm.mod, modrm.rm);
        cpu_store_u8(cpu, &cpu->ds, dest, source->regs_8);
    }
}

/*
 * 0x89: MOV r/m16/32, r16/32
 */

OPCODE_DEFINE(89)
{
    log_trace("mov r/m16/32, r16/32");
    struct modrm modrm;
    raw_to_modrm(cpu_fetch_instruction_u8(cpu), &modrm);
    if (modrm.mod == 3) {
        // Direct addressing mode, reg is source, rm is dest because d == 0
        union cpu_register* source = modrm_to_register(cpu, modrm.reg);
        union cpu_register* dest = modrm_to_register(cpu, modrm.rm);
        if (CPU_PREFIX_STATE_OPERAND32(cpu)) {
            dest->regs_32 = source->regs_32;
        } else {
            dest->regs_16 = source->regs_16;
        }
    } else {
        union cpu_register* source = modrm_to_register(cpu, modrm.reg);
        addr_t dest = modrm_to_address(cpu, modrm.mod, modrm.rm);
        if (CPU_PREFIX_STATE_OPERAND32(cpu)) {
            cpu_store_u32(cpu, &cpu->ds, dest, source->regs_32);
        } else {
            cpu_store_u16(cpu, &cpu->ds, dest, source->regs_16);
        }
    }
}

/*
 * 0x8A: MOV r8, r/m8
 */
OPCODE_DEFINE(8A)
{
    log_trace("mov r8, r/m8");
    struct modrm modrm;
    raw_to_modrm(cpu_fetch_instruction_u8(cpu), &modrm);
    if (modrm.mod == 3) {
        // TODO; Implement if mod == 3
        log_fatal("MOD 3 for opcode 0x8A is unsupported");
    } else {
        // Direct addressing mode, rm is source, reg is dest because d == 1
        union cpu_register* dest = modrm_to_register(cpu, modrm.reg);
        dest->regs_16 = cpu_fetch_u8(
            cpu, &cpu->ds, modrm_to_address(cpu, modrm.mod, modrm.rm));
    }
}

OPCODE_DEFINE(8B)
{
    log_trace("mov r8, r/m8");
    struct modrm modrm;
    raw_to_modrm(cpu_fetch_instruction_u8(cpu), &modrm);
    if (modrm.mod == 3) {
        // TODO; Implement if mod == 3
        log_fatal("MOD 3 for opcode 0x8A is unsupported");
    } else {
        // Direct addressing mode, rm is source, reg is dest because d == 1
        union cpu_register* dest = modrm_to_register(cpu, modrm.reg);
        if (CPU_PREFIX_STATE_OPERAND32(cpu)) {
            dest->regs_32 = cpu_fetch_u32(
                cpu, &cpu->ds, modrm_to_address(cpu, modrm.mod, modrm.rm));
        } else {
            dest->regs_16 = cpu_fetch_u16(
                cpu, &cpu->ds, modrm_to_address(cpu, modrm.mod, modrm.rm));
        }
    }
}

/*
 * 0xB8: MOV r16/32, imm16/32
 */
OPCODE_DEFINE(B8)
{
    if (CPU_PREFIX_STATE_OPERAND32(cpu)) {
        log_trace("mov eax, imm16");
        cpu->ax.regs_32 = cpu_fetch_instruction_u32(cpu);
    } else {
        log_trace("mov ax, imm16");
        cpu->ax.regs_16 = cpu_fetch_instruction_u16(cpu);
    }
}

/*
 * 0xBB: MOV bx, imm16/32
 */
OPCODE_DEFINE(BB)
{
    if (CPU_PREFIX_STATE_OPERAND32(cpu)) {
        log_trace("mov ebx, imm16/32");
        cpu->bx.regs_32 = cpu_fetch_instruction_u32(cpu);
    } else {
        log_trace("mov bx, imm16/32");
        cpu->bx.regs_16 = cpu_fetch_instruction_u16(cpu);
    }
}

/*
 * 0xBC: MOV sp, imm16/32
 */
OPCODE_DEFINE(BC)
{
    if (CPU_PREFIX_STATE_OPERAND32(cpu)) {
        log_trace("mov esp, imm16/32");
        cpu->sp.regs_32 = cpu_fetch_instruction_u32(cpu);
    } else {
        log_trace("mov sp, imm16/32");
        cpu->sp.regs_16 = cpu_fetch_instruction_u16(cpu);
    }
}
/*
 * 0xC3: RETN
 */
OPCODE_DEFINE(C3)
{
    log_trace("retn");
    if (CPU_PREFIX_STATE_OPERAND32(cpu)) {
        cpu->ip.regs_32 = pop_u32(cpu);
    } else {
        cpu->ip.regs_16 = pop_u16(cpu);
    }
}

/*
 * 0xE8: CALL rel16/32
 */
OPCODE_DEFINE(E8)
{
    log_trace("call rel16/32");
    if (CPU_PREFIX_STATE_OPERAND32(cpu)) {
        int32_t offset = cpu_fetch_instruction_u32(cpu);
        uint32_t eip = cpu->ip.regs_32 + offset;
        push_u32(cpu, cpu->ip.regs_32);
        cpu->ip.regs_32 = eip;
    } else {
        int16_t offset = cpu_fetch_instruction_u16(cpu);
        uint16_t eip = cpu->ip.regs_16 + offset;
        push_u16(cpu, cpu->ip.regs_16);
        cpu->ip.regs_16 = eip;
    }
}

/*
 * 0xEA: JMPF ptr16:16/32
 */
OPCODE_DEFINE(EA)
{
    log_trace("jmpf ptr16:16/32");
    uint32_t eip = 0;
    if (CPU_PREFIX_STATE_OPERAND32(cpu)) {
        eip = cpu_fetch_instruction_u32(cpu);
    } else {
        eip = cpu_fetch_instruction_u16(cpu);
    }
    uint16_t cs = cpu_fetch_instruction_u16(cpu);
    cpu->cs.base = cs << 4;
    cpu->cs.selector = cs;
    if (CPU_PREFIX_STATE_OPERAND32(cpu)) {
        cpu->ip.regs_32 = eip;
    } else {
        cpu->ip.regs_16 = eip;
    }
}

/*
 * 0xF4: HLT
 */
OPCODE_DEFINE(F4)
{
    log_trace("hlt");
    cpu->state = CPU_HALTED;
}

opcode_fn_t opcode_table[256] = {
    NULL,     NULL,     NULL,     NULL,     NULL,     NULL,     NULL,
    NULL,     NULL,     NULL,     NULL,     NULL,     NULL,     NULL,
    NULL,     NULL,     NULL,     NULL,     NULL,     NULL,     NULL,
    NULL,     NULL,     NULL,     NULL,     NULL,     NULL,     NULL,
    NULL,     NULL,     NULL,     NULL,     opcode20, NULL,     NULL,
    NULL,     NULL,     NULL,     NULL,     NULL,     NULL,     NULL,
    NULL,     NULL,     NULL,     NULL,     NULL,     NULL,     NULL,
    NULL,     NULL,     NULL,     NULL,     NULL,     NULL,     NULL,
    NULL,     NULL,     NULL,     NULL,     NULL,     NULL,     NULL,
    NULL,     NULL,     NULL,     NULL,     opcode43, NULL,     NULL,
    NULL,     opcode47, NULL,     NULL,     NULL,     NULL,     NULL,
    NULL,     NULL,     NULL,     NULL,     NULL,     opcode52, opcode53,
    NULL,     NULL,     NULL,     opcode57, NULL,     NULL,     opcode5A,
    opcode5B, NULL,     NULL,     NULL,     opcode5F, NULL,     NULL,
    NULL,     NULL,     NULL,     NULL,     NULL,     NULL,     NULL,
    NULL,     NULL,     NULL,     NULL,     NULL,     NULL,     NULL,
    NULL,     NULL,     NULL,     NULL,     NULL,     opcode75, NULL,
    NULL,     NULL,     NULL,     NULL,     NULL,     NULL,     NULL,
    NULL,     NULL,     NULL,     NULL,     NULL,     NULL,     NULL,
    NULL,     opcode86, NULL,     opcode88, opcode89, opcode8A, opcode8B,
    NULL,     NULL,     NULL,     NULL,     NULL,     NULL,     NULL,
    NULL,     NULL,     NULL,     NULL,     NULL,     NULL,     NULL,
    NULL,     NULL,     NULL,     NULL,     NULL,     NULL,     NULL,
    NULL,     NULL,     NULL,     NULL,     NULL,     NULL,     NULL,
    NULL,     NULL,     NULL,     NULL,     NULL,     NULL,     NULL,
    NULL,     NULL,     NULL,     NULL,     NULL,     NULL,     NULL,
    NULL,     NULL,     opcodeB8, NULL,     NULL,     opcodeBB, opcodeBC,
    NULL,     NULL,     NULL,     NULL,     NULL,     NULL,     opcodeC3,
    NULL,     NULL,     NULL,     NULL,     NULL,     NULL,     NULL,
    NULL,     NULL,     NULL,     NULL,     NULL,     NULL,     NULL,
    NULL,     NULL,     NULL,     NULL,     NULL,     NULL,     NULL,
    NULL,     NULL,     NULL,     NULL,     NULL,     NULL,     NULL,
    NULL,     NULL,     NULL,     NULL,     NULL,     NULL,     NULL,
    NULL,     opcodeE8, NULL,     opcodeEA, NULL,     NULL,     NULL,
    NULL,     NULL,     NULL,     NULL,     NULL,     NULL,     opcodeF4,
    NULL,     NULL,     NULL,     NULL,     NULL,     NULL,     NULL,
    NULL,     NULL,     NULL,     NULL,
};

void opcode_execute(struct cpu* cpu)
{
    uint8_t opcode = 0;
    bool isPrefix = 1;
    while (isPrefix) {
        switch ((opcode = cpu_fetch_instruction_u8(cpu))) {
            case 0x2E:
                log_trace("Prefix: %X", opcode);
                cpu->prefix_state.segment = &cpu->cs;
                break;
            case 0x66:
                log_trace("Prefix: %X", opcode);
                cpu->prefix_state.operand32 = 1;
                break;
            default:
                isPrefix = 0;
                break;
        }
    }
    log_trace("Opcode: %X", opcode);
    if (!opcode_table[opcode]) {
        log_fatal("Unknown opcode, should probably throw an Invalid Opcode "
                  "exception, got opcode 0x%X at IP 0x%X",
                  opcode, cpu->cs.base + cpu->ip.regs_16 - 1);
        cpu->state = CPU_HALTED;
        return;
    }
    opcode_table[opcode](cpu);

    // Reset CPU prefix state
    cpu->prefix_state.segment = NULL;
    cpu->prefix_state.operand32 = 0;
    cpu->prefix_state.repne = 0;
    cpu->prefix_state.repe = 0;
}
